#include <DHT.h>
#include <Servo.h>

#define DHT11_PIN 3       // DHT11 sensor pin
const int motionSensorPin = 2;   // Motion sensor connected to digital pin 2
const int ledPin = 9;             // LED connected to PWM pin 9
#define lightSensorPin A3        // light sensor connected to analog pin A3

// Variables
const unsigned long motionTimeout = 5000;  // 5 seconds (adjust as needed)
bool motionDetected = false;
bool lampState = false;
unsigned long lastMotionTime = 0;


int Spoint =  90;      // Starting point of the servo motor
int motionState = 0;   // Variable to store motion sensor state
int lightsensor = 0;   // Variable to store light sensor value
int LEDBrightness = 0; // Variable to store LED intensity

Servo servo;
DHT dht(DHT11_PIN, DHT11);

void setup() {
  pinMode(motionSensorPin, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);           // Initialize serial communication
  analogWrite(ledPin, 0);       // Initialize LED off
  servo.attach(11);             // Include servo motor PWM pin
  servo.write(Spoint);          // Set the starting point of the servo
  delay(5000);                  // Delay for 5 seconds
  dht.begin();                  // Initialize DHT sensor

}

void loop() {
  int motionState = digitalRead(motionSensorPin); // Read the PIR sensor output
  if (motionState == true) {// Motion detected
    if (!motionDetected) {// Motion has just started
      Serial.println("Motion detected");
      motionDetected = true;
      int lightsensor = analogRead(lightSensorPin); //Reding the LightSensor 
      int lightVal = map(lightsensor,0,1023,0,225);
      int LEDBrightness = 225 - lightVal;
      digitalWrite(ledPin, LEDBrightness);  // Turn on LED
      Serial.println("motion detected");
      Serial.print("   lightValue");
      Serial.println(lightsensor);
      Serial.println("   mapped");
      Serial.println(lightVal);
      Serial.println("   Brightness");
      Serial.println(LEDBrightness);
      lastMotionTime = millis();   // Record time of motion detection
    } else {
      // Motion is already detected, keep LED on
      lastMotionTime = millis();   // Update time of last motion
    }
  } else {
    // No motion detected
    if (motionDetected && (millis() - lastMotionTime >= motionTimeout)) {
      // Motion has stopped for motionTimeout period
      Serial.println("Motion stopped");
      motionDetected = false;
      analogWrite(ledPin, 0);  // Turn off LED
    }
  }
  // Check if lamp state has changed 
  //if (digitalRead(ledPin) == HIGH && !lampState) {
  if (analogRead(ledPin) != 0 && !lampState) {
    // Lamp turned on
    Serial.println("Lamp turned on");
    lampState = true;
 } 
 else if (analogRead(ledPin) == 0 && lampState) {
    // Lamp turned off
    Serial.println("Lamp turned off");
    lampState = false;
  }////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Get the LDR sensor value
  int ldr1 = analogRead(A5);
  int ldr2 = analogRead(A0);

  //Get the difference of these values
  int value1 = abs(ldr1 - ldr2);
  int value2 = abs(ldr2 - ldr1);

  //Check these values using an IF condition
  if ((value1 <= 10) || (value2 <= 10)) {

  } else {
    if (ldr1 > ldr2) {
      Spoint = --Spoint;
    }
    if (ldr1 < ldr2) {
      Spoint = ++Spoint;
    }
  }
  
  //Write values on the servo motor
  servo.write(Spoint);
  delay(10);////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature (C): ");
    Serial.println(temperature);
    Serial.print("Humidity (%): ");
    Serial.println(humidity);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Check if lamp state has changed
  if (analogRead(ledPin) != 0 && !lampState) {
    // Lamp turned on
    Serial.println("Lamp turned on");
    lampState = true;
  } else if (analogRead(ledPin) == 0 && lampState) {
    // Lamp turned off
    Serial.println("Lamp turned off");
    lampState = false;
  }}
