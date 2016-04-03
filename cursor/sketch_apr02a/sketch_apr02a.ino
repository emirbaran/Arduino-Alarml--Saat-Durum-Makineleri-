#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

DateTime simdi;
RTC_DS1307 rtc;
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
long simdikizaman=millis();
long eskizaman=millis();
long lcd_eskizaman=millis();
long cursor_eskizaman=millis();

long debounce_sure = 100;
volatile unsigned long onceki_micros;

byte st=0,dk=0,sn=0,gn=0,ay=0;int yil=0;
byte a_st=0,a_dk;
char alarm = 'K';

int mode=0;
int ekran=0;

void setup() {  
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(1,OUTPUT);
  rtc.begin();
  attachInterrupt(0, Mode_Buton, FALLING);
  lcd.begin(16, 2);
  lcd.noCursor();  
 
}
void ekran_yaz(){
  if(simdikizaman-lcd_eskizaman>100){
    if(ekran==0){
      //lcd.clear();
      lcd.setCursor(4,0);
      if(st<10) lcd.print("0");
      lcd.print(st,DEC);
      lcd.print(":");
      if(dk<10) lcd.print("0");
      lcd.print(dk,DEC);
      lcd.print(":");
      if(sn<10) lcd.print("0");
      lcd.print(sn,DEC);
      lcd.setCursor(13,0);
      lcd.print("(");
      lcd.print(alarm);
      lcd.print(")");
      lcd.setCursor(3,1);
      if(gn<10) lcd.print("0");
      lcd.print(gn,DEC);
      lcd.print("/");
      if(ay<10) lcd.print("0");
      lcd.print(ay,DEC);
      lcd.print("/");
      lcd.print(yil,DEC);
      lcd_eskizaman=simdikizaman;
    }else{
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("(");
      lcd.print(alarm);
      lcd.print(")");
      lcd.setCursor(5,1);
      if(a_st<10) lcd.print("0");
      lcd.print(a_st,DEC);
      lcd.print(":");
      if(a_dk<10)lcd.print("0");
      lcd.print(a_dk,DEC);
      lcd_eskizaman=simdikizaman; 
    }
  }
}
void loop() {
  simdikizaman=millis();
  if(ekran==0){
    switch(mode){
      case 0:
        simdi = rtc.now();
        lcd.noCursor();
        st= simdi.hour();
        dk = simdi.minute();
        sn = simdi.second();
        gn = simdi.day();
        ay = simdi.month();
        yil= simdi.year();
        ekran_yaz();
      break;
      case 10:
        cursor_blink(5,0);//saat
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 20:
        cursor_blink(8,0);
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 30:
        cursor_blink(11,0);
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 40:
        cursor_blink(14,0);
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 50:
        cursor_blink(4,1);
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 60:
        cursor_blink(7,1);
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 70:
        cursor_blink(12,1);
        rtc.adjust(DateTime(yil,ay,gn,st,dk,sn));
      break;
      case 80:
        ekran=1;
        mode=10;
        ekran_yaz();
      break;
    }
  }else{
    switch(mode){
      case 10:
        cursor_blink(7,0);
      break;
      case 20:
        cursor_blink(6,1);
      break;
      case 30:
        cursor_blink(9,1);
      break;
      case 40:
        ekran=0;
        mode=0;
      break;
    }
  }

  if(digitalRead(3)==HIGH){
    delay(50);
    if((simdikizaman-eskizaman>200) & digitalRead(3)==HIGH){
      Say_Buton();
      eskizaman=simdikizaman;
    }
  }
  
  if(alarm=='A'){
    if(a_st == st & a_dk==dk){
      digitalWrite(1,HIGH);
    }else{
      digitalWrite(1,LOW);
      }
  }else{
    digitalWrite(1,LOW);
  }
}
void cursor_blink(byte sutun,byte satir){
  if(simdikizaman-cursor_eskizaman >=0 & simdikizaman-cursor_eskizaman <100){
    lcd.cursor();
    lcd.setCursor(sutun,satir);
  }else if(simdikizaman-cursor_eskizaman >=100 & simdikizaman-cursor_eskizaman <=200){
    lcd.noCursor();
  }else{
    cursor_eskizaman = simdikizaman;
  }
  
}
void Mode_Buton(){
  if((long)(micros() - onceki_micros) >= debounce_sure * 1000) {
    mode+=10;
    if(mode==90)mode=0;
    onceki_micros = micros();
  }
}
void Say_Buton(){
  if(ekran==0){
    switch(mode){
      case 10:
        st++;
        if(st==24)st=0;
      break;
      case 20:
        dk++;
        if(dk==60)dk=0;
      break;
      case 30:
        sn++;
        if(sn==60)sn=0;
      break;
      case 40:
        if(alarm=='K')alarm='A';
        else alarm='K';
      break;
      case 50:
        gn++;
        if(gn==32)gn=1;
      break;
      case 60:
        ay++;
        if(ay==13)ay=1;
      break;
      case 70:
        yil++;
        if(yil==2100)yil=1970;
      break;
      }
  }else{
      switch(mode){
      case 10:
        if(alarm=='K')alarm='A';
        else alarm='K';
      break;
      case 20:
        a_st++;
        if(a_st==24)a_st=0;
      break;
      case 30:
        a_dk++;
        if(a_dk==60)a_dk=0;
      break;
      }
  }
  ekran_yaz();
}



