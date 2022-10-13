#define PWR    0xFFA25D
#define MENU   0xFFE21D
#define TEST   0xFF22DD
#define PLUS   0xFF02FD
#define RTRN   0xFFC23D
#define BACK   0xFFE01F
#define PLAY   0xFFA857
#define FRWRD  0xFF906F 
#define BUT0   0xFF6897 
#define MIN    0xFF9867
#define CANCEL 0xFFB04F
#define BUT1   0xFF30CF
#define BUT2   0xFF18E7
#define BUT3   0xFF7A85
#define BUT4   0xFF10EF
#define BUT5   0xFF38C7
#define BUT6   0xFF5AA5
#define BUT7   0xFF42BD
#define BUT8   0xFF4AB5
#define BUT9   0xFF52AD
#define STOP   0xFFFFFFFF
#define PIN    10
#define NUMBER 120

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <iarduino_OLED.h>
#include <IRremote.h>
#include "Adafruit_HTU21DF.h"

iarduino_OLED myOLED(0x3C); //адрес дисплея
decode_results results;
IRrecv irrecv(9); // указываем пин, к которому подключен IR приемник
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBER, PIN);

#ifndef STASSID
#define STASSID "ELTX-2.4GHz_WiFi_FCB0" //Room 52
#define STAPSK  "GP2F095615" //10031998
#endif

ADC_MODE(ADC_VCC);

static uint8_t heat[NUMBER];
extern uint8_t SmallFontRus[], MediumFontRus[], MediumNumbers[], BigNumbers[];
const char* ssid = STASSID;
const char* password = STAPSK;
int cooldown;
int8_t efct_clr = 0;
uint8_t i, k, q, t0, effect, Cooling, Sparking, bright = 50;
uint16_t wait = 50, j;
uint32_t myTimer1, myTimer2;
extern uint8_t Wi_Fi[], update_ota[], black[], point[], point_2[], grad[], 
               Img_Battery_0[], Img_Battery_1[], Img_Battery_2[], Img_Battery_3[],
               Img_Battery_charging[], black_50[], light[], clock1[];
bool menu, pwr, test, plus, rtrn, back, play, frwrd, but0, bmin, cancel, but1, 
     but2, but3, but4, but5, but6, but7, but8, but9, stp, left = true, flag = true;

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

