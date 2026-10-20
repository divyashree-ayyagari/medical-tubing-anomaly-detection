#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLOW_SENSOR_PIN 3

#define GREEN_LED 8
#define YELLOW_LED 9
#define RED_LED 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile int pulseCount = 0;

int pulsesPerSecond = 0;
float flowRate = 0.0;

//Sliding window variables
float flowHistory[10];
int index = 0;
int count = 0;
float avgFlow = 0.0;

// For fluctuation detection
float lastFlow = 0;

void pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Flow Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  pulseCount = 0;
  delay(1000);

  pulsesPerSecond = pulseCount;
  flowRate = pulsesPerSecond / 7.5;

  // Only consider valid flow (>1 L/min approx)
  if (flowRate > 1.0) {

    flowHistory[index] = flowRate;
    index = (index + 1) % 10;

    if (count < 10) count++;

    // Calculate moving average
    avgFlow = 0;
    for (int i = 0; i < count; i++) {
      avgFlow += flowHistory[i];
    }
    avgFlow /= count;
  }

  String status = "";

  // Reset LEDs
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // LEARNING FIRST (priority fix)
  if (count < 10) {
    status = "LEARNING";
  }

  // BLOCKAGE
  else if (flowRate < 0.1) {
    status = "BLOCKAGE";
    digitalWrite(RED_LED, HIGH);
  }

  // AIR BUBBLE
  else if (abs(flowRate - lastFlow) > 3) {
    status = "AIR BUBBLE";
    digitalWrite(RED_LED, HIGH);
  }

  // LEAKAGE
  else if (flowRate < 0.6 * avgFlow) {
    status = "LEAKAGE";
    digitalWrite(YELLOW_LED, HIGH);
  }

  // NORMAL
  else {
    status = "NORMAL";
    digitalWrite(GREEN_LED, HIGH);
  }

  lastFlow = flowRate;

  // Send to ESP32
  Serial.println(status);

  // LCD Display
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("F:");
  lcd.print(flowRate, 1);
  lcd.print(" A:");
  lcd.print(avgFlow, 1);

  lcd.setCursor(0, 1);
  lcd.print(status);

  // Optional: show learning progress
  if (status == "LEARNING") {
    lcd.setCursor(10, 1);
    lcd.print(count);
  }
}
