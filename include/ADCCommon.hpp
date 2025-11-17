#pragma once

#if defined(ARDUINO)

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define PLATFORM_SPI_TYPE               SPIClass
#define PLATFORM_WIRE_TYPE              TwoWire
#define SPI_DATA_ORDER  SPI_MSBFIRST
#define DEFAULT_SDA     (SDA)
#define DEFAULT_SCL     (SCL)
#define DEFAULT_SPISETTING  SPISettings(__freq, __dataOrder, __dataMode);

#define SENSOR_PIN_NONE     (-1)
#define DEV_WIRE_NONE       (0)
#define DEV_WIRE_ERR        (-1)
#define DEV_WIRE_TIMEOUT    (-2)
// #define USEFREERTOS
#ifdef USEFREERTOS
#define msdelay(x) vTaskDelay(x)
#else
#define msdelay(x) delay(x)
#define usdelay(x) delayMicroseconds(x)
#endif

typedef union  {
    struct {
        uint8_t low;
        uint8_t high;
    } bits;
    uint16_t full;
} RegData_t;

class ADCCommon
{
    public:
        ~ADCCommon()
        {
            if (__spiSetting) {
                delete __spiSetting;
            }
        }

        void setSpiSetting(uint32_t freq, uint8_t dataOrder = SPI_DATA_ORDER, uint8_t dataMode = SPI_MODE0);
        bool begin(PLATFORM_WIRE_TYPE &w, uint8_t addr, int sda, int scl);
        bool begin(int cs=-1, int mosi = -1, int miso = -1, int sck = -1,PLATFORM_SPI_TYPE &spi = SPI);

        //! Write method
        int writeCommand(uint8_t command);
        int writeRegister(uint8_t reg, uint8_t norVal, uint8_t orVal);
        int writeRegister(int reg, uint8_t *buf, uint8_t length);
        int writeRegister(int reg, uint8_t val);
        int writeRegister(int reg, RegData_t data);

        // Read method
        int readRegister(int reg, uint8_t *buf, uint8_t length);
        int readRegister(int reg);
        int readRegister(int reg, RegData_t *data);

        bool inline clrRegisterBit(int registers, uint8_t bit)
        {
            int val = readRegister(registers);
            if (val == DEV_WIRE_ERR) {
                return false;
            }
            return  writeRegister(registers, (val & (~_BV(bit)))) == 0;
        }

        bool inline setRegisterBit(int registers, uint8_t bit)
        {
            int val = readRegister(registers);
            if (val == DEV_WIRE_ERR) {
                return false;
            }
            return  writeRegister(registers, (val | (_BV(bit)))) == 0;
        }

        bool inline getRegisterBit(int registers, uint8_t bit)
        {
            int val = readRegister(registers);
            if (val == DEV_WIRE_ERR) {
                return false;
            }
            return val & _BV(bit);
        }

        uint16_t inline readRegisterH8L4(uint8_t highReg, uint8_t lowReg)
        {
            int h8 = readRegister(highReg);
            int l4 = readRegister(lowReg);
            if (h8 == DEV_WIRE_ERR || l4 == DEV_WIRE_ERR)return 0;
            return (h8 << 4) | (l4 & 0x0F);
        }

        uint16_t inline readRegisterH8L5(uint8_t highReg, uint8_t lowReg)
        {
            int h8 = readRegister(highReg);
            int l5 = readRegister(lowReg);
            if (h8 == DEV_WIRE_ERR || l5 == DEV_WIRE_ERR)return 0;
            return (h8 << 5) | (l5 & 0x1F);
        }

        uint16_t inline readRegisterH6L8(uint8_t highReg, uint8_t lowReg)
        {
            int h6 = readRegister(highReg);
            int l8 = readRegister(lowReg);
            if (h6 == DEV_WIRE_ERR || l8 == DEV_WIRE_ERR)return 0;
            return ((h6 & 0x3F) << 8) | l8;
        }

        uint16_t inline readRegisterH5L8(uint8_t highReg, uint8_t lowReg)
        {
            int h5 = readRegister(highReg);
            int l8 = readRegister(lowReg);
            if (h5 == DEV_WIRE_ERR || l8 == DEV_WIRE_ERR)return 0;
            return ((h5 & 0x1F) << 8) | l8;
        }

        void setRegAddressLength(uint8_t len)
        {
            __reg_addr_len = len;
        }


        void setReadRegisterSendStop(bool sendStop)
        {
            __sendStop = sendStop;
        }

    protected:
        bool                __has_init              = false;
        PLATFORM_WIRE_TYPE  *__wire                 = NULL;
        PLATFORM_SPI_TYPE   *__spi                  = NULL;
        SPISettings         *__spiSetting           = NULL;
        uint8_t             __dataOrder             = SPI_DATA_ORDER;
        uint8_t             __dataMode              = SPI_MODE0;
        uint32_t            __freq                  = 400000;

        int                 __readMask              = -1;
        int                 __sda                   = -1;
        int                 __scl                   = -1;
        int                 __cs                    = -1;
        int                 __miso                  = -1;
        int                 __mosi                  = -1;
        int                 __sck                   = -1;
        bool                __sendStop              = true;
        uint8_t             __addr                  = 0xFF;
        uint8_t             __reg_addr_len          = 1;
};

#endif

