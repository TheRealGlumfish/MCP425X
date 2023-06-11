
/*
 * MCP425X.cpp - Simple library for controlling Microchip MCP4251/MCP4252 digital potentiometers over SPI
 *
 * Copyright (c) 2023, Dimitrios Alexopoulos All rights reserved.
 */

#include "Arduino.h"
#include <SPI.h>

#include "MCP425X.h"

Microchip_MCP4251::Microchip_MCP4251(uint8_t CSPin, uint16_t resistance)
{
    _SPIConf = SPISettings(10000000, MSBFIRST, SPI_MODE0);
    _CSPin = CSPin;
    potResistance = resistance;
}

Microchip_MCP4251::Microchip_MCP4251(uint8_t CSPin, uint16_t resistance, SPISettings SPIConf)
{
    _SPIConf = SPIConf;
    _CSPin = CSPin;
    potResistance = resistance;
}

// initializes SPI bus and de-asserts CS pin
void Microchip_MCP4251::begin()
{
    pinMode(_CSPin, OUTPUT);
    digitalWrite(_CSPin, HIGH);
    SPI.begin();
}

// increments the digital potentiometer wiper
void Microchip_MCP4251::incrementWiper(potSelect_t potSelect)
{
    commandError = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if(SPI.transfer(0b00010100) < 0b11111111)
        {
            commandError = true;
        }
    }
    else
    {
        if(SPI.transfer(0b00000100) < 0b11111111)
        {
            commandError = true;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH);
}

// increments the digital potentiometer wiper n times
void Microchip_MCP4251::incrementWiper(potSelect_t potSelect, uint16_t n)
{
    commandError = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    for(int i = 0; i < n; i++)
    {
        if(potSelect)
        {
            if(SPI.transfer(0b00010100) < 0b11111111)
            {
                commandError = true;
            }
        }
        else
        {
            if(SPI.transfer(0b00000100) < 0b11111111)
            {
                commandError = true;
            }
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH);
}

// decrements the digital potentiometer wiper
void Microchip_MCP4251::decrementWiper(potSelect_t potSelect)
{
    commandError = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if(SPI.transfer(0b00011000) < 0b11111111)
        {
            commandError = true;
        }
    }
    else
    {
        if(SPI.transfer(0b00001000) < 0b11111111)
        {
            commandError = true;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
}

// decrements the digital potentiometer wiper n times
void Microchip_MCP4251::decrementWiper(potSelect_t potSelect, uint16_t n)
{
    commandError = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    for(int i = 0; i < n; i++)
    {
        if(potSelect)
        {
            if(SPI.transfer(0b00011000) < 0b11111111)
            {
                commandError = true;
            }
        }
        else
        {
            if(SPI.transfer(0b00001000) < 0b11111111)
            {
                commandError = true;
            }
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
}

// reads the digital potentiometer wiper position and returns the value or 0xFFFF if there is an error
uint16_t Microchip_MCP4251::getWiper(potSelect_t potSelect)
{
    commandError = false;
    uint16_t readData;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if((readData = SPI.transfer16(0b0001110000000000)) < 0b1111111000000000)
        {
            commandError = true;
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
            commandError = true;
            readData = 0xFFFF;
        }
        else
        {
            readData -= 0b1111111000000000;
            readData = 0xFFFF;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
    return readData;
}

// sets the position of the wiper and returns if there is an error
void Microchip_MCP4251::setWiper(potSelect_t potSelect, uint16_t position)
{
    commandError = false;
    digitalWrite(_CSPin, LOW); // asserts the CS pin (active low)
    SPI.beginTransaction(_SPIConf);
    if(potSelect)
    {
        if(SPI.transfer16(0b0001000000000000 | position) < 0b1111111111111111)
        {
            commandError = true;
        }
    }
    else
    {
        if(SPI.transfer16(0b0000000000000000 | position) < 0b1111111111111111)
        {
            commandError = true;
        }
    }
    SPI.endTransaction();
    digitalWrite(_CSPin, HIGH); // de-asserts the CS pin after the transaction has finished
}

// gets the resistance between terminal B and W
uint16_t Microchip_MCP4251::getResistance(potSelect_t potSelect)
{
    return map(getWiper(potSelect), 0, 0, 0x100, potResistance);
}

// sets the resistance between terminal B and W
void Microchip_MCP4251::setResistance(potSelect_t potSelect, uint16_t resistance)
{
    setWiper(potSelect, map(resistance, 0, 0, potResistance, 0x100));
}

// reads the status registers and returns if the IC is in the shutdown state
bool Microchip_MCP4251::getStatus()
{
    commandError = false;
    uint16_t readData;
    SPI.beginTransaction(_SPIConf);
    if((readData = SPI.transfer16(0b0101110000000000)) < 0b1111111000000000)
    {
        commandError = true;
        return false;
    }
    readData = readData & 0b0000000000000010;
    return readData;
}

