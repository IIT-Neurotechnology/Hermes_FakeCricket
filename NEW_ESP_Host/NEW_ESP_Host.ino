#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Robocricket_AP";
const char* password = "Jiminy_AP";
const char* host = "172.20.10.4"; // IP address of the receiver
const unsigned int localUdpPort = 32345; // UDP port to send to
const int LED_BLINK = D5;
const int LED_D2 = D2;
bool handshakeConfirmed = false;
WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BLINK, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  digitalWrite(LED_D2, LOW);
  
  WiFi.mode(WIFI_STA); // Put ESP8266 into client mode
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH);
    delay(100);
    Serial.println("Connecting to Wi-Fi...");
    digitalWrite(LED_BLINK, LOW);
    delay(100);
  }
  Serial.println("Connected to WiFi");
  udp.begin(localUdpPort);
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  // Try to perform handshake after connecting to WiFi
  while (!handshakeConfirmed) {
    if (Serial.available()){
      byte command = Serial.read();
        if (command == 0xAA) { // Unique identification command
          Serial.write(0xBB);  // Respond with 0xBB to indicate FireBeetle
          handshakeConfirmed = true;
        }
    }
  }
  Serial.println("Handshake correctly made");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && handshakeConfirmed) {
    digitalWrite(LED_BLINK, LOW);
    if (Serial.available() >= 8) {
      // Assuming two floats are sent consecutively for vx and vy
      byte buf[8];
      Serial.readBytes(buf, 8);
      float vx, vy;
      memcpy(&vx, buf, 4);
      memcpy(&vy, buf + 4, 4);

      // Prepare packetData for UDP transmission
      udp.beginPacket(host, localUdpPort);
      udp.write(buf, 8); // Send vx and vy as part of the UDP packet
      udp.endPacket();
    }
  } else {
    digitalWrite(LED_BLINK, HIGH); // Indicate lost connection or no handshake
  }
}
