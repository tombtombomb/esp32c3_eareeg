#ifndef __ADS1299_DRIVER_H
#define __ADS1299_DRIVER_H

#include "ADCCommon.hpp"

#define DATA_PACKAGE_LEN 27 // Status Register + 8-Channel Data (216 Bits)

/* Register addresses */
#define ADS1299_ID_REG                   (0x00)
#define ADS1299_CONFIG1_REG              (0x01)
#define ADS1299_CONFIG2_REG              (0x02)
#define ADS1299_CONFIG3_REG              (0x03)
#define ADS1299_LOFF_REG                 (0x04)
#define ADS1299_CH1SET_REG               (0x05)
#define ADS1299_CH2SET_REG               (0x06)
#define ADS1299_CH3SET_REG               (0x07)
#define ADS1299_CH4SET_REG               (0x08)
#define ADS1299_CH5SET_REG               (0x09)
#define ADS1299_CH6SET_REG               (0x0A)
#define ADS1299_CH7SET_REG               (0x0B)
#define ADS1299_CH8SET_REG               (0x0C)
#define ADS1299_BIAS_SENSP_REG           (0x0D)
#define ADS1299_BIAS_SENSN_REG           (0x0E)
#define ADS1299_LOFF_SENSP_REG           (0x0F)
#define ADS1299_LOFF_SENSN_REG           (0x10)
#define ADS1299_LOFF_FLIP_REG            (0x11)
#define ADS1299_LOFF_STATP_REG           (0x12)
#define ADS1299_LOFF_STATN_REG           (0x13)
#define ADS1299_GPIO_REG                 (0x14)
#define ADS1299_MISC1_REG                (0x15)
#define ADS1299_MISC2_REG                (0x16)
#define ADS1299_CONFIG4_REG              (0x17)

/* Register settings */
#define ADS1299_CONFIG1_SETUP_250        (0x06)    // 250 SPS
#define ADS1299_CONFIG1_SETUP_500        (0x05)    // 500 SPS
#define ADS1299_CONFIG1_SETUP_1000       (0x04)    // 1 kSPS
#define ADS1299_CONFIG1_SETUP_2000       (0x03)    // 2 kSPS
#define ADS1299_CONFIG1_SETUP_4000       (0x02)    // 4 kSPS
#define ADS1299_CONFIG1_SETUP_8000       (0x01)    // 8 kSPS
#define ADS1299_CONFIG1_SETUP_16000      (0x00)    // 16 kSPS
#define ADS1299_CONFIG2_SETUP_TEST       (0xD0)    // Test signals are generated internally
#define ADS1299_CONFIG3_SETUP_REFBUF     (0xE0)    // Enable internal reference buffer
#define ADS1299_CONFIG3_SETUP_LOFF_STAT  (0x61)    // BIAS is not connected
#define ADS1299_CONFIG4_SETUP_LOFF_C_EN  (0x02)    // Continuous conversion mode / Lead-Off comp enabled
#define ADS1299_CONFIG4_SETUP_LOFF_S_EN  (0x0A)    // Single-shot mode / Lead-Off comp enabled
#define ADS1299_CONFIG4_SETUP_LOFF_S_DIS (0x08)    // Single-shot mode / Lead-Off comp disabled
#define ADS1299_LOFF_SETUP_TH_92         (0x20)    // Lead-off comp threshold (positive 92.5%)
#define ADS1299_LOFF_SETUP_TH_90         (0x40)    // Lead-off comp threshold (positive 90%)
#define ADS1299_LOFF_SETUP_TH_87         (0x60)    // Lead-off comp threshold (positive 87%)
#define ADS1299_LOFF_SETUP_TH_85         (0x80)    // Lead-off comp threshold (positive 85%)
#define ADS1299_LOFF_SETUP_TH_80         (0xA0)    // Lead-off comp threshold (positive 80%)
#define ADS1299_LOFF_SETUP_TH_75         (0xC0)    // Lead-off comp threshold (positive 75%)
#define ADS1299_LOFF_SETUP_TH_70         (0xE0)    // Lead-off comp threshold (positive 70%)
#define ADS1299_LOFF_SETUP_MAG_24N       (0x04)    // Lead-off curr magnitude 24 nA
#define ADS1299_LOFF_SETUP_MAG_6U        (0x08)    // Lead-off curr magnitude 6 uA
#define ADS1299_LOFF_SETUP_MAG_24U       (0x0C)    // Lead-off curr magnitude 24 uA
#define ADS1299_LOFF_SETUP_FREQ_7        (0x01)    // AC lead-off detection at 7.8 Hz (fCLK / 2^18)
#define ADS1299_LOFF_SETUP_FREQ_31       (0x02)    // AC lead-off detection at 31.2 Hz (fCLK / 2^16)
#define ADS1299_LOFF_SETUP_FREQ_DR       (0x03)    // AC lead-off detection at fDR / 4
#define ADS1299_CH_N_SET_SETUP_NO        (0x00)    // Normal operation
#define ADS1299_CH_N_SET_SETUP_PD        (0x80)    // Channel power-down
#define ADS1299_CH_N_SET_SETUP_GAIN_24   (0x60)    // PGA gain 24
#define ADS1299_CH_N_SET_SETUP_GAIN_12   (0x50)    // PGA gain 12
#define ADS1299_CH_N_SET_SETUP_GAIN_8    (0x40)    // PGA gain 8
#define ADS1299_CH_N_SET_SETUP_GAIN_6    (0x30)    // PGA gain 6
#define ADS1299_CH_N_SET_SETUP_GAIN_4    (0x20)    // PGA gain 4
#define ADS1299_CH_N_SET_SETUP_GAIN_2    (0x10)    // PGA gain 2
#define ADS1299_CH_N_SET_SETUP_GAIN_1    (0x00)    // PGA gain 1
#define ADS1299_CH_N_SET_SETUP_SRB2_OP   (0x00)    // SRB2 Open
#define ADS1299_CH_N_SET_SETUP_SRB2_CL   (0x08)    // SRB2 Closed
#define ADS1299_CH_N_SET_SETUP_MUX_NEI   (0x00)    // Normal electrode input
#define ADS1299_CH_N_SET_SETUP_MUX_IS    (0x01)    // Input shorted
#define ADS1299_CH_N_SET_SETUP_MUX_MEAS  (0x02)    // Used in conjunction with BIAS_MEAS bit
#define ADS1299_CH_N_SET_SETUP_MUX_MVDD  (0x03)    // MVDD for supply measurement
#define ADS1299_CH_N_SET_SETUP_MUX_TEMP  (0x04)    // Temperature sensor
#define ADS1299_CH_N_SET_SETUP_MUX_TEST  (0x05)    // Test signal
#define ADS1299_CH_N_SET_SETUP_MUX_DRP   (0x06)    // BIAS_DRP
#define ADS1299_CH_N_SET_SETUP_MUX_DRN   (0x07)    // BIAS_DRN
#define ADS1299_LOFF_SENSP_SETUP_IN_1_2  (0x03)    // Lead-off detection on IN1P and IN2P
#define ADS1299_LOFF_SENSN_SETUP_IN_1_2  (0x03)    // Lead-off detection on IN1N and IN2N


