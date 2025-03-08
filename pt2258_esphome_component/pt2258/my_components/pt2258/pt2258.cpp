#include "pt2258.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace pt2258
  {

    // TAG used for logging
    static const char *const TAG = "pt2258";

    // Setup function called when the component is initialized
    void PT2258Component::setup()
    {
      ESP_LOGCONFIG(TAG, "Setting up PT2258");
      this->initialize_chip();
      // Any initial setup can be performed here
    }

    // Update function called at a regular interval defined in the configuration
    void PT2258Component::update()
    {
      // Update logic goes here, if necessary
    }

    // Returns the setup priority of the component
    float PT2258Component::get_setup_priority() const
    {
      return setup_priority::DATA;
    }

    // Dumps the configuration of the component for debugging purposes
    void PT2258Component::dump_config()
    {
      ESP_LOGCONFIG(TAG, "PT2258:");
      LOG_I2C_DEVICE(this);
    }

    // Sets the input of the PT2258 device
    void PT2258Component::initialize_chip()
    {
      // Initialize device
      this->write_byte(SYSTEM_RESET);
      // Set channel volumes to 0dB
      for (int chno = 0; chno < 6; chno++)
      {
        // Calculate addresses based on channel number
        uint8_t address01 = 0x90 - chno * 0x20 + (chno / 2) * 0x10;
        uint8_t address10 = 0x80 - chno * 0x20 + (chno / 2) * 0x10;

        // Set volume for each step (01 and 10)
        this->write_byte(address01 | 0x00); // 0dB for 1 step
        this->write_byte(address10 | 0x00); // 0dB for 10 step
      }

      // Set the master volume to 0dB
      this->write_byte(MASTER_VOLUME_1STEP | 0x00);  // 0dB for 1 step
      this->write_byte(MASTER_VOLUME_10STEP | 0x00); // 0dB for 10 step

      // Mute off
      this->write_byte(MUTE);

      ESP_LOGD(TAG, "PT2258 initialization completed");
    }

    // helper method
    unsigned char PT2258Component::HEX2BCD(unsigned char x)
    {
      unsigned char y;
      y = (x / 10) << 4;
      y = y | (x % 10);
      return (y);
    }

    void PT2258Component::set_master_vol(int vol)
    {
      vol = vol - 78;
      vol = abs(vol);
      int vol_10 = vol / 10;
      int vol_1 = vol - vol_10 * 10;
      this->write_byte(vol_10 + 0b11100000);
      this->write_byte(vol_1 + 0b11010000);
    }

    // range : 0 to 15  (0dB to -15dB)
    void PT2258Component::set_channel_vol(uint8_t chvol, uint8_t chno)
    {
      // Define channel addresses directly in the function for clarity
      uint8_t channel_addresses_01[6] = {
          CHANNEL1_VOLUME_STEP_01, CHANNEL2_VOLUME_STEP_01, CHANNEL3_VOLUME_STEP_01,
          CHANNEL4_VOLUME_STEP_01, CHANNEL5_VOLUME_STEP_01, CHANNEL6_VOLUME_STEP_01};

      uint8_t channel_addresses_10[6] = {
          CHANNEL1_VOLUME_STEP_10, CHANNEL2_VOLUME_STEP_10, CHANNEL3_VOLUME_STEP_10,
          CHANNEL4_VOLUME_STEP_10, CHANNEL5_VOLUME_STEP_10, CHANNEL6_VOLUME_STEP_10};

      // Calculate the attenuation value from the desired volume level
      int attenuation = 79 - chvol;

      // Calculate the 10dB and 1dB components
      uint8_t vol_10db = attenuation / 10;
      uint8_t vol_1db = attenuation % 10;

      // Convert the components to BCD
      uint8_t bcd_10db = HEX2BCD(vol_10db);
      uint8_t bcd_1db = HEX2BCD(vol_1db);

      // Send the 10dB step command
      this->write_byte(channel_addresses_10[chno] | bcd_10db);

      // Send the 1dB step command
      this->write_byte(channel_addresses_01[chno] | bcd_1db);
    }

    // Set mute: 1 -> mute on, 0 -> mute off
    void PT2258Component::set_mute(uint8_t mute_state)
    {
      this->write_byte(MUTE + mute_state);
    }

    // Sends a byte of data to the PT2258 device over I2C
    void PT2258Component::write_byte(uint8_t data)
    {
      const uint8_t data_array[] = {data};
      if (this->write(data_array, 1) != i2c::ERROR_OK)
      {
        ESP_LOGE(TAG, "Failed to write to PT2258");
      }
    }

  } // namespace pt2258
} // namespace esphome