# MCP425X
Arduino library for controlling the Microchip MCP4251/MCP4252 digital potentiometers.

## Compatibility
Currently compatible with MCP4251, MCP4252,MCP4231, MCP4232 digital potentiometers, but support is planned for further models.  
Tested with the following Arduino boards, but it should be compatible with any boards that can support SPI:
- Arduino Uno
- Adafruit Metro M0 Expressto

## Installation
To install open Arduino IDE -> Sketch -> Include Library -> Add .ZIP Library... and select the source code (.zip) file.
![Screenshot from 2023-06-21 23-50-15](https://github.com/TheRealGlumfish/MCP425X/assets/65093316/7a4e6952-93fb-4726-a5bc-74aa176416c5)

## Usage
To use, simply include `MCP425X.h` in your sketch.

### Initialization

Each potentiometer connected to the Arduino is an instance of the `Microchip_MCP4251` class.
https://github.com/TheRealGlumfish/MCP425X/blob/4e0578f4e3592cd4060544231960d6df337a7487/src/MCP425X.h#L26-L27
The constructor of the class requires the resistance of the potentiometer, as well as the pin for the chip select line, to be passed as arguments.
Optionally, the default SPI settings (10MHz and Mode 0 as recommended by Microchip) can be overridden.
In the `setup()` section of the sketch, `begin()` needs to be called to initialize the CS pins and the SPI bus.
This needs to be called before any other operations are run on the potentiometer.
Below is an example initialization:
```cpp
Microchip_MCP4251 digiPot(5, 10000);

void setup()
{
    digiPot.begin();
}
```

### Operations
This library supports all the operations that are possible with the digital potentiometers, with the exception of shutdown.
This can still be achieved by manually toggling the shutdown pin of the potentiometer using `digitalWrite()`.
All commands will set `commandError` to true if they fail.
Any subsequent successful commands will set it back to false.
To select which potentiometer an operation is for, either `P0` or `P1` needs to be passed in the `potSelect` argument.

#### Incrementing/Decrementing the wiper
To increment/decrement the wiper, the following functions are provided.
https://github.com/TheRealGlumfish/MCP425X/blob/4e0578f4e3592cd4060544231960d6df337a7487/src/MCP425X.h#L31-L35
It is the responsibility of the user to know if the wiper has reached the max/min position, as further increment/decrement commands will have no effect on the wipers position.

#### Setting/Getting the wiper
The wiper position is stored in volatile memory in the IC.
Therefore, it can be set or retrieved directly using the `setWiper()` and `getWiper()` functions.
https://github.com/TheRealGlumfish/MCP425X/blob/4e0578f4e3592cd4060544231960d6df337a7487/src/MCP425X.h#L37-L38
One must take care to not exceed 9-bit values (or `0x100`) when setting the wiper, as it's the maximum value the 8-bit digital potentiometers use, respectively `0x80` is the max for 7-bit models.

#### Setting/Getting the resistance
Instead of setting the wiper position directly as an unsigned integer between `0x00` and `0x100`, the resistance between terminal B and the wiper can be set and retrieved using the `setResistance()` and`getResistance()` functions.
https://github.com/TheRealGlumfish/MCP425X/blob/4e0578f4e3592cd4060544231960d6df337a7487/src/MCP425X.h#L40-L41

#### Getting the status
The `getStatus` function can be used to get the status of the potentiometer (if in shutdown or normal state).
https://github.com/TheRealGlumfish/MCP425X/blob/4e0578f4e3592cd4060544231960d6df337a7487/src/MCP425X.h#L43

### Deinitialization
After use of the potentiometers have finished, `SPI.end()` can be safely called to disable the SPI bus.
An additional call to `begin()` can re-establish normal functionality.
