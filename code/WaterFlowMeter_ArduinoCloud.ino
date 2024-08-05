/*
  This program measures the flow rate and total volume of liquid passing through a flow sensor,
  synchronizes the device's time with an NTP server, and updates these values to the Arduino IoT Cloud.
  The program features:
  - Measurement of flow rate in liters per minute.
  - Calculation of total volume in milliliters and liters.
  - Periodic updates of flow rate and total volume to the Arduino IoT Cloud.
  - A reset function to reset the total volume measurement.
*/


#include <WiFi.h>
#include "thingProperties.h"

#define SENSOR  47

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 7.8;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
float totalMilliLitres;

// Interrupt service routine for counting pulses from the flow sensor
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);

  // Initialize IoT properties
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Set the debug message level
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Initialize the flow sensor pin
  pinMode(SENSOR, INPUT_PULLUP);

  // Initialize flow measurement variables
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  // Attach interrupt to the flow sensor pin
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

}

void loop() {
  ArduinoCloud.update();

  // Get current time in milliseconds
  currentMillis = millis();

  // If the interval has passed, calculate flow rate and total volume
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Calculate flow rate and update previousMillis
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Calculate the flow volume in milliliters
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    // Print flow rate and total volume to the Serial Monitor
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(" L/min\t");

    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print(" mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println(" L");

    // Update the cloud variables
    flowRateCloud = flowRate;
    totalLitersCloud = totalMilliLitres / 1000;
  }
}

// Function to reset total volume when the reset button is pressed
void onResetButtonCloudChange()  {
  if (resetButtonCloud) {
    totalLitersCloud = totalMilliLitres = 0;
  }
}
