/*
 * Project pump-balancer
 * Description:
 * Author:
 * Date:
 */


#include "Adafruit_ADS1X15.h"
#include "Adafruit_MCP4725.h"

#define ADADAC_ADDR 0x62
#define ADC_ADDR 0x49

SYSTEM_THREAD(ENABLED);

Adafruit_ADS1115 adc(ADC_ADDR);
Adafruit_MCP4725 ada_dac;

float ADC_CH0;
String RECORD;

float _MULT = 0.1875F;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);

  if (!waitFor(Particle.connected, 60000)) {
    Log.warn("Could not connect to the cloud.");
  }

  Particle.syncTime();

  if (Time.isDST()) {
    Time.zone(-4);
  } else {
    Time.zone(-5);
  }

  Time.setFormat(TIME_FORMAT_ISO8601_FULL);

  // Set Up the ADC
  adc.setGain(GAIN_TWOTHIRDS);
  adc.begin();

  // Start the DAC
  ada_dac.begin(ADADAC_ADDR);

  // Set Up the 2 3.3V DAC Channels
  pinMode(DAC1, OUTPUT);
  pinMode(DAC2, OUTPUT);

  delay(3000);

  Serial.println("Starting Up");

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  uint32_t _D = 4095;


  //short _CH0 = adc.readADC_Differential_0_1();
  short _CH0 = adc.readADC_SingleEnded(0);

  ada_dac.setVoltage(_D, true);


  ADC_CH0 = (float)_CH0 * _MULT;

  RECORD = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);

  RECORD.concat(String::format(",%.0f, %.2f", _CH0, ADC_CH0));

  // test
  Serial.println(RECORD);

  scanI2C();

  delay(1000);

}

void scanI2C() {
  Serial.println("\tScanning for i2c devices...");

  byte i, e;
  int n_devices = 0;

  if (!Wire.isEnabled()) {
    Wire.begin();
  }

  for (i=1; i< 127; i++) {
    Wire.beginTransmission(i);
    e = Wire.endTransmission();

    if (e == 0) {
      Serial.print("\tI2C device found at 0x");

      if (i < 16) {
        Serial.print("0");
      }

      Serial.println(i, HEX);

      n_devices++;
    }
  }

  if (n_devices == 0) {
    Serial.println("\tFound 0 i2c devices.");
  }
}
