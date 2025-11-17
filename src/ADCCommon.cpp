#include "ADCCommon.hpp"

void ADCCommon::setSpiSetting(uint32_t freq, uint8_t dataOrder, uint8_t dataMode)
{
    __freq = freq;
    __dataOrder = dataOrder;
    __dataMode = dataMode;
}

bool ADCCommon::begin(PLATFORM_WIRE_TYPE &w, uint8_t addr, int sda, int scl)
{
    if (__has_init)
        // return thisChip().initImpl();
    __wire = &w;
    __sda = sda;
    __scl = scl;
    __wire->begin(__sda, __scl);
    __addr = addr;
    __spi = NULL;
    // __has_init = thisChip().initImpl();
    return __has_init;
}

bool ADCCommon::begin(int cs, int mosi, int miso, int sck,PLATFORM_SPI_TYPE &spi)
{
    log_i("Using Arduino SPI interface.\n");
    // if (__has_init)
        // return thisChip().initImpl();
    __cs  = cs;
    __spi = &spi;
    pinMode(__cs, OUTPUT);
    digitalWrite(__cs, HIGH);
    __spiSetting = new  DEFAULT_SPISETTING;
    if (!__spiSetting) {
        return false;
    }
    if (mosi != -1 && miso != -1 && sck != -1) {
        __spi->begin(sck, miso, mosi);
    } else {
        __spi->begin();
    }
    __wire = NULL;
    // __readMask = thisChip().getReadMaskImpl();
    // __has_init = thisChip().initImpl();
    return __has_init;
}

int ADCCommon::writeCommand(uint8_t command)
{
    #if defined(ARDUINO)
        if (__wire) {
            __wire->beginTransmission(__addr);
            __wire->write(command);
            return (__wire->endTransmission() == 0) ? DEV_WIRE_NONE : DEV_WIRE_ERR;
        }
        if (__spi) {
            __spi->beginTransaction(*__spiSetting);
            digitalWrite(__cs, LOW);
            __spi->transfer(command);
            digitalWrite(__cs, HIGH);
            __spi->endTransaction();
            return DEV_WIRE_NONE;
        }
        return DEV_WIRE_ERR;
    #endif 
}

int ADCCommon::writeRegister(int reg, uint8_t *buf, uint8_t length)
{
    #if defined(ARDUINO)
        if (__wire) {
            __wire->beginTransmission(__addr);
            if (__reg_addr_len == 1) {
                __wire->write(reg);
            } else {
                for (int i = 0; i < __reg_addr_len; ++i) {
                    __wire->write(reg >> (8 * ((__reg_addr_len - 1) - i)));
                }
            }
            __wire->write(buf, length);
            return (__wire->endTransmission() == 0) ? DEV_WIRE_NONE : DEV_WIRE_ERR;
        }
        if (__spi) {
            __spi->beginTransaction(*__spiSetting);
            digitalWrite(__cs, LOW);
            if (__reg_addr_len == 1) {
                __spi->transfer(reg);
            } else {
                for (int i = 0; i < __reg_addr_len; ++i) {
                    __spi->transfer(reg >> (8 * ((__reg_addr_len - 1) - i)));
                }
            }
            __spi->transfer(buf, length);
            digitalWrite(__cs, HIGH);
            __spi->endTransaction();
            return DEV_WIRE_NONE;
        }
        return DEV_WIRE_ERR;
    #endif 
}

int ADCCommon::writeRegister(int reg, uint8_t val)
{
    return ADCCommon::writeRegister(reg, &val, 1);
}

int ADCCommon::writeRegister(int reg, RegData_t data)
{
    return ADCCommon::writeRegister(reg, (uint8_t *)&data.full, 2);
}

int ADCCommon::writeRegister(uint8_t reg, uint8_t norVal, uint8_t orVal)
{
    int val = ADCCommon::readRegister(reg);
    if (val == DEV_WIRE_ERR) {
        return DEV_WIRE_ERR;
    }
    val &= norVal;
    val |= orVal;
    return ADCCommon::writeRegister(reg, val);
}

//! Read method
int ADCCommon::readRegister(int reg)
{
    uint8_t val = 0;
    return readRegister(reg, &val, 1) == -1 ? -1 : val;
}

int ADCCommon::readRegister(int reg, RegData_t *data)
{
    return readRegister(reg, (uint8_t *)data, 2);
}

/*

*/
int ADCCommon::readRegister(int reg, uint8_t *buf, uint8_t length)
{
#if defined(ARDUINO)
    if (__wire) {
        __wire->beginTransmission(__addr);
        if (__reg_addr_len == 1) {
             __wire->write(reg);
        } else {
            for (int i = 0; i < __reg_addr_len; ++i) {
                __wire->write(reg >> (8 * ((__reg_addr_len - 1) - i)));
            }
        }
        if (__wire->endTransmission(__sendStop) != 0) {
            return DEV_WIRE_ERR;
        }
        __wire->requestFrom(__addr, length);
        return __wire->readBytes(buf, length) == length ? DEV_WIRE_NONE : DEV_WIRE_ERR;
    }
    if (__spi) {
        __spi->beginTransaction(*__spiSetting);
        digitalWrite(__cs, LOW);
        if (__reg_addr_len == 1) {
            __spi->transfer(__readMask != -1 ? (reg  | __readMask) : reg);
        } 
        else {
            uint8_t firstBytes = reg >> (8 * ((__reg_addr_len - 1)));
            __spi->transfer(__readMask != -1 ? (firstBytes  | __readMask) : firstBytes);
            for (int i = 1; i < __reg_addr_len; ++i) {
                __spi->transfer(reg >> (8 * ((__reg_addr_len - 1) - i)));
            }
        }
        for (size_t i = 0; i < length; i++) {
            buf[i] = __spi->transfer(0x00);
        }
        digitalWrite(__cs, HIGH);
        __spi->endTransaction();
        return DEV_WIRE_NONE;
    }
#endif
    return DEV_WIRE_ERR;
}