#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//Receiver sta sul robottttt
WiFiUDP UDP;
unsigned int localPort = 32345;
int LED_BLINK = D5;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
int values[3];  // Assuming you want to store the numbers as floats
unsigned int count = 0;
String valueStr = "";
String pacchetto = packetBuffer;

void setup() {
  pinMode(LED_BLINK, OUTPUT);
  Serial.begin(9600);   // This is the UART begin function
  WiFi.mode(WIFI_STA);                        // Set Wi-Fi mode to station (client)
  WiFi.begin("Robocricket_AP", "Jiminy_AP");  // Replace with your hotspot SSID and password

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH);
    delay(500);
    Serial.println("Client-Connecting to Wi-Fi...");
    digitalWrite(LED_BLINK, LOW);
    delay(500);
  }
  Serial.println("Client-Connected to Wi-Fi!");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Local port: ");
  Serial.println(localPort);
  UDP.begin(localPort);
}

void loop() {
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    UDP.read(packetBuffer, packetSize);
    parsePacketData(packetBuffer);
    // Now, you can access the values using the array
    int value1 = abs(values[0]);
    int value2 = abs(values[1]);
    int value3 = abs(values[2]);
    int dir = getDirection(values[0],values[1],values[2]);

    // Send the values through UART
    Serial.write(value1);
    Serial.write(value2);
    Serial.write(value3);
    Serial.write(dir);

    // End the message with a newline character
    Serial.write('\n');
  }

  checkWiFiConnection();
}



void parsePacketData(const char* packetData) {
  String str = packetData;
  str = str.substring(2, str.length() - 1);  // Remove the outer square brackets
  int count = 0;

  char* token = strtok(const_cast<char*>(str.c_str()), " ");
  while (token != NULL) {
    values[count] = atoi(token);  // Convert the token to a float using atof()
    token = strtok(NULL, " ");
    count++;
    if (count >= 3) {
      break;
    }
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLINK, HIGH);
    delay(100);
    memset(values, 0, sizeof(values));
  } else {
    digitalWrite(LED_BLINK, LOW);
  }
}
// Function to determine if a number is positive or negative
byte signToBit(int num) {
  return (num >= 0) ? 1 : 0;
}

byte getDirection(int x, int y, int z) {
  byte direction = 0;

  // Map positive/negative outcomes into a 3-bit number
  direction |= (signToBit(x) << 2);
  direction |= (signToBit(y) << 1);
  direction |= signToBit(z);

  return direction;
}
