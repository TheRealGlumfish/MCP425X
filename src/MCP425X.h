/*
 * MCP425X.h - Simple library for controlling Microchip MCP4251/MCP4252 digital potentiometers over SPI
 *
 * Compatible with any Arduino boards with SPI support and available digital pins
 *
 * Copyright (c) 2023, Dimitrios Alexopoulos All rights reserved.
 */

#ifndef MCP425X_H
#define MCP425X_H

#include "Arduino.h"
#include <SPI.h>

typedef enum {P0, P1} potSelect_t;

class Microchip_MCP4251
{
public:
    uint16_t resistance;
    bool commandError = false;
    
    Microchip_MCP4251(uint8_t CSPin, uint16_t potResistance);
    Microchip_MCP4251(uint8_t CSPin, uint16_t potResistance, SPISettings SPIConf);
    void begin();
    bool incrementWiper(potSelect_t potSelect);
    bool incrementWiper(potSelect_t potSelect, uint16_t n);
    bool decrementWiper(potSelect_t potSelect);
    bool decrementWiper(potSelect_t potSelect, uint16_t n);
    uint16_t getWiper(potSelect_t potSelect);
    bool setWiper(potSelect_t potSelect, uint16_t position);

private:
    SPISettings _SPIConf; // default settings using SPI Mode 0 (Mode 3 is also supported)
    uint8_t _CSPin;
};

#endif
