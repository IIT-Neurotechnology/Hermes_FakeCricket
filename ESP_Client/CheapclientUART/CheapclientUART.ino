#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// WiFi and UDP settings
const char* ssid = "Robocricket_AP";
const char* password = "Jiminy_AP";
const unsigned int localPort = 32345;

WiFiUDP UDP;
int LED_BLINK = D5;

void setup() {
  pinMode(LED_BLINK, OUTPUT);
  Serial.begin(115200); // Initialize serial communication
  WiFi.mode(WIFI_STA); // Set Wi-Fi mode to station (client)
  WiFi.begin(ssid, password); // Connect to Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH);
    delay(500);
    Serial.println("Client-Connecting to Wi-Fi...");
    digitalWrite(LED_BLINK, LOW);
    delay(500);
  }
  Serial.println("Client-Connected to Wi-Fi!");
  Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
  UDP.begin(localPort); // Start UDP listener on the specified port
}

void loop() {
  int packetSize = UDP.parsePacket();
  if (packetSize >= sizeof(float) * 2) { // Ensure the packet contains at least two floats
    float vx, vy;
    UDP.read((char*)&vx, sizeof(vx)); // Read the first float (velocity x)
    UDP.read((char*)&vy, sizeof(vy)); // Read the second float (velocity y)
    
    // You can use vx and vy directly now, for example, printing them
    Serial.print("Velocity X: ");
    Serial.print(vx);
    Serial.print(", Velocity Y: ");
    Serial.println(vy);
    delay(1);
        // Clear any remaining bytes in the buffer to prevent overflow
    while (UDP.available()) {
      UDP.read();
    }
  }

  checkWiFiConnection();
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH); // Indicate lost connection
  } else {
    digitalWrite(LED_BLINK, LOW); // Indicate connection is okay
  }
}
