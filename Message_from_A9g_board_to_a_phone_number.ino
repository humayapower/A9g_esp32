#include <HardwareSerial.h>

// Define the RX and TX pins for Serial2 (connected to A9G module)
#define RXD2 16  // ESP32 RX pin (connect to A9G TX)
#define TXD2 17  // ESP32 TX pin (connect to A9G RX)

// Initialize Serial2 for communication with A9G
HardwareSerial a9gSerial(2);

// Function to send AT commands to A9G module and wait for a response
void sendATCommand(const String& command, const String& expectedResponse, int timeout) {
    a9gSerial.println(command);
    long int time = millis();
    String response = "";

    while ((millis() - time) < timeout) {
        while (a9gSerial.available()) {
            char c = a9gSerial.read();
            response += c;
        }
        if (response.indexOf(expectedResponse) != -1) {
            Serial.println(response);
            return;
        }
    }
    Serial.println("No response or unexpected response");
}

// Function to generate a random alphanumeric string of a given length
String generateRandomString(int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    String randomString = "";

    for (int i = 0; i < length; i++) {
        int index = random(0, sizeof(charset) - 1);
        randomString += charset[index];
    }
    return randomString;
}

void setup() {
    // Initialize serial communications
    Serial.begin(115200);  // Serial monitor
    a9gSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);  // A9G module
    delay(1000);

    // Seed the random number generator
    randomSeed(analogRead(0));

    // Initialize the A9G module
    Serial.println("Initializing A9G module...");
    sendATCommand("AT", "OK", 2000);  // Test AT startup
    sendATCommand("AT+CMGF=1", "OK", 2000);  // Set SMS to text mode
}

void loop() {
    // Generate a random string of 10 characters
    String randomMessage = generateRandomString(10);
    Serial.print("Generated Random Message: ");
    Serial.println(randomMessage);

    // Replace with the recipient's mobile number, including country code
    String recipientNumber = "+1234567890";

    // Send the SMS
    Serial.print("Sending SMS to ");
    Serial.println(recipientNumber);
    sendATCommand("AT+CMGS=\"" + recipientNumber + "\"", ">", 2000);
    a9gSerial.print(randomMessage);
    a9gSerial.write(26);  // ASCII code for CTRL+Z to send the message
    delay(5000);  // Wait for the message to be sent

    // Wait before sending the next message
    delay(60000);  // Send a message every 60 seconds
}
