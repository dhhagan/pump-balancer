/*
*/
#include "Particle.h"
#include "Adafruit_MCP4725.h"

Adafruit_MCP4725::Adafruit_MCP4725() {
}

void Adafruit_MCP4725::begin(uint8_t addr) {
  _i2caddr = addr;

  if (!Wire.isEnabled()) {
    Wire.begin();
  }
}

void Adafruit_MCP4725::setVoltage( uint16_t output, bool writeEEPROM ) {
  Wire.beginTransmission(_i2caddr);

  if (writeEEPROM) {
    Wire.write(MCP4726_CMD_WRITEDACEEPROM);
  } else {
    Wire.write(MCP4726_CMD_WRITEDAC);
  }

  Wire.write(output / 16);
  Wire.write((output % 16) << 4);

  Wire.endTransmission();
}
