#include <HardwareSerial.h>

HardwareSerial A9GSerial(1); // UART1 (RX=16, TX=17)

const char* FIREBASE_URL = "https://a9g-esp32-default-rtdb.firebaseio.com/"; // Replace with your Firebase URL
const char* APN = " airtelgprs.com"; // Replace with your network provider's APN

void setup() {
  Serial.begin(115200);
  A9GSerial.begin(115200, SERIAL_8N1, 16, 17); // Initialize A9G UART

  delay(1000);
  initializeA9G();
}

void loop() {
  float temperature = 25.5; // Replace with actual sensor reading
  String location = "12.34,56.78"; // Replace with actual GPS data
  unsigned long timestamp = millis(); // Replace with actual timestamp if available

  String payload = "{\"temp\":" + String(temperature) + ",\"location\":\"" + location + "\",\"timestamp\":" + String(timestamp) + "}";

  sendToFirebase(payload);
  delay(30000); // Wait for 30 seconds before sending next data
}

void initializeA9G() {
  sendATCommand("AT");
  delay(1000);
  sendATCommand("AT+CPIN?");
  delay(1000);
  sendATCommand("AT+CREG?");
  delay(1000);
  sendATCommand("AT+CGATT=1");
  delay(1000);
  sendATCommand("AT+CGDCONT=1,\"IP\",\"" + String(APN) + "\"");
  delay(1000);
  sendATCommand("AT+HTTPINIT");
  delay(1000);
}

void sendToFirebase(String payload) {
  sendATCommand("AT+HTTPPARA=\"CID\",1");
  delay(500);
  sendATCommand("AT+HTTPPARA=\"URL\",\"" + String(FIREBASE_URL) + "\"");
  delay(500);
  sendATCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(500);
  sendATCommand("AT+HTTPDATA=" + String(payload.length()) + ",10000");
  delay(1000);
  sendATCommand(payload);
  delay(1000);
  sendATCommand("AT+HTTPACTION=1");
  delay(5000);
  sendATCommand("AT+HTTPTERM");
  delay(1000);
}

void sendATCommand(String command) {
  A9GSerial.println(command);
  delay(100);
  while (A9GSerial.available()) {
    Serial.write(A9GSerial.read());
  }
}
