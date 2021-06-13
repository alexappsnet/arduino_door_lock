/*
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 8
#define SERVO_PIN 7

#define GOOD_CARD {245, 24, 58, 190 }

#define NO false
#define YES true

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myservo;

byte allow_key[4] = GOOD_CARD;
int servo_angle;

void turn_servo(int angle)
{
  servo_angle = angle;
  myservo.write(servo_angle);
}

void sound(int hz, int delay_milliseconds)
{
  tone(BUZZER_PIN, hz);
  delay(delay_milliseconds);
  noTone(BUZZER_PIN);
}

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  myservo.attach(SERVO_PIN);
  delay(1000);
  turn_servo(30);
  delay(1000);
  turn_servo(0);
}

void let_them_in()
{
  Serial.println("WELCOME, Boss");
  sound(3000, 200);

  if (servo_angle == 0)
    turn_servo(30);
  else
    turn_servo(0);
}

void no_no()
{
  Serial.println("Who are you?");

  sound(1000, 300);
  sound(2000, 500);
  sound(1000, 300);
  sound(2000, 500);
}

bool read_card()
{
  return rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial();
}

bool allow_card()
{
  for (byte i = 0; i < 4; i++)
  {
    Serial.println(rfid.uid.uidByte[i]);

    if (rfid.uid.uidByte[i] != allow_key[i])
      return NO;
  }
  return YES;
}

void reset_reader()
{
  rfid.PICC_HaltA();
}

void loop() {
  if (read_card())
  {
    if (allow_card())
      let_them_in();
    else
      no_no();
  }

  reset_reader();
}

