// Writing to PI
//int i = 0;
//
//void setup() {
//  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(9600);
//}
//
//void loop() {
//  i = (i + 1) % 2;
//  if (i == 0) {
//    digitalWrite(LED_BUILTIN, HIGH);
//  } else {
//    digitalWrite(LED_BUILTIN, LOW);
//  }
//  Serial.println("hello");
//  delay(1000);
//}

// Reading from PI
int counter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    counter = data.toInt();
    Serial.print("Data: ");
    Serial.println(data);
  }
  if (counter % 2 == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
