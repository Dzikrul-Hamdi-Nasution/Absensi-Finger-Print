#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <Wire.h>
#include <TimeLib.h>
#include "DS3231.h"
#include <SoftwareSerial.h>
SoftwareSerial SIM800L(4, 5); // RX | TX
SoftwareSerial mySerial(3, 2);
RTClib RTC;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int Batas_waktu;
int kunci_waktu, waktu,waktu_jam;
int hadir1, hadir2, hadir3, hadir4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SIM800L.begin(9600);
  finger.begin(57600);
  lcd.begin(16, 2);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  Wire.begin();
  delay(5);

  /*if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
    }
    finger.getTemplateCount();
    if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    }
    else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    }*/

  kunci_waktu = 1;
  hadir1 = 0, hadir2 = 0, hadir3 = 0, hadir4 = 0;

}


void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  lcd.print(" Absensi Finger");
  waktu_RTC();

  if (waktu_jam == 7) {
    if (waktu > 0 and waktu < 30 ) {
      getFingerprintIDez();
      delay(50);
    }
  }

  //



  //Serial.print("time out : ");
  //Serial.println(waktu);
  if (waktu_jam == 7) {
    if (waktu > 30) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Waktu Absen");
      lcd.setCursor(0, 1);
      lcd.print("Telah Habis");
      if (hadir1 == 0) {
        sms_tidak_hadir_1();
      }
      if (hadir2 == 0) {
        sms_tidak_hadir_2();
      }
      if (hadir3 == 0) {
        sms_tidak_hadir_3();
      }
      if (hadir4 == 0) {
        sms_tidak_hadir_4();
      }
      hadir1 = 0, hadir2 = 0, hadir3 = 0, hadir4 = 0;
      waktu == 0;
    }
  }








}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);

  if (finger.fingerID == 1) {
    hadir1 = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("ID 1 hadir");
    nomor_1();
    if (kunci_waktu == 1) {
      waktu = Batas_waktu + 1;
      kunci_waktu = 2;
    }
  }
  if (finger.fingerID == 2) {
    hadir2 = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("ID 2 hadir");
    nomor_2();
    if (kunci_waktu == 1) {
      waktu = Batas_waktu + 1;
      kunci_waktu = 2;
    }
  }
  if (finger.fingerID == 3) {
    hadir3 = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("ID 2 hadir");
    nomor_3();
    if (kunci_waktu == 1) {
      waktu = Batas_waktu + 1;
      kunci_waktu = 2;
    }
  }
  if (finger.fingerID == 4) {
    hadir4 = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("ID 2 hadir");
    nomor_4();
    if (kunci_waktu == 1) {
      waktu = Batas_waktu + 1;
      kunci_waktu = 2;
    }
  }
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}


void waktu_RTC() {
  DateTime now = RTC.now();
  //Serial.print(now.year(), DEC);
  //Serial.print('/');
  //Serial.print(now.month(), DEC);
  //Serial.print('/');
  //Serial.print(now.day(), DEC);
  //Serial.print(' ');
  lcd.setCursor(4, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  Batas_waktu = now.minute();
  Serial.print("Batas_waktu : ");
  Serial.println(Batas_waktu);
  lcd.print(':');
  lcd.print(now.second(), DEC);

}

void sms_tidak_hadir_1() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 1 Alpha");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa DAVID tidak hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
}
void sms_tidak_hadir_2() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 2 Alpha");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa WINDA tidak hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
}
void sms_tidak_hadir_3() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 3 Alpha");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa BANGKIT tidak hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
}
void sms_tidak_hadir_4() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 4 Alpha");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa MARDALENA tidak hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
}


void nomor_1() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  int jam = (now.hour());
  int menit = (now.minute());
  int detik = (now.second());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 1 Hadir");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa DAVID telah hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  SIM800L.print(" Pada jam " );
  SIM800L.print(jam );
  SIM800L.print(":" );
  SIM800L.print(menit );
  SIM800L.print(":" );
  SIM800L.print(detik );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
  finger.fingerID = 0;
}

void nomor_2() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  int jam = (now.hour());
  int menit = (now.minute());
  int detik = (now.second());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 2 Hadir");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa WINDA telah hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  SIM800L.print(" Pada jam " );
  SIM800L.print(jam );
  SIM800L.print(":" );
  SIM800L.print(menit );
  SIM800L.print(":" );
  SIM800L.print(detik );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
  finger.fingerID = 0;
}

void nomor_3() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  int jam = (now.hour());
  int menit = (now.minute());
  int detik = (now.second());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 3 Hadir");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa BANGKIT telah hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  SIM800L.print(" Pada jam " );
  SIM800L.print(jam );
  SIM800L.print(":" );
  SIM800L.print(menit );
  SIM800L.print(":" );
  SIM800L.print(detik );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
  finger.fingerID = 0;
}

void nomor_4() {
  DateTime now = RTC.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  int jam = (now.hour());
  int menit = (now.minute());
  int detik = (now.second());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID 4 Hadir");
  lcd.setCursor(0, 1);
  lcd.print("Mengirim SMS....");
  SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
  SIM800L.write("AT+CMGS=\"082272702466\"\r\n");
  delay(1000);
  SIM800L.write("Mahasiswa MARDALENA telah hadir pada hari " );
  SIM800L.print(tahun );
  SIM800L.print("/" );
  SIM800L.print(bulan );
  SIM800L.print("/" );
  SIM800L.print(hari );
  SIM800L.print(" Pada jam " );
  SIM800L.print(jam );
  SIM800L.print(":" );
  SIM800L.print(menit );
  SIM800L.print(":" );
  SIM800L.print(detik );
  delay(1000);
  SIM800L.write((char)26);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SMS Terkirim ");
  delay(4000);
  lcd.clear();
  finger.fingerID = 0;
}
