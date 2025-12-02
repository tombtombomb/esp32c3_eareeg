#include <Arduino.h>
//#include <freertos/FreeRTOS.h>
#include <ADS1299_Driver.hpp>

#define SPI_FREQUENCY 4000000
#define SPI_CS_PIN 3
#define SPI_MISO_PIN 0
#define SPI_MOSI_PIN 2
#define SPI_CLK_PIN 1
#define ADS1299_RESET_PIN 4
#define ADS1299_READY_PIN 5
// #define TEST_PIN 4
// #define SPI_DATA_ORDER MSBFIRST
#define SPI_MODE SPI_MODE1

// put function declarations here:
//  AD7689_Driver ADCDriver=AD7689_Driver();
ADS1299_Driver adcDriver=ADS1299_Driver(-1,ADS1299_RESET_PIN);
void setup() {
  Serial.begin(9600);
  adcDriver.setSpiSetting(SPI_FREQUENCY,SPI_DATA_ORDER,SPI_MODE);
  adcDriver.begin(SPI_CS_PIN,SPI_MOSI_PIN,SPI_MISO_PIN,SPI_CLK_PIN,SPI);
  adcDriver.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.print("Hello World\n"); // 这行代码被注释掉了，不会执行
  //  Serial.print(ADCDriver.readRawdata()); // 调用ADCDriver对象的readRawdata方法，读取原始数据，并通过串口打印
  //  Serial.print("\n"); // 打印一个换行符，用于格式化输出
  //  delay(200); // 延迟200毫秒，控制打印频率
  u8_t type=adcDriver.readDeviceType();
  /*if(type==ADS1299_DEVICE_8CH)
    Serial.print("ADS1299 8CH\n");
  else if(type==ADS1299_DEVICE_6CH)
    Serial.print("ADS1299 6CH\n");
  else if(type==ADS1299_DEVICE_4CH)
    Serial.print("ADS1299 4CH\n");
  else
    Serial.print("ADS1299 Device Type Error\n");*/
  Serial.print("ADS1299 Device Type: ");
  Serial.print(type);
  Serial.print("\n");
  delay(200);
}

