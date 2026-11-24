#include <LiquidCrystal.h>

// LCD RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Piny
const int ldrPin = A0;
const int buzzerPin = 8;
const int buttonPin = 9;
const int backlightPin = 10;

// ----- CZAS POCZĄTKOWY -----
int hourNow   = 6;
int minuteNow = 29;
int secondNow = 0;

unsigned long lastClockUpdate = 0;

// ----- ALARM -----
int alarmHour   = 6;
int alarmMinute = 30;
int alarmSecond = 0;

bool alarmActive = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(backlightPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Zegar start");
  delay(1000);
  lcd.clear();

  analogWrite(backlightPin, 200);
}

void loop() {

  // ===== Aktualizacja zegara (co 1 s) =====
  if (millis() - lastClockUpdate >= 1000) {
    lastClockUpdate += 1000;

    secondNow++;

    if (secondNow >= 60) {
      secondNow = 0;
      minuteNow++;
    }

    if (minuteNow >= 60) {
      minuteNow = 0;
      hourNow++;
    }

    if (hourNow >= 24) hourNow = 0;
  }

  // ===== Regulacja jasnosci LCD =====
  int ldrValue = analogRead(ldrPin);
  int pwmVal = map(ldrValue, 0, 1023, 255, 50);
  pwmVal = constrain(pwmVal, 0, 255);
  analogWrite(backlightPin, pwmVal);

  // ===== Wyswietlanie =====
  lcd.setCursor(0, 0);
  lcd.print("Czas ");
  printTime(hourNow, minuteNow, secondNow);

  lcd.setCursor(0, 1);
  if (!alarmActive) {
    lcd.print("Alarm ");
    printTime(alarmHour, alarmMinute, alarmSecond);
  } else {
    lcd.print("!!! ALARM !!!   ");
  }

  // ===== Sprawdzenie alarmu =====
  if (hourNow   == alarmHour &&
      minuteNow == alarmMinute &&
      secondNow == alarmSecond) {
    alarmActive = true;
  }

  // ===== Dzialanie alarmu =====
  if (alarmActive) {
    tone(buzzerPin, 1000);

    if (digitalRead(buttonPin) == LOW) {
      alarmActive = false;
      noTone(buzzerPin);
      delay(300);
    }
  } else {
    noTone(buzzerPin);
  }

  delay(80);
}

// ===== Funkcja wyswietlania HH:MM:SS =====
void printTime(int h, int m, int s) {
  if (h < 10) lcd.print('0');
  lcd.print(h);
  lcd.print(':');

  if (m < 10) lcd.print('0');
  lcd.print(m);
  lcd.print(':');

  if (s < 10) lcd.print('0');
  lcd.print(s);
}
