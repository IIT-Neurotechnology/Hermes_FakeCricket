#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Robocricket_AP";
const char* password = "Jiminy_AP";
const char* host = "172.20.10.4"; // IP address of the receiver
const unsigned int port = 32345; // UDP port to send to
const int LED_BLINK = D5;
const int LED_D2 = D2;
WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BLINK, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  digitalWrite(LED_D2, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH); delay(100);
    Serial.println("Connecting to Wi-Fi...");
    digitalWrite(LED_BLINK, LOW); delay(100);
  }
  Serial.println("Connected to WiFi");
  udp.begin(port);
  Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
}

void loop() {
  static bool collecting = false;
  static byte data[12];
  static int dataIndex = 0;

  while (Serial.available()) {
    byte incomingByte = Serial.read();
    if (!collecting) {
      if (incomingByte == 0xAA) { // Start byte detected
        collecting = true;
        dataIndex = 0;
      }
    } else {
      if (incomingByte == 0x55 && dataIndex == 12) { // End byte detected
        float vx, vy, w;
        memcpy(&vx, &data[0], sizeof(vx));
        memcpy(&vy, &data[4], sizeof(vy));
        memcpy(&w,  &data[8], sizeof(w));

        // Send vx and vy via UDP
        udp.beginPacket(host, port);
        udp.write((byte*)&vx, 4);
        udp.write((byte*)&vy, 4);
        udp.write((byte*)&w, 4);
        udp.endPacket();
        
        digitalWrite(LED_D2, !digitalRead(LED_D2)); // Toggle LED to indicate data sent

        collecting = false; // Reset for next packet
      } else if (dataIndex < 12) {
        data[dataIndex++] = incomingByte; // Collect data bytes
      } else {
        // Packet format error, reset
        collecting = false;
      }
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
