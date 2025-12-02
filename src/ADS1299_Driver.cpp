#include "ADS1299_Driver.hpp"

/**/
void ADS1299_Driver::init(void) {
    msdelay(200);// wait for power up, 2^18 clock cycles, then reset
    resetDevice();
    stopCoversion();
}
/**
 * @brief get the register
 * @return: STATUS value
 */
int  ADS1299_Driver::getRegister(u8_t reg, u8_t regNum,u8_t *regData) {
    if (__spi) {
        __spi->beginTransaction(*__spiSetting);
        digitalWrite(__cs, LOW);
        __spi->transfer(ADS1299_SDATAC_CMD);
        __spi->transfer(0x20|reg);  
        __spi->transfer(regNum-0x01);
        for(u8_t i=0;i<regNum;i++)
            regData[i] = __spi->transfer(0x00);
        digitalWrite(__cs, HIGH);
        __spi->endTransaction();
        return DEV_WIRE_NONE;
    }
    return DEV_WIRE_ERR;
}

/**
 * @brief set the register
 * @return: STATUS value
 */
int  ADS1299_Driver::setRegister(u8_t reg, u8_t regNum,u8_t *regData) {
    if (__spi) {
        __spi->beginTransaction(*__spiSetting);
        digitalWrite(__cs, LOW);
        __spi->transfer(ADS1299_SDATAC_CMD);
        __spi->transfer(0x40|reg);  
        __spi->transfer(regNum-0x01);
        for(u8_t i=0;i<regNum;i++)
            __spi->transfer(regData[i]);
        digitalWrite(__cs, HIGH);
        __spi->endTransaction();
        return DEV_WIRE_NONE;
    }
    return DEV_WIRE_ERR;
}

/**
 * @brief Read the ID register
 * @return: ID value
 */
u8_t ADS1299_Driver::readDeviceType(void) {
    u8_t chipID;
    u8_t result;
    if(mode==ADS1299_MODE_IDLE)
        u8_t result=getRegister(ADS1299_ID_REG,0x01,&chipID);
    if(result==DEV_WIRE_NONE)
        return chipID&0x1F;
    return result;
}

/**
 * @brief set Config1 register
 * @param cfg: configuration
 * @return: STATUS value
 */
int ADS1299_Driver::setConfig1(u8_t cfg) {
    return writeRegister(ADS1299_CONFIG1_REG,cfg); 
}

int ADS1299_Driver::setDatarate(u8_t rate) {
    u8_t cfg = readRegister(ADS1299_CONFIG1_REG);
    cfg &= 0xF8;
    cfg |= rate;
    return writeRegister(ADS1299_CONFIG1_REG,cfg);
}

/**
 * @brief set Config2 register
 * @param cfg: configuration
 * @return: STATUS value
 */
int ADS1299_Driver::setConfig2(u8_t cfg) {
    return writeRegister(ADS1299_CONFIG2_REG,cfg);
}

/**
 * @brief set Config3 register
 * @param cfg: configuration
 * @return: STATUS value
 */
int ADS1299_Driver::setConfig3(u8_t cfg) {
    return writeRegister(ADS1299_CONFIG3_REG,cfg);
}

/**
 * @brief Set LOFF register
 * @param cfg: configuration
 * @return: STATUS value
 */
int ADS1299_Driver::setLOFF(u8_t cfg) {
    return writeRegister(ADS1299_LOFF_REG,cfg);
}

/**
 * @brief Set CHn configuration
 * @param ch: channel number (1-8)
 * @param cfg: configuration (0-7)
 * @return: 0 if success, error code otherwise
 */
int ADS1299_Driver::setCHnConfig(u8_t ch, u8_t cfg) {
    if((ch > 8)||(ch < 1))
        return ADS1299_DRIVER_MISSCHn;
    u8_t reg = ADS1299_CH1SET_REG + ch-1;
    return writeRegister(reg,cfg);
}

/**
 * @brief Set CHn gain
 * @param ch: channel number (1-8)
 * @param gain: gain (1-8)
 * @return: 0 if success, error code otherwise
 */
int ADS1299_Driver::setCHnGain(u8_t ch, u8_t gain) {
    if((ch > 8)||(ch < 1))
        return ADS1299_DRIVER_MISSCHn;
    u8_t reg = ADS1299_CH1SET_REG + ch-1;
    u8_t cfg = readRegister(reg);
    cfg &= 0x8F;
    cfg |= gain;
    return writeRegister(reg,cfg);
}

/**
 * @brief Set CHn input configuration
 * @param ch: channel number (1-8)
 * @param cfg: configuration (0-7)
 * @return: 0 if success, error code otherwise
 */
int ADS1299_Driver::setCHnInput(u8_t ch, u8_t cfg) {
    if((ch > 8)||(ch < 1))
        return ADS1299_DRIVER_MISSCHn;
    u8_t reg = ADS1299_CH1SET_REG + ch-1;
    u8_t val = readRegister(reg);
    val &= 0xF8;
    val |= cfg;
    return writeRegister(reg,val);
}

/**
 * @brief Set CHn SRB2 configuration
 * @param ch: channel number (1-8)
 * @param cfg: configuration (0-7)
 * @return: 0 if success, error code otherwise
*/
int ADS1299_Driver::setCHnSRB2(u8_t ch, u8_t cfg) {
    if((ch > 8)||(ch < 1))
        return ADS1299_DRIVER_MISSCHn;
    u8_t reg = ADS1299_CH1SET_REG + ch-1;
    u8_t val = readRegister(reg);
    val &= 0xF7;
    val |= cfg;
    return writeRegister(reg,val);
}

/**
 * @brief Set start pin
 * @param pin: pin number (NULL if start by command)
 * @return: start mode
 */
int ADS1299_Driver::setStartPin(u8_t pin){
    startPin = pin;
    return startPin;
}

/**
 * @brief Set reset mode
 * @param pin: pin number (NULL if reset by command)
 * @return: reset mode
 */
int ADS1299_Driver::setResetPin(u8_t pin){
    resetPin = pin;
    return  resetPin;
}

/**
 * @brief Start conversion
 * @return: STATUS value
 */
int ADS1299_Driver::startCoversion(void) {  
    if(startPin==-1){
        int result=writeCommand(ADS1299_START_CMD);
        if(result==DEV_WIRE_NONE)
            mode=ADS1299_MODE_CONTINUOUS;
        else
            return result;
    }
    else{
        pinMode(startPin,OUTPUT);
        digitalWrite(startPin,HIGH);
        mode=ADS1299_MODE_CONTINUOUS;
        return ADS1299_DRIVER_OK;
    }    
}

/**
 * @brief Stop conversion
 *  @return: STATUS value
 */
int ADS1299_Driver::stopCoversion(void) {
    if(startPin==-1){
        int result=writeCommand(ADS1299_SDATAC_CMD);
        if(result==DEV_WIRE_NONE)
            mode=ADS1299_MODE_IDLE;
        else
            return result;
    }
    else{
        pinMode(startPin,OUTPUT);
        digitalWrite(startPin,LOW);
        mode=ADS1299_MODE_IDLE;
        return ADS1299_DRIVER_OK;
    }
}

int ADS1299_Driver::resetDevice(void) {
    if(resetPin==-1){
        return writeCommand(ADS1299_RESET_CMD);
        msdelay(10);
    }
    else{
        pinMode(resetPin,OUTPUT);
        digitalWrite(resetPin,LOW);
        msdelay(10);
        digitalWrite(resetPin,HIGH);
        msdelay(10);
        return ADS1299_DRIVER_OK;
    } 
}