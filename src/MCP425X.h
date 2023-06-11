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

// TODO: Make use of initializer lists in the constructors in order to change the type of potResistance and SPISettings to const
// TODO: Change potResistance to type enum with all the commercially available resistance values

typedef enum {P0, P1} potSelect_t;

class Microchip_MCP4251
{
public:
    uint16_t potResistance; // potentiometer resistance, used for calculating resistance between W and B terminals
    bool commandError = false; // set to true if a command fails (there is a command error), gets reset between commands
    
    Microchip_MCP4251(uint8_t CSPin, uint16_t resistance);
    Microchip_MCP4251(uint8_t CSPin, uint16_t resistance, SPISettings SPIConf);
    
    void begin();
   
    void incrementWiper(potSelect_t potSelect);
    void incrementWiper(potSelect_t potSelect, uint16_t n);
   
    void decrementWiper(potSelect_t potSelect);
    void decrementWiper(potSelect_t potSelect, uint16_t n);
    
    uint16_t getWiper(potSelect_t potSelect);
    void setWiper(potSelect_t potSelect, uint16_t position);
   
    uint16_t getResistance(potSelect_t potSelect);
    void setResistance(potSelect_t potSelect, uint16_t resistance);

    bool getStatus();

private:
    SPISettings _SPIConf; // default settings using SPI Mode 0 (Mode 3 is also supported)
    uint8_t _CSPin; // Chip select pin
};

#endif
