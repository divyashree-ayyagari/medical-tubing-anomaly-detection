#include <Wire.h>
#include <LiquidCrystal_I2C.h>
NEW SKETCH


LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- PIN DEFINITIONS --------
int flowPin = 2;
int pressurePin = A1;

int greenLED = 8;
int yellowLED = 9;
int redLED = 10;

// -------- FLOW SENSOR VARIABLES --------
volatile int pulseCount = 0;
float flowRate = 0;
unsigned long lastFlowTime = 0;

// -------- FILTERING --------
float flowFiltered = 0;
float pressureFiltered = 0;

// -------- BASELINE --------
float baseFlow = 0;
float basePressure = 0;
int sampleCount = 0;
bool baselineReady = false;

// -------- TIMING --------
unsigned long lastReadTime = 0;

// -------- INTERRUPT --------
void countPulse() {
  pulseCount++;
}

// -------- CLASSIFICATION FUNCTION --------
String classify(float flow, float pressure, float baseFlow, float basePressure) {

  if (flow < baseFlow * 0.3 && pressure > basePressure * 1.3) {
    return "BLOCKAGE";
  }
  else if (flow < baseFlow * 0.5 && pressure < basePressure * 0.7) {
    return "LEAK";
  }
  else if (abs(flow - baseFlow) > 50 && abs(pressure - basePressure) > 50) {
    return "AIR BUBBLE";
  }
  else {
    return "NORMAL";
  }
}

// -------- SETUP --------
void setup() {

  Serial.begin(9600);

  pinMode(flowPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowPin), countPulse, RISING);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Initializing...");
}

// -------- LOOP --------
void loop() {

  // -------- FLOW CALCULATION (every 1 sec) --------
  if (millis() - lastFlowTime >= 1000) {
    flowRate = pulseCount / 7.5;  // calibration factor
    pulseCount = 0;
    lastFlowTime = millis();
  }

  // -------- PRESSURE READING --------
  int pressureRaw = analogRead(pressurePin);

  // -------- SIMPLE FILTER --------
  flowFiltered = (flowFiltered * 0.7) + (flowRate * 0.3);
  pressureFiltered = (pressureFiltered * 0.7) + (pressureRaw * 0.3);

  // -------- BASELINE LEARNING --------
  if (!baselineReady) {
    baseFlow += flowFiltered;
    basePressure += pressureFiltered;
    sampleCount++;

    lcd.setCursor(0,0);
    lcd.print("Learning...");
    lcd.setCursor(0,1);
    lcd.print(sampleCount);

    if (sampleCount >= 50) {
      baseFlow /= sampleCount;
      basePressure /= sampleCount;
      baselineReady = true;
      lcd.clear();
    }

    delay(100);
    return;
  }

  // -------- CLASSIFICATION --------
  String status = classify(flowFiltered, pressureFiltered, baseFlow, basePressure);

  // -------- LED CONTROL --------
  if (status == "BLOCKAGE" || status == "AIR BUBBLE") {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  else if (status == "LEAK") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  else {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  // -------- LCD DISPLAY --------
  lcd.setCursor(0,0);
  lcd.print("F:");
  lcd.print(flowFiltered);
  lcd.print(" ");

  lcd.setCursor(0,1);
  lcd.print(status);
  lcd.print("      ");

  // -------- SERIAL PLOTTER OUTPUT --------
  Serial.print(flowFiltered);
  Serial.print(",");
  Serial.println(pressureFiltered);

  delay(200);
}
