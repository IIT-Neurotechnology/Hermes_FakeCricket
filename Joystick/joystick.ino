
void setup() {
  // initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin A17
  int analogValueA17 = analogRead(A17);
  // map the value to -255 to 255
  int mappedValueA17 = map(analogValueA17, 0, 1023, -245, 254);
  // if the value is under 15, set it to 0
  if(abs(mappedValueA17) < 15) {
    mappedValueA17 = 0;
  }

  // read the input on analog pin A19
  int analogValueA19 = analogRead(A19);
  // map the value to -255 to 255
  int mappedValueA19 = map(analogValueA19, 0, 1023, -254, 254);
  // if the value is under 15, set it to 0
  if(abs(mappedValueA19) < 15) {
    mappedValueA19 = 0;
  }

  // print the values to the serial port
  Serial.print("A17: ");
  Serial.print(mappedValueA17);
  Serial.print(" , A19: ");
  Serial.println(mappedValueA19);

  // delay before the next reading
  delay(100);
}
