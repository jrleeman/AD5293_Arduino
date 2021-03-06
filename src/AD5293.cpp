
#include "AD5293.h"
#include "Arduino.h"
#include "SPI.h"

AD5293::AD5293() {
}

void AD5293::begin(uint8_t num_devices, uint8_t cs_pin, uint16_t min_position, uint16_t max_position) {
    // Set up constants
    AD5293_CS_PIN = cs_pin;
    NUM_AD5293 = num_devices;
    AD5293_MAX_POSITION = max_position;
    AD5293_MIN_POSITION = min_position;

    // Configure the SPI interface
    SPI.begin();
    pinMode(AD5293_CS_PIN, OUTPUT);
    digitalWrite(AD5293_CS_PIN, HIGH);
}


void AD5293::setWiperPosition(uint8_t device_num, uint16_t position) {
    // If the position is out of range, clamp it
    if (position > AD5293_MAX_POSITION) {
        position = AD5293_MAX_POSITION;
    }
    if (position < AD5293_MIN_POSITION ) {
        position = AD5293_MIN_POSITION;
    }

    // Make the command and send it
    writeToDeviceNumber(device_num, CMD_SET_WIPER | position);
}


void AD5293::configure(uint8_t device_num, uint8_t mode, uint8_t write_protect) {
  writeToDeviceNumber(device_num, CMD_WRITE_CTRL | mode << 2 | write_protect << 1);
}


//
// Private methods - Users should not ever need these directly
//

void AD5293::writeToDeviceNumber(uint8_t device_num, uint16_t command) {
    uint16_t ret=0;
    digitalWrite(AD5293_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
    for (int i=NUM_AD5293-1; i>=0; i--) {
      if (i == device_num){
        ret = SPI.transfer16(command);
      }
      else{
        ret = SPI.transfer16(CMD_NOP);
      }
    }
    SPI.endTransaction();
    digitalWrite(AD5293_CS_PIN, HIGH);
}
