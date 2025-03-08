#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pt2258 {

class PT2258Component : public PollingComponent, public i2c::I2CDevice {

 public:
  PT2258Component() {}     // Constructor for the PT2258 component

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;    // Determines the order in which setup() methods are called on startup

  // Initialization of PT2258
  void initialize_chip();

  // PT2258 Command Constants
  // PT2258 Command Constants
  static const uint8_t CHANNEL1_VOLUME_STEP_01 = 0x90;
  static const uint8_t CHANNEL1_VOLUME_STEP_10 = 0x80;
  static const uint8_t CHANNEL2_VOLUME_STEP_01 = 0x50;
  static const uint8_t CHANNEL2_VOLUME_STEP_10 = 0x40;
  static const uint8_t CHANNEL3_VOLUME_STEP_01 = 0x10;
  static const uint8_t CHANNEL3_VOLUME_STEP_10 = 0x00;
  static const uint8_t CHANNEL4_VOLUME_STEP_01 = 0x30;
  static const uint8_t CHANNEL4_VOLUME_STEP_10 = 0x20;
  static const uint8_t CHANNEL5_VOLUME_STEP_01 = 0x70;
  static const uint8_t CHANNEL5_VOLUME_STEP_10 = 0x60;
  static const uint8_t CHANNEL6_VOLUME_STEP_01 = 0xb0;
  static const uint8_t CHANNEL6_VOLUME_STEP_10 = 0xa0;
  static const uint8_t MASTER_VOLUME_1STEP     = 0xe0;
  static const uint8_t MASTER_VOLUME_10STEP    = 0xd0;
  static const uint8_t MUTE                    = 0xF8;
  static const uint8_t SYSTEM_RESET            = 0xc0;


  // Public methods to control the PT2258 device
  void set_master_vol(int vol);
  void set_channel_vol(uint8_t chvol, uint8_t chno);
  void set_mute(uint8_t mute_state);
  


protected:
void write_byte(uint8_t data);    // Protected method to send a single byte to the PT2258 via I2C

private:
// Internal methods or variables can be added here if necessary
unsigned char HEX2BCD (unsigned char x);
};

} // namespace pt2258
} // namespace esphome