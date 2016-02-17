#ifndef _SEEED_BMP280_H_
#define _SEEED_BMP280_H_

#include <Arduino.h>
#include <Wire.h>

#define BMP280_ADDRESS   0x77

#define BMP280_REG_DIG_T1    0x88
#define BMP280_REG_DIG_T2    0x8A
#define BMP280_REG_DIG_T3    0x8C

#define BMP280_REG_DIG_P1    0x8E
#define BMP280_REG_DIG_P2    0x90
#define BMP280_REG_DIG_P3    0x92
#define BMP280_REG_DIG_P4    0x94
#define BMP280_REG_DIG_P5    0x96
#define BMP280_REG_DIG_P6    0x98
#define BMP280_REG_DIG_P7    0x9A
#define BMP280_REG_DIG_P8    0x9C
#define BMP280_REG_DIG_P9    0x9E

#define BMP280_REG_CHIPID          0xD0
#define BMP280_REG_VERSION         0xD1
#define BMP280_REG_SOFTRESET       0xE0

#define BMP280_REG_CAL26           0xE1

#define BMP280_REG_CONTROL         0xF4
#define BMP280_REG_CONFIG          0xF5
#define BMP280_REG_PRESSUREDATA    0xF7
#define BMP280_REG_TEMPDATA        0xFA


class BMP280
{
public:
  void init(void);
  long PressureCompensate;
  float bmp280GetTemperature(unsigned short ut);
  long bmp280GetPressure(unsigned long up);
  float calcAltitude(float pressure);
  unsigned short bmp280ReadUT(void);
  unsigned long bmp280ReadUP(void);

private:
  // Calibratino data
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;

  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;

  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;

  uint8_t bmp280Read(byte address);
  uint16_t bmp280ReadInt(unsigned char address);
  int16_t bmp280ReadInt(unsigned char address);
  void writeRegister(byte deviceAddress, byte address, byte val);
  int16_t readRegister(byte deviceAddress, byte address);
};

#endif
