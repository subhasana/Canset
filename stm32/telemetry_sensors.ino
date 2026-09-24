#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <math.h>

#define MPU_ADDR 0x68

Adafruit_BMP280 bmp;

// UART to Raspberry Pi: RX, TX
HardwareSerial SerialUART(PA10, PA9);

int16_t ax, ay, az;
float velocity = 0;
float dt = 0.5;
float pressureFiltered = 1005.0;

void setup() {
  Serial.begin(115200);        // USB debug
  SerialUART.begin(115200);    // UART to Raspberry Pi

  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();

  // MPU6050 wake
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  // BMP280 init
  if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
    Serial.println("BMP ERROR");
    while (1);
  }

  delay(1000);
}

void loop() {
  // ===== MPU =====
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);

  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();

  float Ax = ax / 16384.0;
  float Ay = ay / 16384.0;
  float Az = az / 16384.0;

  float accelTotal = sqrt(Ax * Ax + Ay * Ay + Az * Az);
  float accelMotion = (accelTotal - 1.0) * 9.81;

  if (fabs(accelMotion) < 0.2) {
    accelMotion = 0;
    velocity = 0;
  }

  velocity += accelMotion * dt;

  // ===== BMP =====
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0;

  pressureFiltered = 0.9 * pressureFiltered + 0.1 * pressure;

  if (pressureFiltered < 980) pressureFiltered = 980;
  if (pressureFiltered > 1030) pressureFiltered = 1030;

  // ===== DATA =====
  String data = String(temperature, 2) + "," +
                String(pressureFiltered, 2) + "," +
                String(accelMotion, 2) + "," +
                String(velocity, 2);

  Serial.println(data);        // Debug
  SerialUART.println(data);    // Send to Pi

  delay(500);
}
