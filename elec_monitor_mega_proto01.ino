/*
 * Copyright (c) 2018. All rights reserved. Nico Coetzee <nicc777@gmail.com>
 * 
 * Lisense: MIT - See bottom of file for full license.
 * 
 * 
 * Platform:
 * --------
 * 
 *    Arduino UNO rev 3
 *    
 *      Supplier Ref  : http://www.communica.co.za/Catalog/Details/P1424521842
 *      Cost          : on 2018-12-14 ZAR305
 * 
 * 
 * Electricity Monitor:
 * -------------------
 * 
 *    Part Acquiriung Details
 *    
 *      Supplier Ref  : http://www.communica.co.za/Catalog/Details/P3308369411
 *      Cost          : on 2018-12-14 ZAR150.01
 *      
 *    Code examples used from https://learn.openenergymonitor.org/
 *    
 *    Dependency: 
 *      
 *      EmonLib - https://github.com/openenergymonitor/EmonLib
 * 
 * 
 * CMU Temparature Probe DS18B20:
 * -----------------------------
 * 
 *    Part Acquiriung Details
 *    
 *      Supplier Ref  : http://www.communica.co.za/Catalog/Details/P3958148566
 *      Cost          : on 2018-12-14 ZAR60.01
 * 
 *    Code examples used from https://www.tweaking4all.com/hardware/arduino/arduino-ds18b20-temperature-sensor/
 * 
 *    Dependency: 
 *    
 *      OneWire - http://www.pjrc.com/teensy/td_libs_OneWire.html
 *  
 *      The DallasTemperature library can do all this work for you!
 *      http://milesburton.com/Dallas_Temperature_Control_Library
 *      
 * 
 * SME Stackable SD Card Shield:
 * ----------------------------
 * 
 *  Part Acquiriung Details
 *  
 *    Supplier Ref  : http://www.communica.co.za/Catalog/Details/P2363846356
 *    Cost          : ZAR95.83
 *    
 *  Test Setup (from the standard code example library ~ orks with this shield):
 *  
 *    SD card attached to SPI bus as follows:
 *        MOSI  - pin 11 on Arduino Uno/Duemilanove/Diecimila
 *        MISO  - pin 12 on Arduino Uno/Duemilanove/Diecimila
 *        CLK   - pin 13 on Arduino Uno/Duemilanove/Diecimila
 *        CS    - depends on your SD card shield or module.
 *      Pin 4 used here for consistency with other Arduino examples
 * 
 * 
 * Sundries:
 * --------
 * 
 *  - USB Cable for the Arduino (aka Printer Cable) (http://www.communica.co.za/Catalog/Details/P1837703657 ZAR25.70)
 *  - 4.7k Resistor (http://www.communica.co.za/Catalog/Details/P3423848296 ZAR0.09)
 *  - 3x jumper cable wires (connecting the temperature probe and resistor to the Arduino (http://www.communica.co.za/Catalog/Details/P0907169794 ZAR39.00 for 40)
 *  - Breakaway headers (x3) for the jumper wires (http://www.communica.co.za/Catalog/Details/P3333231207 ZAR6.07 for 40)
 *  - Tiny piece of PC board for connecting the temperature probe wires, the resistor and the headers (3x3 holes) (http://www.communica.co.za/Catalog/Details/P3913707199 ZAR59.46 for 100x300mm)
 *  
 *  Total Cost (guestimate): around ZAR50.00
 * 
 * ----------------------
 * 
 * Cost analysis (2018-12-14)
 * 
 * Cost of Components : ZAR610.85
 * Sundries           : ZAR50.00
 * 
 * TOTAL COST         : ZAR660.85
 *  
 */


#include "EmonLib.h"            // Required for the energy monitor shield
#include <OneWire.h>            // Required for the temprature probe (DS18B20)
#include <SPI.h>                // Required for the SD Card
#include <SD.h>                 // Required for the SD Card
#include <stdlib.h>             // Required for float to string conversion


EnergyMonitor emon1;            // Create an instance for the energy monitor
OneWire  ds(2);                 // Temperature probe on pin 2 (a 4.7K resistor is necessary)
bool useTempProbe = true;       // Set the initial state of the temperature probe to disabled
float tempC;
float * energyValues;
float lastKnownTemp = 0.0;
const int chipSelect = 4;       // SD Card
bool useSdFunctions = false;    // Flag to indicate if we can write to the SD card

void setup()
{
  // Setup serial interface
  Serial.begin(9600);

  // Initialize the energy monitor - uncalibrated !! (using demo code)
  emon1.voltage(2, 234.26, 1.7);    // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 111.1);          // Current: input pin, calibration.

  // Initialize the SD card
  if (SD.begin(chipSelect)) {
    useSdFunctions = true;
  }
}

float get_temp()
{

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( useTempProbe ) {

    if ( !ds.search(addr)) {
      ds.reset_search();
      delay(250);
      return 0.0;
    }

    if (OneWire::crc8(addr, 7) != addr[7]) {
      useTempProbe = false;
      return 0.0;
    }

    // the first ROM byte indicates which chip
    switch (addr[0]) {
      case 0x10:
        type_s = 1;
        break;
      case 0x28:
        type_s = 0;
        break;
      case 0x22:
        type_s = 0;
        break;
      default:
        useTempProbe = false;
        return 0.0;
    }

  }

  if ( useTempProbe ) {
    ds.reset();
    ds.select(addr);
    ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end
    delay(1000);     // maybe 750ms is enough, maybe not
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad
    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
    }
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } else {
      byte cfg = (data[4] & 0x60);
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    }
    celsius = (float)raw / 16.0;
    return celsius;    
  }
  
  
}

float * get_energy_readings()
{
  static float values[5];
  emon1.calcVI(20,2000);         // Calculate all. No.of wavelengths, time-out
  values[0] = emon1.realPower;
  values[1] = emon1.apparentPower; 
  values[2] = emon1.powerFactor;
  values[3] = emon1.Vrms;  
  values[4] = emon1.Irms;
  return values;
}

void log_data(float t, float e1, float e2, float e3, float e4, float e5 )
{
  String logMsg = "";
  char buff[10];
  dtostrf(t, 4, 6, buff);  //4 is mininum width, 6 is precision
  logMsg += buff;
  logMsg += ", ";
  dtostrf(e1, 4, 6, buff);  //4 is mininum width, 6 is precision
  logMsg += buff;
  logMsg += ", ";
  dtostrf(e2, 4, 6, buff);  //4 is mininum width, 6 is precision
  logMsg += buff;
  logMsg += ", ";
  dtostrf(e3, 4, 6, buff);  //4 is mininum width, 6 is precision
  logMsg += buff;
  logMsg += ", ";
  dtostrf(e4, 4, 6, buff);  //4 is mininum width, 6 is precision
  logMsg += buff;
  logMsg += ", ";
  dtostrf(e5, 4, 6, buff);  //4 is mininum width, 6 is precision
  logMsg += buff;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(logMsg);
    dataFile.close();
    // print to the serial port too:
    Serial.println(logMsg);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
} 

void loop()
{
  tempC = get_temp();
  if (tempC > 0.0 )
  {
    lastKnownTemp = tempC;
  }
  energyValues = get_energy_readings();
  log_data(lastKnownTemp, *(energyValues+0), *(energyValues+1), *(energyValues+2), *(energyValues+3), *(energyValues+4));
  delay(1000);
}


/* 12345678901234567890123456789012345678901234567890123456789012345678901234567890
 * LICENSE
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
