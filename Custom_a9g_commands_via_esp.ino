#include <HardwareSerial.h> // Include for HardwareSerial if not implicitly included

// Define the pins for communication with the A9G
// ESP32 GPIO16 (RX2) -> A9G TX
// ESP32 GPIO17 (TX2) -> A9G RX
const int A9G_RX_PIN = 16; // Connect this to A9G TX
const int A9G_TX_PIN = 17; // Connect this to A9G RX

HardwareSerial A9GSerial(2); // Use Serial2 for communication with A9G (TX2=17, RX2=16)

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);
  while (!Serial); // Wait for serial connection to establish (for some boards)
  Serial.println("ESP32 ready. Sending AT commands to A9G...");
  Serial.println("Type your AT commands in the Serial Monitor below and press Enter.");

  // Initialize Serial2 for communication with A9G
  A9GSerial.begin(115200, SERIAL_8N1, A9G_RX_PIN, A9G_TX_PIN); // Common baud rate for AT modules
  while (!A9GSerial); // Wait for serial connection
  Serial.println("A9G Serial (Serial2) initialized.");

  // Small delay to allow A9G to fully boot up and be ready
  delay(2000);

  // Send a test AT command to check communication
  Serial.println("\nSending 'AT' to A9G...");
  A9GSerial.println("AT");
}

void loop() {
  // Read from ESP32 Serial Monitor (user input) and send to A9G
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // Read until newline
    command.trim(); // Remove leading/trailing whitespace including newline/carriage return

    if (command.length() > 0) {
      Serial.print("Sending to A9G: ");
      Serial.println(command);
      A9GSerial.println(command); // Send the command to A9G
    }
  }

  // Read from A9G and print to ESP32 Serial Monitor
  if (A9GSerial.available()) {
    Serial.print("A9G Response: ");
    while (A9GSerial.available()) {
      Serial.write(A9GSerial.read()); // Read byte by byte and print
    }
    // Ensure a newline after the response for clarity if A9G doesn't send one
    // A9G typically sends CRLF, but adding one here for safety in case.
    Serial.println();
  }
}
