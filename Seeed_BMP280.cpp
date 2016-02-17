#include "Seeed_BMP280.h"

void BMP280::init(void)
{
    Wire.begin();
    ac1 = bmp280ReadInt(0xAA);
    ac2 = bmp280ReadInt(0xAC);
    ac3 = bmp280ReadInt(0xAE);
    ac4 = bmp280ReadInt(0xB0);
    ac5 = bmp280ReadInt(0xB2);
    ac6 = bmp280ReadInt(0xB4);
    b1 = bmp280ReadInt(0xB6);
    b2 = bmp280ReadInt(0xB8);
    mb = bmp280ReadInt(0xBA);
    mc = bmp280ReadInt(0xBC);
    md = bmp280ReadInt(0xBE);    
}

// Read 1 byte from the bmp280 at 'address'
// Return: the read byte;
char BMP280::bmp280Read(unsigned char address)
{
    //Wire.begin();
    unsigned char data;
    Wire.beginTransmission(bmp280_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(bmp280_ADDRESS, 1);
    while(!Wire.available());
    return Wire.read();
}

// Read 2 bytes from the bmp280
// First byte will be from 'address'
// Second byte will be from 'address'+1
short BMP280::bmp280ReadInt(unsigned char address)
{
    unsigned char msb, lsb;
    Wire.beginTransmission(bmp280_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(bmp280_ADDRESS, 2);
    while(Wire.available()<2);
    msb = Wire.read();
    lsb = Wire.read();
    return (short) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned short BMP280::bmp280ReadUT()
{
    unsigned short ut;
    Wire.beginTransmission(bmp280_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x2E);
    Wire.endTransmission();
    delay(5);
    ut = bmp280ReadInt(0xF6);
    return ut;
}
// Read the uncompensated pressure value
unsigned long BMP280::bmp280ReadUP()
{
    unsigned char msb, lsb, xlsb;
    unsigned long up = 0;
    Wire.beginTransmission(bmp280_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x34 + (OSS<<6));
    Wire.endTransmission();
    delay(2 + (3<<OSS));

    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    msb = bmp280Read(0xF6);
    lsb = bmp280Read(0xF7);
    xlsb = bmp280Read(0xF8);
    up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
    return up;
}

void BMP280::writeRegister(short deviceAddress, byte address, byte val)
{
    Wire.beginTransmission(deviceAddress); // start transmission to device
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

short BMP280::readRegister(short deviceAddress, byte address)
{
    short v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();

    Wire.requestFrom(deviceAddress, 1); // read a byte

    while(!Wire.available()) {
        // waiting
    }

    v = Wire.read();
    return v;
}

float BMP280::calcAltitude(float pressure)
{
    float A = pressure/101325;
    float B = 1/5.25588;
    float C = pow(A,B);
    C = 1 - C;
    C = C /0.0000225577;
    return C;
}

float BMP280::bmp280GetTemperature(unsigned short ut)
{
    long x1, x2;

    x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
    x2 = ((long)mc << 11)/(x1 + md);
    PressureCompensate = x1 + x2;

    float temp = ((PressureCompensate + 8)>>4);
    temp = temp /10;

    return temp;
}

long BMP280::bmp280GetPressure(unsigned long up)
{
    long x1, x2, x3, b3, b6, p;
    unsigned long b4, b7;
    b6 = PressureCompensate - 4000;
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

    b7 = ((unsigned long)(up - b3) * (50000>>OSS));
    if (b7 < 0x80000000)
    p = (b7<<1)/b4;
    else
    p = (b7/b4)<<1;

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;

    long temp = p;
    return temp;
}
