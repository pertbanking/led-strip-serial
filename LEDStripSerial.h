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

#include <vector>
#include <string>
#include <cstdint>


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
  LEDStripSerial::color_depth bit_depth_;

  // TODO; Implement these functions
  static <return> compressTo12Bit(unsigned char* data, size_t size);

  static <return> compressTo12Bit(std::vector<char> &data);  

  LEDStripSerial(void);

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
    LEDStripSerial::baud_rate rate = LEDStripSerial::baud_rate::BAUD_9600
    LEDStripSerial::color_depth depth = LEDStripSerial::baud_rate::BIT_12) : 
      BufferedAsyncSerial(
        port_name, 
        (unsigned int) rate),
      bit_depth_(depth)
  {
  }

  void setColorDepth(LEDStripSerial::color_depth depth)
  { bit_depth_ = depth; } 

  LEDStripSerial::color_depth getColorDepth() const
  { return bit_depth_; }

  void write(const char* data, size_t size);

  void write(const unsigned char* data, size_t size);

  void write(const std::vector<char> &data);

  void write(const uint16_t* data, size_t size);

  void write(const std::vector<uint16_t>& data);

  void write(const uint8_t* data, size_t size);

  void write(const std::vector<uint8_t>& data);

};

#endif //LEDSTRIPSERIAL_H_