#include <HardwareSerial.h>

HardwareSerial A9GSerial(1); // RX, TX pins for communication with A9G module

void setup() {
  // Start serial communication with the computer
  Serial.begin(115200);
  // Start serial communication with the A9G module
  A9GSerial.begin(115200);

  // Give some time for the module to initialize
  delay(2000);

  // Check SIM status
  checkSIMStatus();

  // Check network registration status
  checkNetworkRegistration();

  // Check signal strength
  checkSignalStrength();

  // Attach to GPRS network
  attachGPRS();

  // Initialize HTTP service
  initializeHTTP();
}

void loop() {
  // Nothing to do here
}

void sendATCommand(String cmd, unsigned long timeout) {
  A9GSerial.println(cmd);
  String response = "";
  unsigned long start = millis();
  while (millis() - start < timeout) {
    if (A9GSerial.available()) {
      response += (char)A9GSerial.read();
    }
  }
  Serial.print("Command: ");
  Serial.println(cmd);
  Serial.print("Response: ");
  Serial.println(response);
}

void checkSIMStatus() {
  sendATCommand("AT+CPIN?", 2000); // Check SIM status
}

void checkNetworkRegistration() {
  sendATCommand("AT+CREG?", 2000); // Check network registration status
}

void checkSignalStrength() {
  sendATCommand("AT+CSQ", 2000); // Check signal strength
}

void attachGPRS() {
  sendATCommand("AT+CGATT=1", 5000); // Attach to GPRS network
  sendATCommand("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"", 2000); // Set APN (replace with your provider's APN)
  sendATCommand("AT+CGACT=1,1", 5000); // Activate GPRS context
}

void initializeHTTP() {
  sendATCommand("AT+HTTPINIT", 2000); // Initialize HTTP service
  sendATCommand("AT+HTTPPARA=\"CID\",1", 2000); // Set PDP context ID
  sendATCommand("AT+HTTPPARA=\"URL\",\"http://your-project-id.firebaseio.com/sensor_data.json\"", 2000); // Set URL
  sendATCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"", 2000); // Set content type
}
