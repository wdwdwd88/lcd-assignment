#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// c0: 찌그러짐 - 착지 후 웅크림
byte c0[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B10101,
  B11111
};

// c1: 바닥 - 원래 모양
byte c1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B10101,
  B11111,
  B01110
};

// c2: 도약 - 위로 길쭉 (꼬리 아래) + 그림자
byte c2[] = {
  B00000,
  B01110,
  B10101,
  B11111,
  B01110,
  B00100,
  B00000,
  B01110
};

// c3: 상승/하강 중간 + 작은 그림자
byte c3[] = {
  B00000,
  B01110,
  B10101,
  B11111,
  B01110,
  B00000,
  B00000,
  B00100
};

// c4: 최고점 + 작은 그림자
byte c4[] = {
  B01110,
  B10101,
  B11111,
  B01110,
  B00000,
  B00000,
  B00000,
  B00100
};

// c5: 낙하 - 아래로 길쭉 (꼬리 위) + 그림자
byte c5[] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B11111,
  B01110,
  B00000,
  B01110
};

// c6: 최고점에서 눈 깜빡
byte c6[] = {
  B01110,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000,
  B00000,
  B00100
};

// c7: 착지 충격 - 철퍽 납작하게 퍼짐
byte c7[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101,
  B11111
};

// 재생 순서와 각 모션 유지 시간(ms)
// c7 -> c0 -> c1 -> c2 -> c3 -> c4 -> c3 -> c5 -> (다시 c7)
// 바닥/최고점은 길게, 빠르게 움직이는 구간은 짧게 -> 중력처럼 부드러운 움직임
const byte seq[] = { 7,  0,   1,  2,  3,   4,  3,  5 };
const int  dur[] = { 70, 130, 60, 60, 90, 200, 90, 60 };
const byte SEQ_LEN = sizeof(seq);

byte step = 0;
unsigned long lastFrame = 0;
unsigned int bounceCount = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, c0);
  lcd.createChar(1, c1);
  lcd.createChar(2, c2);
  lcd.createChar(3, c3);
  lcd.createChar(4, c4);
  lcd.createChar(5, c5);
  lcd.createChar(6, c6);
  lcd.createChar(7, c7);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(seq[0]);
  lastFrame = millis();
}

void loop() {
  if (millis() - lastFrame >= dur[step]) {
    lastFrame = millis();
    step = (step + 1) % SEQ_LEN;
    if (step == 0) bounceCount++;

    byte f = seq[step];
    if (f == 4 && bounceCount % 4 == 3) f = 6;   // 4번 튈 때마다 최고점에서 눈 깜빡

    lcd.setCursor(0, 0);
    lcd.write(f);
  }
}
