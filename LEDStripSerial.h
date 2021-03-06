/**
 * @file LEDStripSerial.h
 *
 * This class is just a nicer interface to the library in the folder
 * Serial_Port_Communication. 
 * 
 * @author Joshua Petrin
 */

#ifndef LEDSTRIPSERIAL_H_
#define LEDSTRIPSERIAL_H_

#include "Serial_Port_Communication/BufferedAsyncSerial.h"

#include <string>
#include <vector>
#include <cstdint>

// Forward declaration
class LEDStripSerial;

/**
 * @brief The direct USB serial interface class. Extends the 
 *        BufferedAsyncSerial class, so users can call 
 *        BufferedAsyncSerial::write() on it.
 *
 * This class can send asynchronous messages to the USB port with the help of 
 * Boost's asio library. That is, the USB port will send data in another thread
 * as the main program so program flow will not be interrupted. 
 * 
 * The only difference between this class and the BufferedAsyncSerial class is
 * that this one has a predefined baud_rate enum. To use, simply call the 
 * methods that are inherited from BufferedAsyncSerial (see the Doxygen 
 * inherited methods list above) once you have called the constructor.
 *
 * @warning Though the predefined baud_rate's should give few problems, not 
 *          all baud_rate's are supported on all platforms. If deviating from 
 *          the default BAUD_9600, please ensure your platfrom supports your 
 *          baud rate. 
 */
class LEDStripSerial : public BufferedAsyncSerial
{
private:
  /// Private default constructor.
  LEDStripSerial(void);

protected:
  int bit_depth_;
  bool little_endian_system_;

  /**
   * @brief Pack a 12-bit RGB list of values.
   *
   * This function assumes that `data` is a 12-bit RGB string of `char`s; that
   * is, its structure looks something like this:
   * ```
   * { 0x01  0x23  0x04  0x56  0x07  0x89  0x01  0x23 }
   * ```
   * although the 4 most-significant bits of even-indexed `char`s will be 
   * ignored by this function. If passed into this function, the above array
   * will be transformed into the following one:
   * ```
   * { 0x12  0x34  0x56  0x78  0x91  0x23  0x00  0x00 }
   * ```
   * so the values are 'smashed' together and made into a contiguous string.
   *
   * Since the array itself is manipulated, the return value is the new length
   * of the occupied array (although the actual array itself still occupies the
   * same amount of memory). Also, if there are an odd number of 12-bit RGB 
   * values in `data`, the final array will end with a byte that only has its
   * first 4 bits filled, e.g. `{ 0x61 0x23 } => { 0x12 0x30 }` (return `= 2`).
   * 
   * @param  data The array to be packed
   * @param  size The size of `data`
   * @return      The size of the changed `data` array, with the half-byte at 
   *              the end counting as a whole one byte.
   */
  static size_t compress12Bit(unsigned char* data, size_t size);

  /**
   * @brief Pack a 12-bit RGB list of values.
   * 
   * Does the same thing as `compress12Bit(unsigned char*, size_t)` 
   * essentially, but resizes `data` as well (e.g. `data.size()` will return 
   * the actual size of the buffer).
   * 
   * @param  data An `std::vector` with RGB values to be compressed
   * @see compress12Bit(unsigned char* data, size_t size)
   */
  static std::vector<char> compress12Bit(const std::vector<char> &data);  


public:

  enum baud_rate { 
    BAUD_1200 = 1200,
    BAUD_2400 = 2400,
    BAUD_4800 = 4800,
    BAUD_9600 = 9600, 
    BAUD_19200 = 19200,
    BAUD_38400 = 38400,
    BAUD_57600 = 57600,
    BAUD_74880 = 74880,
    BAUD_115200 = 115200,
    BAUD_230400 = 230400
  };

  enum color_depth { 
    BIT_12,
    BIT_8
  };

  /**
   * @constructor Calls the super BufferedAsyncSerial(std::string, unsigned int,
   *              ...) constructor.
   *
   * @param port_name The port from which Boost asio will transmit serial data
   * @param rate      The baud_rate at which Boost will transmit (default 9600)
   */
  LEDStripSerial(
    std::string port_name, 
    LEDStripSerial::baud_rate rate = LEDStripSerial::baud_rate::BAUD_9600,
    LEDStripSerial::color_depth depth = LEDStripSerial::color_depth::BIT_12);

  /**
   * @brief Set the color depth of the receiving LED strip. 
   *
   * If set to `BIT_8`, all messages will pass through the stream unadulterated.
   * If set to `BIT_12`, all messages will be sent as their shortened 12-bit 
   * representations
   * 
   * @param depth The desired color depth of the signal. 
   * 
   * @see compress12Bit(unsigned char* data, size_t size)
   */
  void setColorDepth(LEDStripSerial::color_depth depth)
  { bit_depth_ = depth; } 

  /**
   * @return The color depth this serial class is currently transmitting for.
   */
  LEDStripSerial::color_depth getColorDepth() const
  { return LEDStripSerial::color_depth(bit_depth_); }

  /**
   * @brief Write a `char` array to the USB serial port. 
   * @param data The data to send (8- or 12-bit RGB)
   * @param size The length of the array to be considered
   *
   * @see AsyncSerial::write(const char *data, size_t size)
   */
  void write(const char* data, size_t size);

  /**
   * @brief Write an `unsigned char` array to the USB serial port. 
   * @param data The data to send (8- or 12-bit RGB)
   * @param size The length of the array to be considered
   */
  void write(const unsigned char* data, size_t size);

  /**
   * @brief Write a `std::vector<char>` to the USB serial port. 
   * @param data The data to send (8- or 12-bit RGB)
   *
   * @see AsyncSerial::write(const std::vector<char> &data)
   */
  void write(const std::vector<char> &data);

  /**
   * @brief Write a `uint16_t` array to the USB serial port. 
   *
   * It's assumed that an 8-bit color depth has values next to one another. 
   * That is, `{0x1234, 0x5678, etc.}`, and no bytes are wasted. 
   * It's assumed that a 12-bit color depth has one RGB value per `uint16_t`.
   * That is, `{0x0123, 0x0456, 0x0789, etc.}`, with the first 4 bits wasted.
   *
   * Obviously, it would make very little sense to use this method with `BIT_8`
   * bit depth. 
   * 
   * @param data The data to send (8- or 12-bit RGB)
   * @param size The length of the array to be considered
   */
  void write(const uint16_t* data, size_t size);

  /**
   * @brief Write a `std::vector<uint16_t>` to the USB serial port. 
   * @param data The data to send (8- or 12-bit RGB)
   *
   * @see write(const uint16_t* data, size_t size)
   */
  void write(const std::vector<uint16_t>& data);

  /**
   * @brief Write a `std::vector<uint8_t>` to the USB serial port. 
   * @param data The data to send (8- or 12-bit RGB)
   *
   * @see write(const unsigned char* data, size_t size)
   */
  void write(const std::vector<uint8_t>& data);

};

#endif //LEDSTRIPSERIAL_H_