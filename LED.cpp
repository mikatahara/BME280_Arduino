#include <Arduino.h>
#include <Wire.h>
#include "LED.h"

//**********************ＬＥＤセットアップ*******************
static uint8_t LedDot_G_cnv[8];
static uint8_t LedDot_R_cnv[8];
static uint8_t LedDot_G[8];

// ３x７ドットの数字
static const uint8_t LEDNum[10][8] = {
  { //"0"
    B11100000,
    B10100000,
    B10100000,
    B10100000,
    B10100000,
    B10100000,
    B11100000,
    B00000000 },
  { //"1"
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00000000 },
  { //"2"
    B11100000,
    B00100000,
    B00100000,
    B11100000,
    B10000000,
    B10000000,
    B11100000,
    B00000000 },
  { //"3"
    B11100000,
    B00100000,
    B00100000,
    B11100000,
    B00100000,
    B00100000,
    B11100000,
    B00000000 },
  { //"4"
    B10100000,
    B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B00000000 },
  { //"5"
    B11100000,
    B10000000,
    B10000000,
    B11100000,
    B00100000,
    B00100000,
    B11100000,
    B00000000 },
  { //"6"
    B11100000,
    B10000000,
    B10000000,
    B11100000,
    B10100000,
    B10100000,
    B11100000,
    B00000000 },
  { //"7"
    B11100000,
    B10100000,
    B10100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00000000 },
  { //"8"
    B11100000,
    B10100000,
    B10100000,
    B11100000,
    B10100000,
    B10100000,
    B11100000,
    B00000000 },
  { //"8"
    B11100000,
    B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B00000000 },
};

