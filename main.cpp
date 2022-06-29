#include<SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>

#define green_led 4
#define red_led 3
#define buzzer 2
#define RECEIVINGCALL 1
#define IDLE 2
#define BUSY 3
#define NO_ANSWER 4
#define TALKING 5
#define ERROR 6
#define config_sms  "This number is configured."
#define ERROR_str "ERROR"
#define CLCC "+CLCC:"
#define CMGL "+CMGL:"

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
SoftwareSerial fingerprint(A4, A5);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerprint);

uint8_t id;

int update_sms = 1;
int sms_send;
int msg_update = 0;

String lat = "lattitude=";
String lon = "\nlongitude=";
String site = "\nhttp://maps.google.com/maps?f=q&q=";
char no_configured = 0;
float lat_int;
float lon_int;

char data[100];
char date[50];
char c_lat[15];
char c_lon[15];
char c[50];
char c_sms_number[15];
char c_sms[20];
int a, b;

boolean notConnected = true;
boolean gsm_connect = false;
unsigned long current_time ;
String numtel;
String msg = "";
String num = "";
String ok = "OK";
String indata = "";
String inputString = "";
int msg_index;
int index1;
int index2;
int alert = 0;
int counter = 0;

int scan_fingerprint = 0;
int no_finger = 1;
int any_error = 1;
int send_msg = 0;
int enroll = 0;
long previous_millis = 0;

String sms_num, sms; // = get_sender_number();

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  finger.begin(57600);
  pinMode(buzzer, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  lcd.clear();
  lcd.print("  Woman Safety");
  lcd.setCursor(0, 1);
  lcd.print(" with GPS track");
  delay(2000);

  while (notConnected == true)
  {
    lcd.clear();
    lcd.print(F("Connecting Gsm"));
    if (connect_gsm())
    {
      lcd.clear();
      lcd.print(F("Gsm Connected"));
      notConnected = false;
    }
    else
    {
      notConnected = true;
      lcd.clear();
      lcd.print(F("Gsm not Connected"));
      while (1);
    }
  }
  delay(2000);
  lcd.clear();
  lcd.print(F("Kindly Call."));
  config_number();
//  gps_track();
//  indata.reserve(200);


  fingerprint.listen();
  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Sensor Found");
  } else {
    lcd.clear();
    lcd.print("Sensor Not Found");
    while (1) {
      delay(1);
    }
  }
  delay(1000);
  lcd.clear();
  lcd.print("Initializing...");
  id = 0;
  while (id < 99)
  {
    deleteFingerprint(id);
    id++;
  }
  id = 1;
  while (any_error != 0)
  {
    getFingerprintEnroll();
  }
  fingerprint.end();
  digitalWrite(green_led, HIGH);
  lcd.clear();
  lcd.print("Monitoring...");
  delay(1000);
//  gps_track();

}

void loop() {

  if (millis() - previous_millis >=  6000)
  {
    fingerprint.listen();
    scan_fingerprint = getFingerprintIDez();
    if (scan_fingerprint == -1)
    {
      digitalWrite(buzzer, HIGH);
      digitalWrite(green_led, LOW);
      digitalWrite(red_led, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(red_led, LOW);
      delay(1000);
      counter++;

    }
    else
    {
      digitalWrite(buzzer, LOW);
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, HIGH);
      counter = 0;
    }
    fingerprint.end();
    previous_millis = millis();
  }

  if ( counter > 1 && send_msg == 0)
  {
    send_msg = 1;
    digitalWrite(buzzer, HIGH);
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
    fingerprint.end();
    gps_track();
    while (1)
    {
      lcd.clear();
      lcd.print("Alert!!!!!");
      lcd.setCursor(0,1);
      lcd.print("Need Help.....");
      digitalWrite(buzzer, HIGH);
      digitalWrite(green_led, LOW);
      digitalWrite(red_led, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(red_led, LOW);
      delay(1000);
    }
  }

  else
  {
    lcd.clear();
    lcd.print("Monitoring...");
    digitalWrite(buzzer, LOW);
    digitalWrite(green_led, HIGH);
    digitalWrite(red_led, LOW);
    delay(1000);
  }
}





