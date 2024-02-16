#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>   

const char* ssid = "Robocricket_AP";
const char* password = "Jiminy_AP";
const char* host = "172.20.10.4"; // IP address of the receiver
const int port = 32345; // UDP port to send to
int LED_BLINK = D5;
int LED_D2 = D2;
int SWITCH_D7 = D7;
int JOYSTICK_X = A2; // analog input for the x-axis of the joystick
int JOYSTICK_Y = A3; // analog input for the y-axis of the joystic
unsigned long switchPressedTime = 0;
const int manualModeSwitchTime = 3000;
bool manualMode = false;
int len=0;
WiFiUDP udp;
const int packetSize = 50;  
byte packetData[packetSize];

void setup() {
  Serial.begin(115200);
  pinMode(LED_BLINK, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  digitalWrite(LED_D2, LOW);
  pinMode(SWITCH_D7, INPUT_PULLUP); // Set D7 as input with pull-up resistor
  
  WiFi.mode(WIFI_STA); // put ESP8266 into client mode
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH);
    delay(100);
    Serial.println("Server-Connecting to Wi-Fi...");
    digitalWrite(LED_BLINK, LOW);
    delay(100);
  }
  Serial.println("Connected to WiFi");

  udp.begin(port); // Use any local port

  Serial.println("Connected to Wi-Fi!");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Local port: ");
  Serial.println(port);
  udp.beginPacket(host, port);
}

void loop() {
  if (digitalRead(SWITCH_D7) == HIGH) { // if the switch is closed
    switchPressedTime = millis(); // remember the time it was closed
    manualMode = false; // reset manual mode
  } else if (millis() - switchPressedTime >= manualModeSwitchTime) { // if the switch has been open for more than 3 seconds
    manualMode = true; // enter manual mode
  }

  if (manualMode) {
    digitalWrite(LED_D2, HIGH); // turn off LED on D2
    Serial.println("manual mode");
   // read joystick values
    int joystickX = analogRead(JOYSTICK_X);
    int joystickY = analogRead(JOYSTICK_Y);

    // print joystick values
    Serial.print("Joystick X: ");
    Serial.println(joystickX);
    Serial.print("Joystick Y: ");
    Serial.println(joystickY);
  } else {
    digitalWrite(LED_D2, LOW); // turn off LED on D2
    if (Serial.available()) {
      String str = Serial.readStringUntil('\n');
      len = str.length();
      str.getBytes(packetData,len);
      udp.write(packetData, len);
      udp.endPacket();
    }
  }
  
  if (WiFi.status() != WL_CONNECTED) { 
    digitalWrite(LED_BLINK, HIGH);
    delay(100);
  } else {
    digitalWrite(LED_BLINK, LOW);
  }
}
