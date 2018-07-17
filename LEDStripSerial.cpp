

void LEDStripSerial::write(const char* data, size_t size)
{
  if (bit_depth_ == BIT_8)
  {
    BufferedAsyncSerial::write(data, size);
    return;
  }
  else
  {
    LEDStripSerial::compressTo12Bit(data, size);
  }
}

void LEDStripSerial::write(const unsigned char *data, size_t size);
{
  this->write(reinterpret_cast<char*> data, size);
}

void LEDStripSerial::write(const std::vector<char> &data)
{
  
}

void LEDStripSerial::write(const uint16_t* data, size_t size)
{

}

void LEDStripSerial::write(const std::vector<uint16_t>& data)
{

}

void LEDStripSerial::write(const uint8_t* data, size_t size)
{

}

void LEDStripSerial::write(const std::vector<uint8_t>& data)
{
  
}



LEDStripSerial::compressTo12Bit(unsigned char* data, size_t size)
{
  // TODO: Implement.
}

LEDStripSerial::compressTo12Bit(std::vector<char> &data)
{
  // TODO: Implement.
}
