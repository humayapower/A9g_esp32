#include <HardwareSerial.h>

// Define the RX and TX pins for Serial2 (connected to A9G module)
#define RXD2 16  // ESP32 RX pin (connect to A9G TX)
#define TXD2 17  // ESP32 TX pin (connect to A9G RX)

// Initialize Serial2 for communication with A9G
HardwareSerial a9gSerial(2);

// Function to send AT commands to A9G module and wait for a response
String sendATCommand(const String& command, const String& expectedResponse, int timeout) {
    a9gSerial.println(command);
    long int time = millis();
    String response = "";

    while ((millis() - time) < timeout) {
        while (a9gSerial.available()) {
            char c = a9gSerial.read();
            response += c;
        }
        if (response.indexOf(expectedResponse) != -1) {
            return response;
        }
    }
    return "No response or unexpected response";
}

void setup() {
    // Initialize serial communications
    Serial.begin(115200);  // Serial monitor
    a9gSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);  // A9G module
    delay(1000);

    Serial.println("Initializing A9G module...");

    // Test AT startup
    Serial.println(sendATCommand("AT", "OK", 2000));

    // Set SMS to text mode
    Serial.println(sendATCommand("AT+CMGF=1", "OK", 2000));

    // Check network registration
    String cregResponse = sendATCommand("AT+CREG?", "+CREG: 1,1", 5000);
    if (cregResponse.indexOf("+CREG: 1,1") != -1) {
        Serial.println("Network registered successfully.");
    } else {
        Serial.println("Network registration failed.");
        return;
    }

    // Attach to GPRS service
    String cgattResponse = sendATCommand("AT+CGATT=1", "OK", 5000);
    if (cgattResponse.indexOf("OK") != -1) {
        Serial.println("GPRS attached successfully.");
    } else {
        Serial.println("GPRS attachment failed.");
        return;
    }

    // Set PDP context with Airtel APN
    String cgdcontResponse = sendATCommand("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"", "OK", 5000);
    if (cgdcontResponse.indexOf("OK") != -1) {
        Serial.println("PDP context set successfully.");
    } else {
        Serial.println("Setting PDP context failed.");
        return;
    }

    // Activate PDP context
    String cgactResponse = sendATCommand("AT+CGACT=1,1", "OK", 5000);
    if (cgactResponse.indexOf("OK") != -1) {
        Serial.println("PDP context activated successfully.");
    } else {
        Serial.println("Activating PDP context failed.");
        return;
    }

    // Check IP address
    String cifsrResponse = sendATCommand("AT+CIFSR", ".", 5000);  // Expecting an IP address
    if (cifsrResponse.indexOf(".") != -1) {
        Serial.println("IP address assigned: " + cifsrResponse);
    } else {
        Serial.println("Failed to obtain IP address.");
        return;
    }

    // Initialize HTTP service
    String httpInitResponse = sendATCommand("AT+HTTPINIT", "OK", 5000);
    if (httpInitResponse.indexOf("OK") != -1) {
        Serial.println("HTTP service initialized.");
    } else {
        Serial.println("Failed to initialize HTTP service.");
        return;
    }

    // Set HTTP URL
    String url = "http://example.com"; // Replace with your target URL
    String httpParaResponse = sendATCommand("AT+HTTPPARA=\"URL\",\"" + url + "\"", "OK", 5000);
    if (httpParaResponse.indexOf("OK") != -1) {
        Serial.println("HTTP parameters set.");
    } else {
        Serial.println("Failed to set HTTP parameters.");
        return;
    }

    // Send HTTP GET request
    String httpGetResponse = sendATCommand("AT+HTTPACTION=0", "+HTTPACTION: 0,", 10000);
    if (httpGetResponse.indexOf("+HTTPACTION: 0,200") != -1) {
        Serial.println("HTTP GET request successful.");
        
        // Read the HTTP response
        String httpReadResponse = sendATCommand("AT+HTTPREAD", "+HTTPREAD:", 10000);
        Serial.println("HTTP Response:");
        Serial.println(httpReadResponse);
    } else {
        Serial.println("HTTP GET request failed.");
    }

    // Terminate HTTP service
    String httpTermResponse = sendATCommand("AT+HTTPTERM", "OK", 5000);
    if (httpTermResponse.indexOf("OK") != -1) {
        Serial.println("HTTP service terminated.");
    } else {
        Serial.println("Failed to terminate HTTP service.");
    }
}

void loop() {
    // Main loop can be used for other tasks
}
