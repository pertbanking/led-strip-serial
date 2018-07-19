

#include "LEDStripSerial.h"

#include <iostream>


int main()
{
  unsigned char stuff1[] = { 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x23, 0x34 };  // 8->6
  unsigned char stuff2[] = { 0x01, 0x23, 0x04, 0x56, 0x07, 0x89, 0x01, 0x23 };  // 8->6
  unsigned char stuff3[] = { 0x01, 0x23, 0x04, 0x56, 0x07, 0x89 };              // 6->5

  LEDStripSerial::compress12Bit(stuff1, 8);
  std::cout << "stuff1: { ";
  for (int i = 0; i < 8; ++i)
    printf("0x%X ", stuff1[i]);
  std::cout << "}" << std::endl;

  LEDStripSerial::compress12Bit(stuff2, 8);
  std::cout << "stuff2: { ";
  for (int i = 0; i < 8; ++i)
    printf("0x%X ", stuff2[i]);
  std::cout << "}" << std::endl;

  LEDStripSerial::compress12Bit(stuff3, 6);
  std::cout << "stuff3: { ";
  for (int i = 0; i < 6; ++i)
    printf("0x%X ", stuff3[i]);
  std::cout << "}" << std::endl;

  return 0;
}