/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "SPI.h"
#include "simplerST7735.h"
#include "MapleFreeRTOS1000_pp.h"
/**
 * 
 * @param w
 * @param h
 * @param spi
 * @param pinDc
 * @param pinCS
 */
class stm32duinoST7735 : public st7735
{
    public:
                     stm32duinoST7735(int w, int h,  int  pinDc, int pinCS);
             virtual ~stm32duinoST7735();
             virtual void init();
             virtual void sendByte(int byte); // 8 bytes
             virtual void sendWord(int byte); // 16 bytes
             virtual void sendBytes(int nb, const uint8_t *data); // 8 bits
             virtual void sendWords(int nb, const uint16_t *data); // 16 bits
             virtual void floodWords(int nb, const uint16_t data); // 16 bits
             virtual void updateHwRotation();
    protected:
                    int dataSize;
            void setSpiSize(int s)
            {
                if(s==dataSize) return;
                switch(s)
                {
                    case 8: SPI.setDataSize (DATA_SIZE_8BIT); break;
                    case 16:SPI.setDataSize (DATA_SIZE_16BIT); break;
                    default: xAssert(0);
                }
                dataSize=s;
            }                                        
};