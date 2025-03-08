# PT2258 ESPHome Component

## Overview

This is an ESPHome custom component for interfacing with the **PT2258** digital potentiometer via **I2C**. The PT2258 is a 6-channel electronic volume controller, making it ideal for audio applications such as AV surround sound systems, car audio, and multimedia speakers.

## Features

- Supports **6 independent audio channels**
- Uses **I2C** communication for control
- **Adjustable volume** for each channel (0dB to -79dB, in 1dB steps)
- **Master volume control**
- **Mute functionality**
- Integration with **Home Assistant** via ESPHome

## Hardware Requirements

- **PT2258 Digital Potentiometer**
- **ESP32 Board**
- **I2C connections**:
  - **SDA:** GPIO 21
  - **SCL:** GPIO 22

## Installation

### 1. Clone the Repository

```sh
git clone https://github.com/hrch3k/pt2258_esphome_component.git
cd pt2258_esphome_component
```

### 2. Configure ESPHome

Modify your **ESPHome YAML** configuration to include the PT2258 component:

```yaml
esphome:
  name: pt2258-volume-control
  friendly_name: Audio Mixer  

esp32:
  board: esp32dev
  framework:
    type: arduino

logger:
  level: debug

api:
  encryption:
    key: "your_api_encryption_key"

ota:
  password: "your_ota_password"

wifi:
  ssid: "your_wifi_ssid"
  password: "your_wifi_password"

external_components:
  - source: my_components

i2c:
  sda: 21
  scl: 22
  scan: True
  id: pt2258_bus

pt2258:
  id: pt2258_audio
```

### 3. Flash the ESP

Once the ESPHome YAML is configured, compile and upload it to your ESP32:

```sh
esphome run pt2258.yaml
```

## API Methods

### Set Master Volume

```cpp
id(pt2258_audio).set_master_vol(volume_level);
```

- **volume\_level**: (0 to -79 dB)

### Set Individual Channel Volume

```cpp
id(pt2258_audio).set_channel_vol(volume_level, channel_number);
```

- **volume\_level**: (0 to -79 dB)
- **channel\_number**: (0 to 5)

### Mute/Unmute

```cpp
id(pt2258_audio).set_mute(1);  // Mute On
id(pt2258_audio).set_mute(0);  // Mute Off
```

## Home Assistant Integration

To add volume controls in Home Assistant, define sliders in ESPHome:

```yaml
number:
  - platform: template
    name: "Channel 1 Volume"
    id: "Channel1Vol"
    step: 1
    min_value: 0
    max_value: 79
    initial_value: 79
    mode: slider
    set_action:
      then:
        - lambda: |-
            id(pt2258_audio).set_channel_vol(x, 0);
```

## Wiring Diagram

| PT2258 Pin | ESP32 Pin |
| ---------- | --------- |
| SDA        | GPIO 21   |
| SCL        | GPIO 22   |
| VCC        | 5V        |
| GND        | GND       |

## References

- [**PT2258 Datasheet**](https://download.mikroe.com/documents/datasheets/PT2258.pdf)
- [**ESPHome Documentation**](https://esphome.io)

## License

This project is licensed under the **MIT License**. Feel free to use, modify, and contribute!

## Contributing

Pull requests are welcome! If you find any issues or have improvements, feel free to submit a PR.

---

