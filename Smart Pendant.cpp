Integrated Health Monitoring with Wearable Smart Pendant:

Source Code:

#include <Wire. h>
#include <Bluetooth Serial .h>
#include <MPU6050_light.h>
BluetoothSerial Serial BT;
MPU6050 mpu (Wire);
const int button1Pin = 4;
const int button2Pin = 5;
bool lastButton1State = HIGH;
bool lastButton2State = HIGH;
bool sent1 = false;
bool sent2 = false;
bool fallSent = false;
void setup() {
 Serial.begin(115200);
 SerialBT.begin("ESP32_BT");
 Serial.println("Bluetooth Started. Connect to 'ESP32_BT'");
 pin Mode(button1Pin, INPUT_PULLUP);
 pinMode(button2Pin, INPUT_PULLUP);
 Wire.begin();
 byte status = mpu.begin();
 if (status != 0) {
 Serial.println("MPU6050 connection failed!");
 while (1);
 }
Serial.println("MPU6050 ready, calibrating...");
 delay(1000);
 mpu.calcOffsets(); // Do not move MPU while calibrating!
 Serial.println("Calibration complete");
}
void loop() {
 mpu.update();
 float accX = mpu.getAccX();
 float accY = mpu.getAccY();
 float accZ = mpu.getAccZ();
 float accMagnitude = sqrt(accX * accX + accY * accY + accZ * accZ);
 // Fall detection logic
 if ((accMagnitude > 2.0 || accMagnitude < 0.3) && !fallSent) {
 SerialBT.println("fall signal");
 Serial.println("fall signal sent");
 fallSent = true;
 delay(500); // debounce fall
 } else if (accMagnitude >= 0.8 && accMagnitude <= 1.2) {
 fallSent = false;
 }
 // Button 1 logic
 bool currentButton1State = digitalRead(button1Pin);
 if (currentButton1State == LOW && lastButton1State == HIGH && !sent1) {
 SerialBT.println("Hungry");
Serial.println("signal1 sent");
 sent1 = true;
 delay(200);
 }
 if (currentButton1State == HIGH) {
 sent1 = false;
 }
 lastButton1State = currentButton1State;
 // Button 2 logic
 bool currentButton2State = digitalRead(button2Pin);
 if (currentButton2State == LOW && lastButton2State == HIGH && !sent2) {
 SerialBT.println("Thirsty");
 Serial.println("signal2 sent");
 sent2 = true;
 delay(200);
 }
 if (currentButton2State == HIGH) {
 sent2 = false;
 }
 lastButton2State = currentButton2State;
}