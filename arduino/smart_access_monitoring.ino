#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <DHT.h>

// --- LCD pins ---
LiquidCrystal lcd(8, 7, 23, 25, 27, 29);

// --- PIR motion sensor ---
const int pirPin = 12;  // Connect PIR sensor OUT pin here
bool motionDetected = false;

// --- RFID setup ---
#define SS_PIN 53
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);

// --- Servo motor for door lock ---
Servo doorLock;
const int servoPin = 13;  // Connect signal wire of servo  here

// --- Keypad setup ---
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- LEDs and buzzer ---
const int greenLED = 9;
const int redLED = 10;
const int buzzer = 11;

// --- Access credentials ---
const String masterPIN = "8016";
String inputPIN = "";

// Replace with your RFID card's UID
byte validUID[4] = {0xDE, 0xAD, 0xBE, 0xEF};

// Auto-lock timing
unsigned long unlockTime = 0;
const int unlockDuration = 3000;

// --- Ultrasonic Sensor pins and variables ---
const int trigPin = 31;
const int echoPin = 33;
long duration;
int distance;
bool doorOpenedByUltrasonic = false;

// --- DHT11 sensor ---
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Analog pins for light and water sensors
const int lightSensorPin = A0;
const int waterLevelPin = A1;

// Timing for environment update
unsigned long lastEnvUpdate = 0;
const unsigned long envInterval = 10000; // 10 seconds

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(pirPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Ensure LEDs are OFF at startup
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  doorLock.attach(servoPin);
  doorLock.write(0); // Locked

  dht.begin();

  lcd.begin(16, 2);
  lcd.print(" Smart Access ");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  handleKeypad();
  handleRFID();
  handleMotion();
  handleUltrasonic();
  autoLock();

  unsigned long currentMillis = millis();
  if (currentMillis - lastEnvUpdate >= envInterval) {
    displayEnvironment();
    lastEnvUpdate = currentMillis;
  }
}

// --- Keypad logic ---
void handleKeypad() {
  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PIN: ");
    inputPIN += key;
    lcd.print(inputPIN);

    if (key == '#') {
      inputPIN.remove(inputPIN.length() - 1); // Remove '#'
      if (inputPIN == masterPIN) {
        grantAccess("PIN Access");
      } else {
        denyAccess("Wrong PIN");
      }
      inputPIN = "";
    } else if (key == '*') {
      inputPIN = "";
      lcd.clear();
      lcd.print("System Ready");
    }
  }
}

// --- RFID logic ---
void handleRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  if (isAuthorized(rfid.uid.uidByte)) {
    grantAccess("RFID Access");
  } else {
    denyAccess("Unknown Card");
  }

  rfid.PICC_HaltA();  // Stop reading
}

// --- RFID authorization check ---
bool isAuthorized(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] != validUID[i]) return false;
  }
  return true;
}

// --- Access granted routine ---
void grantAccess(String method) {
  lcd.clear();
  lcd.print(method);
  lcd.setCursor(0, 1);
  lcd.print("Access Granted");

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
  tone(buzzer, 1000, 200);
  doorLock.write(90); // Unlock door
  unlockTime = millis();
}

// --- Access denied routine ---
void denyAccess(String reason) {
  lcd.clear();
  lcd.print(reason);
  lcd.setCursor(0, 1);
  lcd.print("Access Denied");

  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  tone(buzzer, 300, 500);
  delay(500);
  noTone(buzzer);
  digitalWrite(redLED, LOW);
}

// --- Auto lock after timeout ---
void autoLock() {
  if (unlockTime > 0 && millis() - unlockTime >= unlockDuration) {
    doorLock.write(0); // Lock door
    unlockTime = 0;
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    lcd.clear();
    lcd.print("System Ready");
  }
}

// --- PIR motion detection ---
void handleMotion() {
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH && !motionDetected) {
    motionDetected = true;

    lcd.clear();
    lcd.print("!! MOTION ALERT !!");
    lcd.setCursor(0, 1);
    lcd.print("INTRUDER ALERT");

    for (int i = 0; i < 3; i++) {
      digitalWrite(redLED, HIGH);
      tone(buzzer, 1000);
      delay(300);
      digitalWrite(redLED, LOW);
      noTone(buzzer);
      delay(300);
    }

    lcd.clear();
    lcd.print("System Ready");
  } else if (pirState == LOW) {
    motionDetected = false;  // Reset flag for next trigger
  }
}

// --- Ultrasonic sensor auto-door control ---
void handleUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > 0 && distance <= 20 && !doorOpenedByUltrasonic) {
    doorOpenedByUltrasonic = true;

    lcd.clear();
    lcd.print("Auto-Door Trigger");
    lcd.setCursor(0, 1);
    lcd.print("Opening...");

    doorLock.write(90); // Unlock
    tone(buzzer, 800, 200);
    delay(3000);        // Keep door open for 3s

    doorLock.write(0);  // Lock again
    lcd.clear();
    lcd.print("System Ready");
  }

  if (distance > 30) {
    doorOpenedByUltrasonic = false;
  }

  delay(200); // Check every 200ms
}

// --- Display environment monitoring data ---
void displayEnvironment() {
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();

  int lightLevel = analogRead(lightSensorPin);
  int waterLevel = analogRead(waterLevelPin);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");
  if (!isnan(tempC)) lcd.print(tempC);
  else lcd.print("Err");

  lcd.print("C H:");
  if (!isnan(humidity)) lcd.print(humidity);
  else lcd.print("Err");
 
  lcd.setCursor(0,1);
  lcd.print("L:");
  lcd.print(lightLevel);
  lcd.print(" W:");
  lcd.print(waterLevel);
}
