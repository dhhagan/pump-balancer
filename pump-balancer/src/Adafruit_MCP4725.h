/*
  Code for the MCP4725 12-bit DAC.
*/

#ifndef Adafruit_MCP4725_h
#define Adafruit_MCP4725_h

#include "application.h"

#define MCP4726_CMD_WRITEDAC            (0x40)  // Writes data to the DAC
#define MCP4726_CMD_WRITEDACEEPROM      (0x60)  // Writes data to the DAC and the EEPROM (persisting the assigned value after reset)

class Adafruit_MCP4725 {
  public:
    Adafruit_MCP4725();

    void begin(uint8_t addr);
    void setVoltage( uint16_t output, bool writeEEPROM );

  private:
    uint8_t _i2caddr;
};

#endif
