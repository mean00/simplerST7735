/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 * 
 * This is a demo init code to :
 *   * initialize a 128x128 screen
 *   * based on roger clark STM32duino framework
 * 
 * 
 */
#include "stm32duinoST7735.h"
#include "simplerST7735_priv.h"
#include "SPI.h"

//#define SPI_USE_DMA 
/**
 * 
 * @param w
 * @param h
 * @param spi
 * @param pinDc
 * @param pinCS
 */
stm32duinoST7735::stm32duinoST7735(int w, int h,  int  pinDc, int pinCS) :  st7735(w,h,pinDc,pinCS)
{
    _PhysicalXoffset=2;
    _PhysicalYoffset=1;
}
/**
 * 
 */
stm32duinoST7735::~stm32duinoST7735()
{

}
// This is the init sequence for the 160x80 LCd on the longan nano
void stm32duinoST7735::init()
{
    
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    
    baseInit();
  // Init sequence borrowed from adafruit
#define DELAY 0x80
    static const uint8_t init_sequence[] =
    //--------------rcmd1---------------
    {                 
    ST7735_SWRESET, DELAY  , 150,  0xff,  
    ST7735_SLPOUT , DELAY  , 255,  0xff, 
    ST7735_FRMCTR1, 3      , 0x01, 0x2C, 0x2D, 0xff,  
    ST7735_FRMCTR2, 3      , 0x01, 0x2C, 0x2D, 0xff,    
    ST7735_FRMCTR3, 6      , 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,  0xff,      
    ST7735_INVCTR , 1      , 0x07, 0Xff, 
    ST7735_PWCTR1 , 3      , 0xA2, 0x02, 0x84, 0Xff,
    ST7735_PWCTR2 , 1      , 0xC5, 0xff,
    ST7735_PWCTR3 , 2      , 0x0A, 0x00, 0xff,    
    ST7735_PWCTR4 , 2      , 0x8A, 0x2A, 0xff,     
    ST7735_PWCTR5 , 2      , 0x8A, 0xEE, 0xff,
    ST7735_VMCTR1 , 1      , 0x0E, 0xff,  
    ST7735_INVOFF , 0      , 0xff, // 
    ST7735_MADCTL , 1      , 0xC8, 0xff,
    ST7735_COLMOD , 1 ,      0x05 ,0xff,
     
//-----------------------------Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)    
    ST7735_CASET  , 4      , 0x00, 0x02,  0x00, 0x7F+0x02,   0xff,  
    ST7735_RASET  , 4,       0x00, 0x01,  0x00, 0x9F+0x01 ,  0xff, 
//-------------------------------Rcmd3
                
    ST7735_GMCTRP1,     16,  0x02, 0x1c, 0x07, 0x12,  0x37, 0x32, 0x29, 0x2d,  0x29, 0x25, 0x2B, 0x39,0x00, 0x01, 0x03, 0x10,  0xff,//  1: Magical unicorn dust, 16 args, no delay:
    ST7735_GMCTRN1,     16 , 0x03, 0x1d, 0x07, 0x06,   0x2E, 0x2C, 0x29, 0x2D,0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,  0xff,//  2: Sparkles and rainbows, 16 args, no delay:
    ST7735_NORON  ,   DELAY, 10,   0xff,                     //  //  3: Normal display on, no args, w/delay   10 ms delay
    ST7735_DISPON ,   DELAY, 100,  0xff,//  4: Main screen turn on, no args w/delay
    0xff};                  //     100 ms delay

  const uint8_t *gp=init_sequence;
  digitalWrite(_pinCS,LOW);
  while(1)
  {
      if(gp[0]==0xff) break;
      int size=gp[1];
      if(size==DELAY)
      {
          xAssert(gp[3]==0xff)
          sendCommand(gp[0],0,gp+1);
          size=1;
          xDelay(gp[2]);
      }
      else
      {
        xAssert(gp[size+2]==0xff)
        sendCommand(gp[0],size,gp+2);
      }      
      gp+=size+3;        
  }   
  digitalWrite(_pinCS,HIGH);    
  
  
}
/**
 * 
 * @param byte
 */
void stm32duinoST7735::sendByte(int byte)
{
    SPI.setDataSize (SPI_CR1_DFF_8_BIT); // Set spi 16bit mode  
    SPI.write(byte);
}
/**
 * 
 * @param byte
 */
void stm32duinoST7735::sendWord(int byte)
{
   SPI.setDataSize (SPI_CR1_DFF_16_BIT); 
   SPI.write16(byte);
}
static const uint8_t rotMode[4]={0x8,0xc8,0x78,0xa8};
/**
 * 
 */
void stm32duinoST7735::updateHwRotation(void)
{
    sendCommand(ST7735_MADCTL,1,rotMode+_rotation);
}

#ifdef SPI_USE_DMA  
void stm32duinoST7735::sendBytes(int nb, const uint8_t *data)
{
    SPI.setDataSize (SPI_CR1_DFF_8_BIT); // Set spi 16bit mode  
    SPI.dmaSend(data, nb, false);
}
void stm32duinoST7735::sendWords(int nb, const uint16_t *data)
{
    SPI.setDataSize (SPI_CR1_DFF_16_BIT); // Set spi 16bit mode  
    SPI.dmaSend(data, nb, false);
}
void stm32duinoST7735::floodWords(int nb, const uint16_t data)
{
    SPI.setDataSize (SPI_CR1_DFF_16_BIT); // Set spi 16bit mode  
    SPI.dmaSend(&data, nb, true);
}
#else
void stm32duinoST7735::sendBytes(int nb, const uint8_t *data)
{
    SPI.setDataSize (SPI_CR1_DFF_8_BIT); // Set spi 16bit mode  
     for(int i=0;i<nb;i++)
          SPI.write(data[i]);      
}
void stm32duinoST7735::sendWords(int nb, const uint16_t *data)
{
    SPI.setDataSize (SPI_CR1_DFF_16_BIT); // Set spi 16bit mode  
      for(int i=0;i<nb;i++)
          SPI.write16(data[i]);      
}
void stm32duinoST7735::floodWords(int nb, const uint16_t data)
{
    SPI.setDataSize (SPI_CR1_DFF_16_BIT); // Set spi 16bit mode  
   for(int i=0;i<nb;i++)
          SPI.write16(data);      
}


#endif

// EOF