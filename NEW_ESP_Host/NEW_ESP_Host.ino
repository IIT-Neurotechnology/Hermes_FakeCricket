#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Robocricket_AP";
const char* password = "Jiminy_AP";
const char* host = "172.20.10.4"; // IP address of the receiver
const unsigned int port = 32345; // UDP port to send to
const int LED_BLINK = D5;
const int LED_D2 = D2;
WiFiUDP udp;
bool ledstatus=0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BLINK, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  digitalWrite(LED_D2, HIGH);

  WiFi.mode(WIFI_STA); // Put ESP8266 into client mode
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH); delay(100);
    Serial.println("Connecting to Wi-Fi...");
    digitalWrite(LED_BLINK, LOW); delay(100);
  }
  Serial.println("Connected to WiFi");
  udp.begin(port); // Use any local port
  Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available() > 0) {
    // Check if there's more data than needed for a single command and its data

    byte command = Serial.read(); // Now read the latest command byte
    switch (command) {
      case 0xAA: // Handshake command
        Serial.write(0xAA); // Echo back the handshake byte
        digitalWrite(LED_D2, LOW); // Indicate handshake received
        break;
      case 0xBB: // Velocity data command
        if (Serial.available() >= 8) { // Now there should be exactly 8 bytes for two floats
          float vx, vy;
          ledstatus = !ledstatus;
          digitalWrite(LED_D2, ledstatus); // Toggle LED to indicate data processing
          Serial.readBytes((byte*)&vx, 4); // Read the most recent vx
          Serial.readBytes((byte*)&vy, 4); // Read the most recent vy
          // Send the most recent vx and vy via UDP
          udp.beginPacket(host, port);
          udp.write((byte*)&vx, 4);
          udp.write((byte*)&vy, 4);
          udp.endPacket();
        }
        break;
      default: // Other commands
        ledstatus = !ledstatus;
        delay(500);
        digitalWrite(LED_D2, ledstatus); // Toggle LED to indicate other command processing
        break;
    }
  }

  // Check Wi-Fi status
  if (WiFi.status() != WL_CONNECTED) { 
    digitalWrite(LED_BLINK, HIGH);
    delay(100);
  } else {
    digitalWrite(LED_BLINK, LOW);
  }
}

