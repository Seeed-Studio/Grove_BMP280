/*
    bmp280_example.ino
    Example sketch for BMP280

    Copyright (c) 2016 seeed technology inc.
    Website    : www.seeedstudio.com
    Author     : Lambor, CHN
    Create Time:
    Change Log :  2021/03/11 by @Kongduino
                  Reworked example to display true altitude based on MSL

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "Seeed_BMP280.h"
#include <Wire.h>

BMP280 bmp280;
float MSL = 102009; // Mean Sea Level in Pa

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n\nBMP280 test");
  if (!bmp280.init()) {
    Serial.println("Device not connected or broken!");
  }
}

void loop() {
  float pressure = bmp280.getPressure();
  //get and print temperatures
  Serial.print("Temp: ");
  Serial.print(bmp280.getTemperature());
  Serial.println(" *C");
  // Using "*C" as the unit for Celsius because the Arduino IDE doesn't support special symbols

  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  Serial.print(pressure / 100.0);
  Serial.println(" HPa");

  //get and print altitude data
  // Pass MSL to the function
  Serial.print("Altitude: ");
  Serial.print(bmp280.calcAltitude(MSL));
  Serial.println(" m");

  Serial.println("\n");
  //add a line between output of different times.
  delay(1000);
}
