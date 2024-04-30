const int GSM_RST = 32;        // Define the pin for modem reset
const int GSM_PWR_KEY = 22;    // Define the pin for modem power key
const int MODEM_RX = 25;      // Define the pin for ESP32's RX to modem's TX
const int MODEM_TX = 26;      // Define the pin for ESP32's TX to modem's RX

#include <Adafruit_ADS1015.h>
#include "HX711.h"

Adafruit_ADS1115 ads1(0x49);
String adcString[8];
HX711 scale;

const int LOADCELL_DOUT_PIN = 33;
const int LOADCELL_SCK_PIN = 32;

void setup() {
  pinMode(GSM_RST, OUTPUT);
  pinMode(GSM_PWR_KEY, OUTPUT);
  pinMode(36, INPUT);    // Analog input-battery voltage monitor
  
  digitalWrite(GSM_PWR_KEY, HIGH);  // Set modem to flight mode
  digitalWrite(GSM_RST, HIGH);
  delay(1000);
  digitalWrite(GSM_RST, LOW);
  delay(1000);
  digitalWrite(GSM_RST, HIGH);
  delay(1000);

  Serial.begin(9600);  // Initialize the serial monitor
  Serial2.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);  // Initialize communication with modem

  Serial.println("SIM AT START >>>>>>>>>>>>>>");
  delay(2000);
  Serial.flush();

  Serial2.println("AT+NCONFIG=AUTOCONNECT,TRUE");
  delay(2000);
  while (Serial2.available()) {
    char response = Serial2.read();
    Serial.write(response);
  }
  Serial2.println("AT");
  delay(2000);
  while (Serial2.available()) {
    char response = Serial2.read();
    Serial.write(response);
  }
  Serial2.println("AT+CEREG?");
  delay(2000);
  while (Serial2.available()) {
    char response = Serial2.read();
    Serial.write(response);
  }
  Serial.flush();

  // Initialize library with data output pin, clock input pin, and gain factor.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Wire.begin(16, 17);
  ads1.begin();
  ads1.setGain(GAIN_ONE);
  Serial.println("The device is powered up");
  Serial.println("Initialized analog inputs");
}

void loop() {
  Serial.print(".");

  Serial2.println("AT");

  while (Serial2.available()) {
    char response = Serial2.read();
    Serial.write(response);
  }
  delay(5000);

  Serial2.println("AT+CEREG?");
  delay(2000);
  while (Serial2.available()) {
    char response = Serial2.read();
    Serial.write(response);
  }

  // Additional code for analog inputs
  Serial.print("I1: ");
  Serial.println(digitalRead(35));
  Serial.print("I2: ");
  Serial.println(digitalRead(34));
  Serial.print("Battery Voltage: ");
  Serial.println(readBattery());
  delay(800);
  printanalog();
  delay(800);
}

int readBattery() {
  unsigned int analog_value;
  analog_value = analogRead(36);
  return analog_value;
}

void printanalog() {
  for (int i = 0; i < 4; i++) {
    adcString[i] = String(ads1.readADC_SingleEnded(i));
    adcString[i] = String(ads1.readADC_SingleEnded(i));
    delay(10);
    Serial.print("A" + String(i + 1) + ": ");
    Serial.print(adcString[i]);
    Serial.print("  ");
  }

  Serial.println("____________________________________");
}
