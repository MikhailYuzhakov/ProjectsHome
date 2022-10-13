#define CLK 13
#define DT 10
#define SW 8
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
#define REL 9
#define BUZZ 7

#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <DS3232RTC.h>
#include "GyverEncoder.h"

byte simvol[8]   = {B00110,
                    B01001,
                    B01001,
                    B00110,
                    B00000,
                    B00000,
                    B00000,
                    B00000,};
byte simvol1[8]   = {B11000,
                     B11001,
                     B00010,
                     B00100,
                     B01000,
                     B10011,
                     B00011,
                     B00000,};
byte simvol2[8]   = {B00000,
                     B00000,
                     B00000,
                     B00100,
                     B00100,
                     B10101,
                     B01110,
                     B00100,};
byte simvol3[8]   = {B00000,
                     B00000,
                     B00000,
                     B00000,
                     B00000,
                     B11111,
                     B01110,
                     B00100,};
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
DS3232RTC myRTC;
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой

int tempo = 140;
int buzzer = 7;
uint16_t val = 0;
char str[20];
char str1[20];
int melody[] = {

  // Take on me, by A-ha
  // Score available at https://musescore.com/user/27103612/scores/4834399
  // Arranged by Edward Truong

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
float temp;
float hum;
char str_temp[6];
char str_hum[6];
char str_smk[6];
char str_pwr[6];
int int_temp_out;
char sec1[3];
char hour1[3];
char min1[3];
float temp_out;
float smoke = 43;
float power = 52;
uint8_t min_set = 0;
int cur_min = 59;
int cur_sec = 59;
uint8_t sec_set = 0;
uint8_t pos = 0;
uint8_t ent = 0;
uint8_t l_ent = 0;
uint8_t sec_set_dec = 1;
uint8_t min_set_dec = 1;
bool sw_hold = false;
bool sw_clk = false;
bool time1 = false;
bool rotate = false;
bool turn_on = false;

void take_on_me()
{
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void setup() 
{
  pinMode(BUZZ, OUTPUT);
  pinMode(REL, OUTPUT);
  digitalWrite(REL, HIGH);
  attachPCINT(CLK);
  attachPCINT(DT);
//  Serial.begin(9600);
  myRTC.begin();
  enc1.setType(TYPE1);
  lcd.begin(16, 2);
//  lcd.setCursor(3,0);
//  lcd.print("Smokehouse");
  lcd.createChar(1, simvol);  
  lcd.createChar(2, simvol1);
  lcd.createChar(3, simvol2);
  lcd.createChar(4, simvol3);
//  digitalWrite(7, HIGH);
//  delay(100);
//  digitalWrite(7, LOW);
//  delay(100);
//  digitalWrite(7, HIGH);
//  delay(100);
//  digitalWrite(7, LOW);
//  delay(1000);
  lcd.clear();
  htu.begin();
}
void loop() 
{
  if (cur_min == 0 && cur_sec == 0)
  {
   lcd.clear();
   lcd.setCursor(4,0);
   lcd.print("Finish!");
   sprintf(min1, "XX"); sprintf(sec1, "XX");
   take_on_me();
   turn_on = true;
  }
  enc1.tick();
  if (enc1.isClick()) sw_clk = !sw_clk;
  time_t t;
  tmElements_t tm;
  temp = htu.readTemperature();
  hum = htu.readHumidity();
  temp_out = myRTC.temperature() / 4.0;
  cur_min = min_set_dec*10 + min_set - minute();
  if (cur_min < 0) cur_min = 60 - abs(cur_min);
  cur_sec = sec_set_dec*10 + sec_set - second();
  if (cur_sec < 0) {cur_sec = 60 - abs(cur_sec); cur_min = cur_min - 1;}
  

  if (turn_on) {sprintf(min1, "XX"); sprintf(sec1, "XX");} else
  {
      if (cur_min < 10) {sprintf(min1, "0%d", cur_min);} else {sprintf(min1, "%d", cur_min);}
      if (cur_sec < 10) {sprintf(sec1, "0%d", cur_sec);} else {sprintf(sec1, "%d", cur_sec);}
  }
  
  if (!sw_hold)
  {
    if (time1) 
    {
      digitalWrite(7, HIGH);
      delay(100);
      digitalWrite(7, LOW);
      delay(100);
    }
    int_temp_out = int(temp_out);
    dtostrf(temp, 2, 0, str_temp);
    dtostrf(hum, 2, 0, str_hum);
    dtostrf(smoke, 2, 0, str_smk);
    dtostrf(power, 2, 0, str_pwr);
    sprintf(str, "%s""\1""C   %s""\2""   %s""\2", str_temp, str_hum, str_smk);
    sprintf(str1, "%d""\1""C  %s:%s  %s""\2", int_temp_out, min1, sec1, str_pwr);
    lcd.setCursor(0,0);
    lcd.print(str);
    lcd.setCursor(0,1);
    lcd.print(str1);
    delay(200);
    lcd.clear();
    time1 = false;
  } else
  {
    turn_on = false;
    lcd.clear();
    if (!time1) 
    {
      min_set_dec = 1;
      min_set = 0;
      sec_set_dec = 0;
      sec_set = 1;
      time1 = true;
      digitalWrite(7, HIGH);
      delay(100);
      digitalWrite(7, LOW);
      delay(100);
    } else
    {
      if (min_set_dec > 9) {min_set_dec = 0;}
      if (min_set > 9) {min_set = 0;}
      if (sec_set_dec > 9) {sec_set_dec = 0;}
      if (sec_set > 9) {sec_set = 0;}
      sprintf(str1, "      %u%u:%u%u", min_set_dec, min_set, sec_set_dec, sec_set);
      if (pos > 4) pos = 0;
      lcd.setCursor(6+pos,0);
      if (sw_clk)
      {
        if (l_ent != ent) rotate = true; else rotate = false;
        lcd.print("\4");
        if (rotate)
        {
          switch (pos) 
          {
            case 0:
              min_set_dec++;
            break;
            case 1:
              min_set++;
            break;
            case 2:
              digitalWrite(7, HIGH);
              delay(50);
              digitalWrite(7, LOW);
              delay(50);
              digitalWrite(7, HIGH);
              delay(50);
              digitalWrite(7, LOW);
              delay(50); 
            break;
            case 3:
              sec_set_dec++;
            break;
            case 4:
              sec_set++;
            break;
          }
        }
      } else
      {
        ent = 0;
        lcd.print("\3");
      }
      l_ent = ent;
      lcd.setCursor(0,1);
      lcd.print(str1);
      tm.Hour = 0;
      tm.Minute = 0;
      tm.Second = 0;
      t = makeTime(tm);
      RTC.set(t);
      setTime(t);
    }
  }
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

uint8_t attachPCINT(uint8_t pin) {
  if (pin < 8) { // D0-D7 // PCINT2
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << pin);
    return 2;
  }
  else if (pin > 13) { //A0-A5  // PCINT1
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << pin - 14);
    return 1;
  }
  else { // D8-D13  // PCINT0
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << pin - 8);
    return 0;
  }
}

ISR(PCINT0_vect) 
{
  if (enc1.isRight()) ent++;
  if (enc1.isRightH()) pos++;
  if (enc1.isHolded()) sw_hold = !sw_hold;
  enc1.tick();
}
