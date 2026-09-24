from RF24 import *
import serial
import time

# ===== UART FROM STM32 =====
ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)

# ===== NRF SETUP =====
radio = RF24(25, 0)   # CE=GPIO25, CSN=GPIO8

if not radio.begin():
    print("❌ NRF not detected")
    exit()
else:
    print("✅ NRF Ready")

radio.setChannel(76)
radio.setDataRate(RF24_250KBPS)
radio.setPALevel(RF24_PA_HIGH)   # 🔥 stronger signal
radio.setAutoAck(False)

address = b"00001"
radio.openWritingPipe(address)
radio.stopListening()

print("🚀 Pi Telemetry Relay Started")
time.sleep(2)

while True:
    try:
        data = ser.readline().decode(errors='ignore').strip()

        if data:
            print("📥 UART:", data)

            # Ensure packet fits NRF (max 32 bytes)
            packet = ("T:" + data)[:30]

            success = radio.write(packet.encode())

            if success:
                print("📡 Sent:", packet)
            else:
                print("❌ Send failed")

            print("----------------------")

    except Exception as e:
        print("Error:", e)
        time.sleep(1)
