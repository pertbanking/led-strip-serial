/**
 * @file LEDStripSerial.cpp
 *
 * @author Joshua Petrin
 */

#ifndef LEDSTRIPSERIAL_CPP_
#define LEDSTRIPSERIAL_CPP_

#include "LEDStripSerial.h"

#include <algorithm>
#include <memory>
#include <iostream>


LEDStripSerial::LEDStripSerial(
    std::string port_name,
    LEDStripSerial::baud_rate rate,
    LEDStripSerial::color_depth depth) :
      BufferedAsyncSerial(
        port_name, 
        (unsigned int) rate),
      bit_depth_(depth)
  {
    // determine the endianness of the system by seeing how it treats adjacent
    // bytes
    unsigned char test_char[] = { 0x01, 0x00 };
    uint16_t* test = reinterpret_cast<uint16_t*>(test_char);
    little_endian_system_ = (*test == 0x0001);  // would otherwise be 0x0100
  }

void LEDStripSerial::write(const char* data, size_t size)
{
  if (bit_depth_ == BIT_12)
  {
    if (size % 2 != 0)
    {
      // The memory has not been passed correctly.
      throw std::domain_error("12-bit color depths only ever have arrays with"
        "even sizes.");
    }
    std::unique_ptr<char[]> data_copy(new char[size]);
    std::copy(data, data + size, data_copy.get());

    size_t new_size = 
      LEDStripSerial::compress12Bit(
        reinterpret_cast<unsigned char*>(data_copy.get()), 
        size);

    BufferedAsyncSerial::write(data_copy.get(), new_size);
  }
  else
  {
    BufferedAsyncSerial::write(data, size);
    return;
  }


}

void LEDStripSerial::write(const unsigned char *data, size_t size)
{
  this->write(reinterpret_cast<const char*>(data), size);
}

void LEDStripSerial::write(const std::vector<char> &data)
{
  if (bit_depth_ == BIT_12)
  {
    std::vector<char> data_copy = LEDStripSerial::compress12Bit(data);
    BufferedAsyncSerial::write(data_copy);
  }
  else
  {
    BufferedAsyncSerial::write(data);
  }
}

void LEDStripSerial::write(const uint16_t* data, size_t size)
{
  // Just convert these numbers into 8-bit chars and shove off
  size_t bit_8_size = size*2;
  const char* data_char = reinterpret_cast<const char*>(data);
  
  std::unique_ptr<char[]> data_char_copy(new char[bit_8_size]);
  std::copy(data_char, data_char + bit_8_size, data_char_copy.get());

  if (little_endian_system_)
  {
    // every pair of chars is backwards. swap every pair.
    for (int i = 0; i < bit_8_size; i += 2)
    {
      char tmp = data_char_copy[i];
      data_char_copy[i]   = data_char_copy[i+1];
      data_char_copy[i+1] = tmp;
    }
  }
  
  // 8- and 12-bit check is done in the write(char*, size_t) method.
  this->write(data_char_copy.get(), bit_8_size);
}

void LEDStripSerial::write(const std::vector<uint16_t>& data)
{
  std::vector<char> data_char;
  const char* data_char_ptr = reinterpret_cast<const char*>(data.data());
  std::copy(
    data_char_ptr, 
    data_char_ptr + data.size()*2, 
    std::back_inserter(data_char));

  // 8- and 12-bit check is done in the write(char*, size_t) method.
  this->write(data_char);
}

void LEDStripSerial::write(const std::vector<uint8_t>& data)
{
  std::vector<char> cast_data;
  std::transform(
    data.begin(), 
    data.end(), 
    std::back_inserter(cast_data), 
    [](uint8_t in) {
      return *reinterpret_cast<char*>(&in);
    } );

  this->write(cast_data);
}



size_t LEDStripSerial::compress12Bit(unsigned char* data, size_t size)
{
  size_t new_size = (size/4)*3 + ((size % 4 != 0)? 2 : 0);

  // Map the 12-bit char values
  // { 0x01, 0x23, 0x04, 0x56, 0x07, 0x89 }
  // to these new char values:
  // { 0x12, 0x34, 0x56, 0x78, 0x90 }
  for (int i = 0; i < (size/4)*3; i += 3)
  {
    data[i]   = (data[(i/3)*4] << 4) | (data[(i/3)*4 + 1] >> 4);
    data[i+1] = (data[(i/3)*4 + 1] << 4) | (data[(i/3)*4 + 2] & 0x0F);
    data[i+2] = data[(i/3)*4 + 3];
  }
  if (size % 4 != 0)
  {
    // consider the final 12-bit on the end
    data[new_size-2] = (data[size-2] << 4) | (data[size-1] >> 4);
    data[new_size-1] = data[size-1] << 4;
  }

  // Fill the data array with the changed values from the new array and fill 
  // the remainder of values with 0.
  for (int i = new_size; i < size; ++i)
    data[i] = 0;

  return new_size;
}

std::vector<char> LEDStripSerial::compress12Bit(const std::vector<char> &data)
{
  std::vector<char> data_copy;
  std::copy(data.begin(), data.end(), std::back_inserter(data_copy));
  size_t new_size = 
    LEDStripSerial::compress12Bit(
      reinterpret_cast<unsigned char*>(data_copy.data()), 
      data_copy.size());

  data_copy.resize(new_size);
  return data_copy;
}

#endif //LEDSTRIPSERIAL_CPP_