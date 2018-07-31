

#include "LEDStripSerial.h"

#include <iostream>


int main()
{
  // Begin testing.

  LEDStripSerial serial(
    "/dev/ttyACM0", 
    LEDStripSerial::BAUD_9600, 
    LEDStripSerial::BIT_12);  // for debugging

  { // char*
    std::cout << "Char: " << std::endl;

    // This says 'hello!' in hex
    char stuff1[] = { 0x06, -0x7A, 0x05, 0x6C, 0x06, -0x3A, 0x0F, 0x21 };  // 8->6
    char stuff2[] = { 0x01, 0x23, 0x04, 0x56, 0x07, 0x09, 0x01, 0x23 };  // 8->6
    char stuff3[] = { 0x01, 0x23, 0x04, 0x56, 0x07, 0x12 };              // 6->5
    char newline[] = { '\n' };
    
    serial.write(stuff1, 8);
    serial.setColorDepth(LEDStripSerial::BIT_8);
    serial.write(newline, 1);
    serial.setColorDepth(LEDStripSerial::BIT_12);
    std::cout << "stuff1: { ";
    for (int i = 0; i < 8; ++i)
      printf("0x%X ", stuff1[i]);
    std::cout << "}" << std::endl;
  
    // serial.write(stuff2, 8);
    // serial.write(newline, 1);
    // std::cout << "stuff2: { ";
    // for (int i = 0; i < 8; ++i)
    //   printf("0x%X ", stuff2[i]);
    // std::cout << "}" << std::endl;
  
    // serial.write(stuff3, 6);
    // serial.write(newline, 1);
    // std::cout << "stuff3: { ";
    // for (int i = 0; i < 6; ++i)
    //   printf("0x%X ", stuff3[i]);
    // std::cout << "}" << std::endl;
  
    // std::cout << std::endl;
  }

  return 0;

  { // unsigned char*
    std::cout << "Unsigned char: " << std::endl;

    unsigned char stuff1[] = { 0x12, 0x34, 0x56, 0x78, 0x12, 0x23, 0x34, 0x45 };  // 8->6
    unsigned char stuff2[] = { 0x01, 0x23, 0x04, 0x56, 0x07, 0x09, 0x01, 0x23 };  // 8->6
    unsigned char stuff3[] = { 0x01, 0x23, 0x04, 0x56, 0x07, 0x12 };              // 6->5
    
    serial.write(stuff1, 8);
    std::cout << "stuff1: { ";
    for (int i = 0; i < 8; ++i)
      printf("0x%X ", stuff1[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff2, 8);
    std::cout << "stuff2: { ";
    for (int i = 0; i < 8; ++i)
      printf("0x%X ", stuff2[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff3, 6);
    std::cout << "stuff3: { ";
    for (int i = 0; i < 6; ++i)
      printf("0x%X ", stuff3[i]);
    std::cout << "}" << std::endl;
  
    std::cout << std::endl;
  }

  { // uint16_t
    std::cout << "uint16_t: " << std::endl;

    uint16_t stuff1[] = { 0x1234, 0x5678, 0x1234, 0x5678 };  // 4->??
    uint16_t stuff2[] = { 0x0123, 0x0012, 0x1234 };  // 3->??
    uint16_t stuff3[] = { 0x1234, 0x5678, 0x9101, 0x1121, 0x3141 };  // 5->??
    
    serial.write(stuff1, 4);
    std::cout << "stuff1 literal: { ";
    for (int i = 0; i < 4; ++i)
      printf("0x%X ", stuff1[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff2, 3);
    std::cout << "stuff2 literal: { ";
    for (int i = 0; i < 3; ++i)
      printf("0x%X ", stuff2[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff3, 5);
    std::cout << "stuff3 literal: { ";
    for (int i = 0; i < 5; ++i)
      printf("0x%X ", stuff3[i]);
    std::cout << "}" << std::endl;
  
    std::cout << std::endl;
  }

  { // std::vector<char>
    std::cout << "Vector<char>: " << std::endl;

    std::vector<char> stuff1 { 0x12, 0x34, 0x56, 0x78, 0x12, 0x23, 0x34, 0x45 };  // 8->6
    std::vector<char> stuff2 { 0x01, 0x23, 0x04, 0x56, 0x07, 0x09, 0x01, 0x23 };  // 8->6
    std::vector<char> stuff3 { 0x01, 0x23, 0x04, 0x56, 0x07, 0x12 };              // 6->5
  
    serial.write(stuff1);
    std::cout << "stuff1: { ";
    for (int i = 0; i < stuff1.size(); ++i)
      printf("0x%X ", stuff1[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff2);
    std::cout << "stuff2: { ";
    for (int i = 0; i < stuff2.size(); ++i)
      printf("0x%X ", stuff2[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff3);
    std::cout << "stuff3: { ";
    for (int i = 0; i < stuff3.size(); ++i)
      printf("0x%X ", stuff3[i]);
    std::cout << "}" << std::endl;

    std::cout << std::endl;
  }

  { // std::vector<uint8_t>
    std::cout << "Vector<uint8_t>: " << std::endl;

    std::vector<uint8_t> stuff1 { 0x12, 0x34, 0x56, 0x78, 0x12, 0x23, 0x34, 0x45 };  // 8->6
    std::vector<uint8_t> stuff2 { 0x01, 0x23, 0x04, 0x56, 0x07, 0x09, 0x01, 0x23 };  // 8->6
    std::vector<uint8_t> stuff3 { 0x01, 0x23, 0x04, 0x56, 0x07, 0x12 };              // 6->5
  
    serial.write(stuff1);
    std::cout << "stuff1: { ";
    for (int i = 0; i < stuff1.size(); ++i)
      printf("0x%X ", stuff1[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff2);
    std::cout << "stuff2: { ";
    for (int i = 0; i < stuff2.size(); ++i)
      printf("0x%X ", stuff2[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff3);
    std::cout << "stuff3: { ";
    for (int i = 0; i < stuff3.size(); ++i)
      printf("0x%X ", stuff3[i]);
    std::cout << "}" << std::endl;

    std::cout << std::endl;
  }

  { // std::vector<uint16_t>
    std::cout << "Vector<uint16_t>: " << std::endl;

    std::vector<uint16_t> stuff1 { 0x1234, 0x5678, 0x1223, 0x3445 };  // 4->??
    std::vector<uint16_t> stuff2 { 0x0123, 0x0456, 0x0789, 0x0123 };  // 4->??
    std::vector<uint16_t> stuff3 { 0x0123, 0x0456, 0x0712 };          // 3->??
  
    serial.write(stuff1);
    std::cout << "stuff1: { ";
    for (int i = 0; i < stuff1.size(); ++i)
      printf("0x%X ", stuff1[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff2);
    std::cout << "stuff2: { ";
    for (int i = 0; i < stuff2.size(); ++i)
      printf("0x%X ", stuff2[i]);
    std::cout << "}" << std::endl;
  
    serial.write(stuff3);
    std::cout << "stuff3: { ";
    for (int i = 0; i < stuff3.size(); ++i)
      printf("0x%X ", stuff3[i]);
    std::cout << "}" << std::endl;
  }



  // TODO: Test each method. 

  return 0;
}