static const uint8_t LEDNumS[10][8] = {
  { //"0"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000 },
  { //"1"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000000 },
  { //"2"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11000000 },
  { //"3"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11100000 },
  { //"4"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11110000 },
  { //"5"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111000 },
  { //"6"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111100 },
  { //"7"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111110 },
  { //"8"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111 },
  { //"9"
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B11111111 },
};

//**********************ＬＥＤの初期化*******************
void LED_setup() {
  Wire.begin();                        // initialise the connection
  Wire.setClock(400000L);              //HT16K33のクロックはMax 400kHz
  LED_Driver_Setup(LDaddrs1, 1);       //HT16K33システムオシレータ　ＯＮ
  LED_Driver_Blink(LDaddrs1, 1, 0);    //blink_Hz=0 点滅off, 1は2Hz, 2は1Hz, 3は0.5Hz, on_off=0は消灯、1は点灯
  LED_Driver_Brightness(LDaddrs1, brightness);  // brightness= 0～15
  LED_Driver_DisplayInt(LDaddrs1);     //Display Black OUT
}


//**********************ＬＥＤドライバ　HT16K33　セットアップ*******************
void LED_Driver_Setup(uint8_t LD_addrs, uint8_t on_off) {
  //HT16K33は8X8マトリックスLEDを２台まで制御できる。
  //ドライバＩＣ#1 アドレスは0x70から設定する。
  Wire.beginTransmission(LD_addrs);
  Wire.write(0x20 | on_off);  //システムオシレータをONにする
  Wire.endTransmission();
}
//**********************ＬＥＤドライバ　HT16K33　点滅周期設定*******************
void LED_Driver_Blink(uint8_t LD_addrs, uint8_t on_off, uint8_t blink_Hz) {
  //blink_Hz=0 点滅off, 1は2Hz, 2は1Hz, 3は0.5Hz, on_off=0は消灯、1は点灯　
  Wire.beginTransmission(LD_addrs);
  Wire.write(0x80 | (blink_Hz << 1) | on_off);
  Wire.endTransmission();
}
//**********************ＬＥＤドライバ　HT16K33　明るさ設定*********************
void LED_Driver_Brightness(uint8_t LD_addrs, uint8_t brightness) {
  // brightness= 0～15
  Wire.beginTransmission(LD_addrs);
  Wire.write(0xE0 | brightness);
  Wire.endTransmission();
}
//**********************ＬＥＤドライバ　HT16K33　画面初期化*********************
void LED_Driver_DisplayInt(uint8_t LD_addrs) {
  Wire.beginTransmission(LD_addrs);
  Wire.write(0x00);
  for (int i = 0; i < 8; i++) {
    Wire.write(B00000000);  //Greenの8x8LED初期化
    Wire.write(B00000000);  //Redの8x8LED初期化
  }
  Wire.endTransmission();
}
//**********************Adafruit8x8 Bi-Color ドット bit変換******************************
void LED_Ada88_BiColor_cnv(uint8_t* Bdot1, uint8_t* Bdot2) {  //Adafruit Bicolor LEDでは90度左に回転し、水平反転したら正しく表示される
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      bitWrite(Bdot2[j], i, bitRead(Bdot1[i], j));
    }
  }
}

void LED_SHIFT(uint8_t* Bdot1, uint8_t* Bdot2) {
  for (uint8_t j = 0; j < 8; j++) {
    Bdot2[j] = Bdot1[j] >> 4;
  }
}

//**********************ＬＥＤドライバ　HT16K33　8X8データ送信*******************
void LED_Driver_DisplayOutput(uint8_t LD_addrs, uint8_t* DotB1, uint8_t* DotB2) {
  int i, j;
  Wire.beginTransmission(LD_addrs);
  Wire.write(B00000000);  //これは必要
  for (i = 0; i < 8; i++) {
    Wire.write(DotB1[i]);  //Greenの8x8LEDのバイトをここで送る。
    Wire.write(DotB2[i]);  //Redの8x8LEDのバイトをここで送る。
  }
  Wire.endTransmission();
}

//**********************温度の表示*******************
void disp_temp(float ftmp) {

  uint16_t tmp = (uint16_t)ftmp;
  uint8_t dh = (uint8_t)(tmp / 10.);
  uint8_t dl = (uint8_t)(tmp - dh * 10.);
  uint8_t dll = (uint8_t)((ftmp - tmp) * 10);
  uint8_t LedDot[8];
  uint8_t LedDot_l[8];

  //文字列バイトをそのまま送信
  LED_SHIFT(LEDNum[dl], LedDot_l);
  for (uint8_t j = 0; j < 8; j++) {
    LedDot[j] = LEDNum[dh][j] | LedDot_l[j];
  }
  LED_Ada88_BiColor_cnv(LedDot, LedDot_G_cnv);
  LED_Ada88_BiColor_cnv(LEDNumS[dll], LedDot_R_cnv);
  LED_Driver_DisplayOutput(LDaddrs1, LedDot_G_cnv, LedDot_R_cnv);
}

//**********************湿度の表示*******************
void disp_humid(float fhumi) {

  uint16_t tmp = (uint16_t)fhumi;
  uint8_t dh = (uint8_t)(tmp / 10.);
  uint8_t dl = (uint8_t)(tmp - dh * 10.);
  uint8_t dll = (uint8_t)((fhumi - tmp) * 10);
  uint8_t LedDot[8];
  uint8_t LedDot_l[8];
  uint8_t LedDot_O[8];

  //文字列バイトをそのまま送信
  LED_SHIFT(LEDNum[dl], LedDot_l);
  for (uint8_t j = 0; j < 8; j++) {
    LedDot[j] = LEDNum[dh][j] | LedDot_l[j];
  }

  for (uint8_t j = 0; j < 8; j++) {
    LedDot_O[j] = LedDot[j] | LEDNumS[dll][j];
  }

  LED_Ada88_BiColor_cnv(LedDot, LedDot_G_cnv);
  LED_Ada88_BiColor_cnv(LedDot_O, LedDot_R_cnv);
  LED_Driver_DisplayOutput(LDaddrs1, LedDot_G_cnv, LedDot_R_cnv);
}



