#include "Arduino.h"
#include "AD7689_Driver.hpp"
#include <SPI.h>

void AD7689_Driver::startCoversion(void){
#if defined(ARDUINO)
    digitalWrite(__cs, LOW);
    delayMicroseconds(1);
    digitalWrite(__cs, HIGH);
    //delayMicroseconds(2);//延时2us，等待转换完成，SPI 10MHz
#endif
}

void AD7689_Driver::init(u16_t cfg=0x3FF9)
{
#if defined(ARDUINO)
    if (__spi) {
        conversionConfig(cfg);
    }
#endif
}

void AD7689_Driver::conversionConfig(u16_t cfg)
{
    readAndconfig(cfg);
    readAndconfig(cfg);
    readAndconfig(cfg);//初始化上电后，写入第三次cfg开始有效
}
u16_t AD7689_Driver::readAndconfig(u16_t cfg)
{
    u16_t data;
#if defined(ARDUINO)
    if (__spi) {
        startCoversion();//开始转换
        __spi->beginTransaction(*__spiSetting);
        digitalWrite(__cs, LOW);
        data=__spi->transfer16(cfg);
        digitalWrite(__cs, HIGH);
        __spi->endTransaction();
        delayMicroseconds(2);//延时2us，等待转换完成，SPI 10MHz
    }
    currentCFG=cfg;
#endif
    return data;
}

u16_t AD7689_Driver::readRawdata(void){
#if defined(ARDUINO)
     return readAndconfig(0x0000);
#endif
}

void AD7689_Driver::readLine(u16_t* data)
{
    static u8_t lineScan=0;
#if defined(ARDUINO)
    if(lineScan==0){

    }
#endif
}

