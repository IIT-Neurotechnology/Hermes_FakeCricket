#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//Questo e il programma che sta sul robot per il controllore wireless
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
  if (packetSize >= sizeof(float) * 3) { // Ensure the packet contains at least two floats
    float vx, vy, w;
    UDP.read((char*)&vx, sizeof(vx)); // Read the first float (velocity x)
    UDP.read((char*)&vy, sizeof(vy)); // Read the second float (velocity y)
    UDP.read((char*)&w, sizeof(w)); // Read the third float (angular velocity w)
    
    // Start byte to indicate the beginning of a new packet
    byte startByte = 0xAA;
    // End byte to indicate the end of the packet
    byte endByte = 0x55;
    
    // Send data over Serial to Teensy with start and end bytes
    Serial.write(startByte);
    Serial.write((byte*)&vx, sizeof(vx)); // vx float
    Serial.write((byte*)&vy, sizeof(vy)); // vy float
    Serial.write((byte*)&w, sizeof(w)); // w float
    Serial.write(endByte);

    // Clear any remaining bytes in the UDP buffer to prevent overflow
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
