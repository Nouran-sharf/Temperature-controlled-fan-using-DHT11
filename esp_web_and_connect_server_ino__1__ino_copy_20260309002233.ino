#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// --- Pin Definitions ---
#define DHTPIN 2       // The digital pin connected to the DHT11 data pin
#define DHTTYPE DHT11  // Specifying the sensor type

const int relayPin = 3;
const int redLedPin = 5;   // Red LED for FAN OFF / Low temp
const int greenLedPin = 4; // Green LED for FAN ON / High temp

// --- Initialize Components ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is the most common I2C address

void setup() {
  // Set pin modes
  pinMode(relayPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  Serial.begin(9600);
  
  // Start the sensor and display
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Read temperature as Celsius
  float tempc = dht.readTemperature();

  // Error handling: Check if the sensor disconnected or failed
  if (isnan(tempc)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    return; // Stops the loop here and tries again
  }

  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(tempc);
  
  // Update LCD Row 1 (Temperature)
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempc);
  lcd.print(" C   "); // The extra spaces clear any leftover characters
  digitalWrite(redLedPin, HIGH);
  // --- Logic for Fan and LEDs ---
  if (tempc > 28) { 
    // Temperature exceeds 28: Turn ON Fan and Green LED
    digitalWrite(relayPin, HIGH);
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    
    // Update LCD Row 2
    lcd.setCursor(0, 0);
    lcd.print("Status: FAN ON ");
    delay(30000);
  } 
  else {
    digitalWrite(relayPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    
    // Update LCD Row 2
    lcd.setCursor(0, 1);
    lcd.print("Status: FAN OFF");
  }
}