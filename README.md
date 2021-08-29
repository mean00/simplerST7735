The is a very portable ST7735 LCD library
To use it, derive the class st7735 and add your own functions to provide :
              void sendByte(int byte); // 8 bytes
              void sendWord(int byte); // 16 bytes
              void sendBytes(int nb, const uint8_t *data); // 8 bits
              void sendWords(int nb, const uint16_t *data); // 16 bits
              void floodWords(int nb, const uint16_t data); // 16 bits
By design it works over SPI, I2C, ....              
Example for lnArduino and Roger Clark STM32 are provided              
