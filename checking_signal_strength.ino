void setup() {
  // Initialize hardware serial for debugging
  Serial.begin(115200);
  // Initialize Serial1 for A9G communication
  Serial1.begin(115200);

  // Allow time for the serial connections to initialize
  delay(1000);

  // Send AT command to check signal quality
  Serial1.println("AT+CSQ");
}

void loop() {
  // Check if A9G has sent data
  if (Serial1.available()) {
    // Read the response from A9G
    String response = Serial1.readString();
    // Print the response to the Serial Monitor
    Serial.println("A9G Response: " + response);

    // Parse the signal strength from the response
    int rssi = parseSignalStrength(response);
    if (rssi != -1) {
      Serial.print("Signal Strength (RSSI): ");
      Serial.print(rssi);
      Serial.println(" dBm");
    } else {
      Serial.println("Failed to parse signal strength.");
    }
  }
  delay(1000); // Wait before next read
}

// Function to parse signal strength from AT+CSQ response
int parseSignalStrength(const String& response) {
  int index = response.indexOf("+CSQ: ");
  if (index != -1) {
    int commaIndex = response.indexOf(',', index);
    if (commaIndex != -1) {
      String rssiStr = response.substring(index + 6, commaIndex);
      int rssiValue = rssiStr.toInt();
      if (rssiValue >= 0 && rssiValue <= 31) {
        // Convert rssiValue to dBm
        int rssiDbm = -113 + 2 * rssiValue;
        return rssiDbm;
      }
    }
  }
  return -1; // Parsing failed
}
