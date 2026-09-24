# Cansat Pin Connections

## Transmitter Side

### Raspberry Pi 4 ↔ nRF24L01

| nRF24L01 | Raspberry Pi 4 |
|---|---|
| VCC | 3.3V (physical pin 17) |
| GND | GND (physical pin 25) |
| CE | GPIO25 (physical pin 22) |
| CSN | GPIO8 (physical pin 24) |
| SCK | GPIO11 (physical pin 23) |
| MOSI | GPIO10 (physical pin 19) |
| MISO | GPIO9 (physical pin 21) |
| IRQ | Not connected |

### Raspberry Pi 4 ↔ STM32 Black Pill UART

| Raspberry Pi 4 | STM32 Black Pill |
|---|---|
| GPIO14 / UART TX (physical pin 8) | RX |
| GPIO15 / UART RX (physical pin 10) | TX |
| GND | GND |

### MPU6050 ↔ STM32 Black Pill

| MPU6050 | STM32 Black Pill |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCL | PB8 (I²C SCL) |
| SDA | PB9 (I²C SDA) |
| AD0 | GND (I²C address: `0x68`) |
| INT | Not connected |
| XDA | Not connected |
| XCL | Not connected |

### BMP280 ↔ STM32 Black Pill

| BMP280 | STM32 Black Pill |
|---|---|
| VCC/VIN | 3.3V |
| GND | GND |
| SCL | PB8 (I²C SCL) |
| SDA | PB9 (I²C SDA) |
| CS/CSB | 3.3V (I²C mode) |
| SDO | GND (I²C address: `0x76`) |

The MPU6050 and BMP280 share the STM32 I²C bus on PB8/PB9. Their addresses do not conflict.

## Receiver Side

### nRF24L01 ↔ ESP32

| nRF24L01 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| CE | GPIO4 |
| CSN | GPIO5 |
| SCK | GPIO18 |
| MISO | GPIO19 |
| MOSI | GPIO23 |
| IRQ | Not connected |

## Electrical Notes

- Use 3.3V power and logic for the nRF24L01, MPU6050, and BMP280.
- Keep all grounds common within each connected system.
- Ensure the I²C bus has pull-up resistors to 3.3V if the breakout boards do not provide them.
- The STM32 UART must use 3.3V-compatible logic when connected to Raspberry Pi GPIO pins.
