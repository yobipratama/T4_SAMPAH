#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servo;
int angle = 10;

const char* ssid = "Yobi";
const char* password = "pppppppp";

// defines pins numbers
const int trigPin1 = D6; // Ultrasonic Sensor 1 trigger pin
const int echoPin1 = D5; // Ultrasonic Sensor 1 echo pin
const int trigPin2 = D8; // Ultrasonic Sensor 2 trigger pin
const int echoPin2 = D7; // Ultrasonic Sensor 2 echo pin

// defines variables
long duration1, duration2;
int distance1, distance2;

// membuat variable wifiStatus utk menyimpan data status wifi
int wifiStatus;

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

void setup() {
  servo.attach(D4); // Diganti dengan pin yang tersedia di platform Wokwi
  servo.write(angle);

  pinMode(trigPin1, OUTPUT); // Sets the trigPin1 as an Output
  pinMode(echoPin1, INPUT);  // Sets the echoPin1 as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin2 as an Output
  pinMode(echoPin2, INPUT);  // Sets the echoPin2 as an Input

  Serial.begin(9600); // Starts the serial communication

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Trash Capacity:");

  // Membual label status terkneksi dengan nama Wifi
  Serial.println();
  Serial.println();
  Serial.print("Terkoneksi dengan Wifi : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // ketika WiFI.status nilainya TDK sama dg WL_CONNECTED
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop() {
  wifiStatus = WiFi.status();

  if (wifiStatus == WL_CONNECTED) {
    Serial.println("");
    Serial.println("ESP8266 sudah terkonesi dg Wifi!");
    Serial.println("IP address esp8266 : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("WiFi tdk terkoneksi");
  }

  delay(1000); // check for connection every once a second

  // Measure distance from Ultrasonic Sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Measure distance from Ultrasonic Sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Calculate trash capacity percentage based on sensor 1
  int capacityPercentage = map(distance1, 10, 100, 0, 100);

  // Prints the distance and trash capacity on the Serial Monitor
  Serial.print("Distance 1: ");
  Serial.println(distance1);
  Serial.print("Distance 2: ");
  Serial.println(distance2);
  Serial.print("Trash Capacity (%): ");
  Serial.println(capacityPercentage);

  // Display trash capacity on LCD
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear the previous capacity
  lcd.setCursor(0, 1);
  lcd.print(capacityPercentage);
  lcd.print("%");

  delay(10);

  if (distance1 < 30) {
    servo.write(180);
    delay(2000);
  } else {
    servo.write(0);
  }
}
