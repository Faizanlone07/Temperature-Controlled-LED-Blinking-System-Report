#include <TimerOne.h>
#include <Arduino.h>

// Define the onboard LED pin
const int LED_PIN = 13;

// Define the LM35 temperature sensor pin
const int TEMP_SENSOR_PIN = A0;

// Define the temperature threshold
const float TEMP_THRESHOLD = 30.0;

// Define the blink interval for below 30 degrees Celsius
const int BLINK_INTERVAL_LOW = 250;

// Define the blink interval for above 30 degrees Celsius
const int BLINK_INTERVAL_HIGH = 500;

int blinkInterval;

// Define the current temperature variable
float currentTemperature;

// Define the previous temperature variable
float previousTemperature;

// Define the timer variable
unsigned long timer;

void timer1Isr() {
  timer++;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

  // Set the LM35 temperature sensor pin as input
  pinMode(TEMP_SENSOR_PIN, INPUT);

  // Initialize the timer variable
  timer = 0;

  // Set up Timer1 for 1 millisecond interrupts
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11); // CTC mode, 8 prescaler
  OCR1A = 156; // 1 millisecond interrupt period
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare match A interrupt

  // Start Timer1
  TCCR1B |= (1 << CS11);

  blinkInterval = BLINK_INTERVAL_LOW;
}

void loop() {
 // Read the temperature from the LM35 sensor
  currentTemperature = analogRead(TEMP_SENSOR_PIN) / 102.4;

  // Check if the temperature has changed
  if (currentTemperature != previousTemperature) {
    // Update the previous temperature variable
    previousTemperature = currentTemperature;

    // Check if the temperature is below the threshold
    if (currentTemperature < TEMP_THRESHOLD) {
      // Set the blink interval to the low value
      blinkInterval = BLINK_INTERVAL_LOW;
    } else {
      // Set the blink interval to the high value
      blinkInterval = BLINK_INTERVAL_HIGH;
    }
  }

  // Check if the timer has expired
  if (timer >= blinkInterval) {
    // Toggle the onboard LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    // Reset the timer
    timer = 0;
  }
}
