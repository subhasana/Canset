#include <SPI.h>
#include <RF24.h>

// CE = GPIO4, CSN = GPIO5
RF24 radio(4, 5);

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin(18, 19, 23, 5);

  if (!radio.begin()) {
    Serial.println("❌ NRF ERROR");
    while (1);
  }

  radio.setChannel(76);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setAutoAck(false);

  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("📡 ESP32 Ground Station Ready");
}

void loop() {
  if (radio.available()) {
    char text[32] = {0};

    radio.read(&text, sizeof(text));

    Serial.print("📩 Raw: ");
    Serial.println(text);

    // ===== PARSE TELEMETRY =====
    if (text[0] == 'T' && text[1] == ':') {
      char *data = text + 2;

      float temp, press, accel, vel;

      int result = sscanf(data, "%f,%f,%f,%f",
                          &temp, &press, &accel, &vel);

      if (result == 4) {
        Serial.println("----- Parsed Data -----");
        Serial.print("🌡 Temp: "); Serial.println(temp);
        Serial.print("🌍 Pressure: "); Serial.println(press);
        Serial.print("📈 Accel: "); Serial.println(accel);
        Serial.print("🚀 Velocity: "); Serial.println(vel);
        Serial.println("----------------------");
      } else {
        Serial.println("⚠️ Parse Error");
      }
    }
  }
}
