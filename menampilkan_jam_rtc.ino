#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "PrayerTimes.h"

const int buzzerPin = 9;//gpio 9
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

double times[sizeof(TimeName) / sizeof(char*)];
char longi[16];
char latit[16];
char sholat[7][8] = {"Subh", "Fajr", "Dzhr", "Asar", " ", "Magr", "Isya"};
char daysOfTheWeek[7][12] = {"Ahad", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
int  tahun, bulan, tanggal, hour, minute, second;
String hari;
float lintang      = -5.45;             // latit
float bujur        = 105.26;            // longi
byte  zonaWaktu    = 7; 

void setup() {
  Wire.begin(SDA, SCL);
  rtc.begin();
  lcd.init();
  lcd.backlight();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  set_asr_method(Shafii);                  // Pendapat Imam Syafi'i
  set_high_lats_adjust_method(AngleBased); //
  set_fajr_angle(20);                      // Sudut Waktu Subuh
  set_isha_angle(18);     
}

void loop() {
  DateTime now = rtc.now();
  tahun = now.year();
  bulan = now.month();
  tanggal = now.day();
  hari = daysOfTheWeek[now.dayOfTheWeek()];
  hour = now.hour();
  minute = now.minute();
  second = now.second();
  get_prayer_times(now.year(), now.month(), now.day(), lintang, bujur, zonaWaktu, times);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(hari);
  lcd.print(" ");
  lcd.print(tanggal);
  lcd.print('/');
  lcd.print(bulan);
  lcd.print('/');
  lcd.print(tahun);
  lcd.print("    ");
 for (int i=0;i<sizeof(times)/sizeof(double);i++)
    {
      if(i != 4 && i != 7)
      {
        char tmp[10];
        int hours, minutes;
        get_float_time_parts(times[i], hours, minutes);
        
        lcd.setCursor(0, 1);
        lcd.print(hour);
        lcd.print(':');
        lcd.print(minute);
        lcd.print(" ");
        lcd.print(sholat[i]);
        lcd.print(" ");
        lcd.print(hours);
        lcd.print(':');
        lcd.print(minutes);
        delay(2000);
        
      }
    }
  
  delay(1000);
}