#define ADS1299_CONFIG1_SETUP_DEFAULT    (0x96)    // Daisy chain mode / Oscillator clock output disabled / 250 SPS
#define ADS1299_CONFIG2_SETUP_DEFAULT    (0xC0)    // Test signals are driven externally / Sig amp 1x / Sig freq 2^21
#define ADS1299_CONFIG3_SETUP_DEFAULT    (0x60)    // Power down ref buffer / BIAS Open / BIAS Power and sense down
#define ADS1299_CONFIG4_SETUP_DEFAULT    (0x00)    // Continuous conversion mode / LOFF comparators disabled
#define ADS1299_LOFF_SETUP_DEFAULT       (0x00)    // 95%/5% / 6nA current magnitude / DC lead-off detection
#define ADS1299_CH_N_SET_SETUP_DEFAULT   (0x61)    // Normal operation / PGA gain 24 / SRB2 Open / Channel input shorted
#define ADS1299_LOFF_SENSP_SETUP_DEFAULT (0x00)    // Lead-off detection disabled
#define ADS1299_LOFF_SENSN_SETUP_DEFAULT (0x00)    // Lead-off detection disabled

/* Commands */
#define ADS1299_WAKEUP_CMD               (0x02)
#define ADS1299_STANDBY_CMD              (0x04)
#define ADS1299_RESET_CMD                (0x06)
#define ADS1299_START_CMD                (0x08)
#define ADS1299_STOP_CMD                 (0x0A)
#define ADS1299_RDATAC_CMD               (0x10)
#define ADS1299_SDATAC_CMD               (0x11)
#define ADS1299_RDATA_CMD                (0x12)
#define ADS1299_READ_REG_CMD             (0x20)
#define ADS1299_WRITE_REG_CMD            (0x40)

/* Mode */
#define ADS1299_MODE_CONTINUOUS          (0x00)
#define ADS1299_MODE_SINGLE              (0x01)
#define ADS1299_MODE_IDLE                (0x02)
#define ADS1299_DEVICE_8CH               (0x1E)
#define ADS1299_DEVICE_6CH               (0x1D)
#define ADS1299_DEVICE_4CH               (0x1C)

#define ADS1299_DRIVER_OK                DEV_WIRE_NONE
#define ADS1299_DRIVER_ERROR             DEV_WIRE_ERR
#define ADS1299_DRIVER_TIMEOUT           DEV_WIRE_TIMEOUT
#define ADS1299_DRIVER_MISSCHn           -3


class ADS1299_Driver:public ADCCommon{

    public:
        ADS1299_Driver(void){
            startPin=-1;
            resetPin=-1;
            mode=ADS1299_MODE_IDLE;
        };
        ADS1299_Driver(int start, int reset){
            startPin=start;
            resetPin=reset;
            mode=ADS1299_MODE_IDLE;
        };
        void init(void);
        u8_t readDeviceType(void);
        int getRegister(u8_t reg, u8_t ,u8_t *regData);
        int setRegister(u8_t reg, u8_t val,u8_t *regData);
        int setConfig1(u8_t cfg);
        int setDatarate(u8_t rate);
        int setConfig2(u8_t cfg);
        int setConfig3(u8_t cfg);
        int setLOFF(u8_t cfg);
        int setCHnConfig(u8_t ch, u8_t cfg);
        int setCHnGain(u8_t ch, u8_t gain);
        int setCHnEnable(u8_t ch, bool enable);
        int setCHnInput(u8_t ch, u8_t cfg);
        int setCHnSRB2(u8_t ch, u8_t cfg);
        int setStartPin(u8_t pin);
        int setResetPin(u8_t pin);
        int startCoversion(void);
        int stopCoversion(void);
        int stopReadData(void); 
        int resetDevice(void);


    protected:
        // u8_t currentCFG;
        int startPin;
        int resetPin;
        u8_t mode;
};

#endif