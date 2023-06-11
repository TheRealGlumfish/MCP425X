
/*
 * MCP425X.cpp - Simple library for controlling Microchip MCP4251/MCP4252 digital potentiometers over SPI
 *
 * Copyright (c) 2023, Dimitrios Alexopoulos All rights reserved.
 */

#include "Arduino.h"
#include <SPI.h>

#include "MCP425X.h"

Microchip_MCP4251::Microchip_MCP4251(uint8_t CSPin, uint16_t potResistance)
{
    _SPIConf = SPISettings(10000000, MSBFIRST, SPI_MODE0);
    _CSPin = CSPin;
    resistance = potResistance;
}

Microchip_MCP4251::Microchip_MCP4251(uint8_t CSPin, uint16_t potResistance, SPISettings SPIConf)
{
    _SPIConf = SPIConf;
    _CSPin = CSPin;
    resistance = potResistance;
}

// initializes SPI bus and de-asserts CS pin
void Microchip_MCP4251::begin()
{
    pinMode(_CSPin, OUTPUT);
    digitalWrite(_CSPin, HIGH);
    SPI.begin();
}

// increments the digital potentiometer wiper and returns if there is an error
bool Microchip_MCP4251::incrementWiper(bool potSelect)
{
    bool error = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if(SPI.transfer(0b00010100) < 0b11111111)
        {
            error = true;
        }
    }
    else
    {
        if(SPI.transfer(0b00000100) < 0b11111111)
        {
            error = true;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH);
    return error;
}

// increments the digital potentiometer wiper n times and returns if there is an error
bool Microchip_MCP4251::incrementWiper(bool potSelect, uint16_t n)
{
    bool error = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    for(int i = 0; i < n; i++)
    {
        if(potSelect)
        {
            if(SPI.transfer(0b00010100) < 0b11111111)
            {
                error = true;
            }
        }
        else
        {
            if(SPI.transfer(0b00000100) < 0b11111111)
            {
                error = true;
            }
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH);
    return error;
}

// decrements the digital potentiometer wiper and returns if there is an error
bool Microchip_MCP4251::decrementWiper(bool potSelect)
{
    bool error = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if(SPI.transfer(0b00011000) < 0b11111111)
        {
            error = true;
        }
    }
    else
    {
        if(SPI.transfer(0b00001000) < 0b11111111)
        {
            error = true;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
    return error;
}

// decrements the digital potentiometer wiper n times and returns if there is an error
bool Microchip_MCP4251::decrementWiper(bool potSelect, uint16_t n)
{
    bool error = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    for(int i = 0; i < n; i++)
    {
        if(potSelect)
        {
            if(SPI.transfer(0b00011000) < 0b11111111)
            {
                error = true;
            }
        }
        else
        {
            if(SPI.transfer(0b00001000) < 0b11111111)
            {
                error = true;
            }
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
    return error;
}

// reads the digital potentiometer wiper position and returns the value or 0xFFFF if there is an error
uint16_t Microchip_MCP4251::getWiper(bool potSelect)
{
    uint16_t readData;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if((readData = SPI.transfer16(0b0001110000000000)) < 0b1111111000000000)
        {
            readData = 0xFFFF;
        }
        else
        {
            readData -= 0b1111111000000000;
        }
    }
    else
    {
        if((readData = SPI.transfer16(0b0000110000000000)) < 0b1111111000000000)
        {
            readData = 0xFFFF;
        }
        else
        {
            readData -= 0b1111111000000000;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
    return readData;
}

// sets the position of the wiper and returns if there is an error
bool Microchip_MCP4251::setWiper(bool potSelect, uint16_t position)
{
    bool error = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if(SPI.transfer16(0b0001000000000000 | position) < 0b1111111111111111)
        {
            error = true;
        }
    }
    else
    {
        if(SPI.transfer16(0b0000000000000000 | position) < 0b1111111111111111)
        {
            error = true;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
    return error;
}

