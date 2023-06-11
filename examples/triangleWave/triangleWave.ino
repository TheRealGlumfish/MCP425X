/*
 * triangleWave.ino - Generates a triangle wave at the wiper of the potentiometer
 * 
 * Connect terminal A of P0 to VCC and B to GND, W is the output
 *
 * Copyright (c) 2023, Dimitrios Alexopoulos All rights reserved.
 */

#include <MCP425X.h>

Microchip_MCP4251 testPot(8, 5000); // initialize digital potentiometer with pin 8 as CS

void setup() 
{
    testPot.begin();
}

void loop()
{
    testPot.incrementWiper(P0, 120);
    testPot.decrementWiper(P0, 120);
    testPot.decrementWiper(P0, 120);
    testPot.incrementWiper(P0, 120);
}