//----------------------------------------------------------
void IR_handler(){
  uint32_t IR_code;
  if (irrecv.decode(&results)) // если данные пришли выполняем команды
  {
    IR_code = results.value;
    irrecv.resume();
    switch (IR_code) {
        case PWR:
          pwr = !pwr;
          Serial.println("PWR");
          break;
        case MENU:
          menu = !menu;
          t0 = 0;
          Serial.print("MENU=");
          myOLED.drawImage(black_50, 0, 25);
          Serial.println(menu);
          break;
        case TEST:
          test = !test;
          t0 = 0;
          Serial.println("TEST");
          break;
        case PLUS:
          plus = !plus;
          t0 = 0;
          Serial.println("PLUS");
          break;
        case RTRN:
          rtrn = !rtrn;
          t0 = 0;
          Serial.println("RTRN");
          break;
        case BACK:
          back = !back;
          t0 = 0;
          Serial.println("BACK");
          break;
        case PLAY:
          play = !play;
          t0 = 0;
          Serial.println("PLAY");
          break;
        case FRWRD:
          frwrd = !frwrd;
          t0 = 0;
          Serial.println("FRWRD");
          break;
        case BUT0:
          but0 = !but0;
          t0 = 0;
          Serial.println("BUT0");
          break;
        case MIN:
          bmin = !bmin;
          t0 = 0;
          Serial.println("MIN");
          break;
        case CANCEL:
          cancel = !cancel;
          t0 = 0;
          Serial.println("CANCEL");
          break;
        case BUT1:
          but1 = !but1;
          t0 = 0;
          Serial.println("BUT1");
          break;
        case BUT2:
          but2 = !but2;
          t0 = 0;
          Serial.println("BUT2");
          break;
        case BUT3:
          but3 = !but3;
          t0 = 0;
          Serial.println("BUT3");
          break;
        case BUT4:
          but4 = !but4;
          t0 = 0;
          Serial.println("BUT4");
          break;
        case BUT5:
          but5 = !but5;
          t0 = 0;
          Serial.println("BUT5");
          break;
        case BUT6:
          but6 = !but6;
          t0 = 0;
          Serial.println("BUT6");
          break;
        case BUT7:
          but7 = !but7;
          t0 = 0;
          Serial.println("BUT7");
          break;
        case BUT8:
          but8 = !but8;
          t0 = 0;
          Serial.println("BUT8");
          break;
        case BUT9:
          but9 = !but9;
          t0 = 0;
          Serial.println("BUT9");
          break;
        case STOP:
          stp = true;
          break;
      } 
  }
}
//----------------------------------------------------------
void onStpClck(uint8_t max_t0){
  if (stp) 
  {
    t0++;
    stp = false;
  }
  if (t0 > max_t0) {
    Serial.println("STOP EFFECT");
    pwr = true;
    t0 = 0;
  }
}
//----------------------------------------------------------
void animation(uint8_t this_effect){ 
  myTimer2 = millis(); //сбрасываем таймер
  //радуга
  if (this_effect == 1) {
    for(i = 0; i < pixels.numPixels(); i++)
      pixels.setPixelColor(i, Wheel(((i*256 / pixels.numPixels()) + j) & 255));
    pixels.show();
    j++;
    if (j >= 256*5) j = 0;
  }
  
  //бегущая точка
  if (this_effect == 2) { 
      if (!left){
        i--;
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        pixels.show();
        if (i == 0) left = true;
      } else {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        pixels.show();
        i++;
        if (i > pixels.numPixels()) left = false;       
      }
    }

  //бегущая радуга
  if (this_effect == 3) {
    for (i = 0; i < pixels.numPixels(); i = i + 3) pixels.setPixelColor(i + q, 0); //выкл
    if (q < 2) q++; else {q = 0; j++;}
    if (j >= 255) j = 0;
    for (i = 0; i < pixels.numPixels(); i = i + 3) pixels.setPixelColor(i + q, Wheel((i+j) % 255)); //ВКЛ
    pixels.show();
    }

  //плавная смена цвета
  if (this_effect == 4) {
    if (flag) k++;
    if (k == 255) 
    {
      flag = false;
      q++;
    }
    if (!flag) k--;
    if (k == 0)
    {
      flag = true;
      q++;
    }
    if (q == 2) {q = 0; j++;}
    if (j > 2) j = 0;
    if (j == 0) setAll(k,0,0);
    if (j == 1) setAll(0,k,0);
    if (j == 2) setAll(0,0,k);
    pixels.show();
  }
  
  //эффект горящего факела
  if (this_effect == 5) {
    Cooling = 50;
    Sparking = 120;
    // Step 1.  Cool down every cell a little
    for (i = 0; i < pixels.numPixels(); i++) {
      cooldown = random(0, ((Cooling * 10) / pixels.numPixels()) + 2);
      if (cooldown > heat[i]) {
        heat[i] = 0;
      } else {
        heat[i] = heat[i] - cooldown;
      }
    }
    
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (k = pixels.numPixels() - 1; k >= 2; k--) {
      heat[k] = (heat[k-1] + heat[k-2] + heat[k-2]) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < Sparking) {
      int y = random(7);
      heat[y] = heat[y] + random(160, 255);
    }
    
    // Step 4.  Convert heat to LED colors
    for (j = 0; j < pixels.numPixels(); j++) {
      setPixelHeatColor(j, heat[j]);
    }
    pixels.show();
  }

  //метеор - не работает
  if (this_effect == 6) {
    uint8_t red = 255, green = 255, blue = 255, 
         meteorSize = 10, meteorTrailDecay = 64;
    bool meteorRandomDecay = true;
    setAll(0,0,0);
                    
    for (j = 0; j < meteorSize; j++) 
      if((k - j < pixels.numPixels()) && (k - j >= 0)) 
        pixels.setPixelColor(k - j, pixels.Color(red, green, blue));     
    
    for (j = 0; j < pixels.numPixels(); j++) 
      if ((!meteorRandomDecay) || (random(10) > 5)) 
      {
        fadeToBlack(j, meteorTrailDecay);  
      }
            
    pixels.show();
    
    if (k > pixels.numPixels()) k = 0;
    k++;   
  }
}
//-----------------------------------------------------------------
void btnSolver(){
  if (pwr) { //нажата кнопка Питание
    for (uint8_t i = 0; i <= NUMBER; i++) pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
    pwr = !pwr;
    effect = 0;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("ВЫКЛ", 0, 25);
  }
  
  if (menu) //нажата кнопка Меню
  {
    myOLED.print("МЕНЮ", 0, 25);
    if (frwrd) //а потом кнопка Вперед
    {
      if (efct_clr + 1 > 8) efct_clr = 0;
      else efct_clr++;
      frwrd = !frwrd;
      myOLED.drawImage(black_50, 0, 25);
      myOLED.print("МЕНЮ >" + String(efct_clr), 0, 25);
      Serial.println(efct_clr);
      pixels.show();
    }
    if (back)  //а потом кнопка Назад
    {
      if (efct_clr - 1 < 0) efct_clr = 8;
      else efct_clr--;
      back = !back;
      myOLED.drawImage(black_50, 0, 25);
      myOLED.print("МЕНЮ >" + String(efct_clr), 0, 25);
      Serial.println(efct_clr);
      pixels.show();  
    }
    switch(efct_clr) {
      case 0: setAll(255, 255, 255); break; //белый
      case 1: setAll(255, 0, 0); break; //красный
      case 2: setAll(0, 255, 0); break; //зеленый
      case 3: setAll(0, 0, 255); break; //синий
      case 4: setAll(255, 163, 16); break; //розовый
      case 5: setAll(255, 16, 134); break; //золотой
      case 6: setAll(0, 234, 190); break; //голубой (аква)
      case 7: setAll(255, 113, 193); break; //теплый белый
      case 8: setAll(255, 0, 115); break; //оранжевый
    }  
  }

  if (but0) {
    for (uint8_t i = 0; i <= NUMBER; i++) pixels.setPixelColor(i, pixels.Color(255,255,255));
    pixels.setBrightness(255);
    pixels.show();
    but0 = !but0;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Кнопка 0", 0, 25);
  }

  if (but1) {
    for (uint8_t i = 0; i <= NUMBER; i++) pixels.setPixelColor(i, pixels.Color(255,0,0));
    pixels.show();
    but1 = !but1;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Кнопка 1", 0, 25);
  }
  
  if (but2) {
    for (uint8_t i = 0; i <= NUMBER; i++) pixels.setPixelColor(i, pixels.Color(0,255,0));
    pixels.show();
    but2 = !but2;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Кнопка 2", 0, 25);
  }
  
  if (but3) {
    for (uint8_t i = 0; i <= NUMBER; i++) pixels.setPixelColor(i, pixels.Color(0,0,255));
    pixels.show();
    but3 = !but3;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Кнопка 3", 0, 25);
  }

  if (but4) {
    effect = 1;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Эффект 1", 0, 25);
    but4 = !but4;
  }
  
  if (but5) {
    i = 0;
    effect = 2;
    but5 = !but5;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Эффект 2", 0, 25);
  }

  if (but6) {
    effect = 3;
    but6 = !but6;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Эффект 3", 0, 25);
  }

  if (but7) {
    effect = 4;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Эффект 4", 0, 25);
    but7 = !but7;
    j = 0;
    q = 0;
    k = 0;
  }

  if (but8) {
    effect = 5;
    but8 = !but8;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Эффект 5", 0, 25);
  }

  if (but9) {
    effect = 6;
    myOLED.drawImage(black_50, 0, 25);
    myOLED.print("Эффект 6", 0, 25);
    k = 0;
    but9 = !but9;
  }

  if (frwrd && menu == false) 
  {
    if (wait + 10 > 1000) wait = 10; else wait = wait + 10;
    Serial.println(wait);
    myOLED.drawImage(black_50, 0, 50);
    myOLED.drawImage(clock1, 0, 49);
    myOLED.print(String(wait), 15, 50);
    frwrd = !frwrd;
  }

  if (back && menu == false) 
  {
    if (wait - 10 < 10) wait = 1000; else wait = wait - 10;
    Serial.println(wait); 
    myOLED.drawImage(black_50, 0, 50);
    myOLED.drawImage(clock1, 0, 49);
    myOLED.print(String(wait), 15, 50);
    back = !back;
  }

  if (plus) {
    if (bright + 10 > 255) bright = 255;
    else bright = bright + 10;
    pixels.setBrightness(bright);
    pixels.show();
    Serial.println(bright);
    myOLED.drawImage(black_50, 0, 63);
    myOLED.drawImage(light, 0, 62);
    myOLED.print(String(bright), 15, 63);
    plus = !plus;
  }
    
  if (bmin) {
    if (bright - 10 < 0) bright = 0;
    else bright = bright - 10;
    pixels.setBrightness(bright);
    pixels.show();
    Serial.println(bright);
    myOLED.drawImage(black_50, 0, 63);
    myOLED.drawImage(light, 0, 62);
    myOLED.print(String(bright), 15, 63);
    bmin = !bmin;
  }
}
//-----------------------------------------------------------------
void wifi_begin(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    //ESP.restart();
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//-----------------------------------------------------------------
void per_begin(){
  Serial.begin(115200);
  irrecv.enableIRIn();
  pinMode(9, INPUT);
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  delay(100);
  if (!htu.begin()) Serial.println("Couldn't find sensor!");
  myOLED.begin();                                                    // Инициируем работу с дисплеем.
  myOLED.setFont(SmallFontRus);
  myOLED.setCoding(TXT_UTF8);
  myOLED.print("Умный дом", 0, 10);
  myOLED.drawLine(0, 14, 127, 14);
  myOLED.drawLine(52, 14, 52, 63);
  pixels.begin();
  pixels.setBrightness(bright);
  pixels.setPixelColor(1, pixels.Color(255, 0, 0));
  pixels.show();
  delay(500);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.show();
}
//-----------------------------------------------------------------
void charge(){
  uint16_t vcc_mv = 0;
  uint8_t bat_status = 0;
  vcc_mv = ESP.getVcc();
  //if (vcc_mv > 4200) myOLED.drawImage(Img_Battery_charging, 112, 10);
  if (vcc_mv > 3700) myOLED.drawImage(Img_Battery_3, 112, 10);
  if (vcc_mv < 3700 && vcc_mv >= 3200) myOLED.drawImage(Img_Battery_2, 112, 10);
  if (vcc_mv < 3200 && vcc_mv >= 2700) myOLED.drawImage(Img_Battery_1, 112, 10);
  if (vcc_mv < 2700) myOLED.drawImage(Img_Battery_0, 112, 10);
}
//-----------------------------------------------------------------
void ota_begin(){
    ArduinoOTA.setHostname("SmartHome");
  //ArduinoOTA.setPassword("10031998");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
  
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  myOLED.print("OTA", 76, 10);
  Serial.println("OTA is ready!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//-----------------------------------------------------------------
void data_refresh(){
  float temp, rel_hum;
  temp = htu.readTemperature();
  rel_hum = htu.readHumidity();
  myOLED.print("Умный дом", 0, 10);
  
  //отделяю целую часть от дробной ради того, чтобы напечатать свой символ точки разделительной
  temp = round(temp*10);
  temp = temp / 10;
  uint8_t int1 = (int)temp; //целая часть температуры
  temp = temp*10 - int1*10;
  uint8_t int2 = (int)temp; //дробная часть температуры
  //печать температуры
  myOLED.setFont(BigNumbers);
  myOLED.print(int1, 57, 42); //печать целой части
  myOLED.drawImage(point, 87, 47); //печать дробной точки
  myOLED.print(int2, 93, 42); //печать целой части
  myOLED.drawImage(grad, 109, 28); //печать знака градус Цельсия 
  
  //расчёт и печать влажности
  rel_hum = round(rel_hum*10);
  rel_hum = rel_hum / 10;
  int1 = (int)rel_hum; //целая часть влажности
  rel_hum = rel_hum*10 - int1*10;
  int2 = (int)rel_hum; //дробная часть влажности
  
  myOLED.setFont(MediumNumbers);
  myOLED.print(int1, 65, 63); //печать целой части
  myOLED.drawImage(point_2, 91, 63); //печать дробной точки
  myOLED.print(int2, 95, 63); //печать целой части
  myOLED.setFont(MediumFontRus);
  myOLED.print("С", 113, 42);
  myOLED.setFont(SmallFontRus);
  myOLED.print("%", 108, 63);

  charge(); //проверяем статус заряда
  wifi_begin(); //пробуем подключиться к Wi-Fi
  wifi_status(); //проверяем статус Wi-Fi
  myTimer1 = millis(); //сбрасываем таймер
}
//-----------------------------------------------------------------
void wifi_status(){
  if (WiFi.status() == WL_CONNECTED) myOLED.drawImage(Wi_Fi, 98, 10); 
    else myOLED.drawImage(black, 98, 10);
}
//-----------------------------------------------------------------
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
//-----------------------------------------------------------------
void setAll(uint8_t red, uint8_t grn, uint8_t blue){
  for (i = 0; i < pixels.numPixels(); i++) pixels.setPixelColor(i, pixels.Color(red, grn, blue));
}
//-----------------------------------------------------------------
void setPixelHeatColor (int Pixel, byte temperature1) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature1/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    pixels.setPixelColor(Pixel, pixels.Color(255, 255, heatramp));
  } else if( t192 > 0x40 ) {             // middle
    pixels.setPixelColor(Pixel, pixels.Color(255, heatramp, 0));
  } else {                               // coolest
    pixels.setPixelColor(Pixel, pixels.Color(heatramp, 0, 0));
  }
}
//-----------------------------------------------------------------
void fadeToBlack(int ledNo, byte fadeValue) {
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = pixels.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    //Serial.println("R="+String(r)+ " G=" + String(g) + " B=" +String(b));
    pixels.setPixelColor(ledNo, pixels.Color(r,g,b));
}
//==========================BEGIN=================================
void setup() 
{
  per_begin(); //запускаем всю переферию
  wifi_begin(); //подключаемся к сети Wi-Fi
  charge(); //отрисовка актуального уровня заряда АКБ
  wifi_status(); //отрисовка актуального статуса wi-fi
  ota_begin(); //запускаем апдейт по воздуху
  data_refresh();
}
//-----------------------------------------------------------------
void loop() 
{
  ArduinoOTA.handle(); //опрос о статусе обновления прошивки
  IR_handler(); //опрос ИК пульта
  onStpClck(10); //нажата ли кнопка стоп?
  btnSolver();
  
  if (millis() - myTimer1 >= 300000) data_refresh(); //задача 1
  if (millis() - myTimer2 >= wait) animation(effect); //задача 2
}
//============================END=================================
