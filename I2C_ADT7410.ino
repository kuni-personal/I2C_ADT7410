// ********** どれか一つを有効化(コメントアウトを外す) *******
//#define SLEEP
//#define ONESHOT
#define NORM
//#define SPS
// ***********************************************************

#include <Wire.h>;

int I2CAdrs;

// 初期化
void setup(void) {
  I2CAdrs = 0x48;
  Serial.begin(19200);
  Wire.begin();       // マスタ初期化

#ifdef NORM
  // ノーマル
  Wire.beginTransmission(I2CAdrs);  // S.C発行,CB送信
  Wire.write(0x03);                 // Configuration register 選択
  Wire.write(0x00 | 0x80);          // SLEEP mode 設定 0110 0000
  Wire.endTransmission();           // ストップ・コンディション
#endif

#ifdef SPS
  // SPS
  Wire.beginTransmission(I2CAdrs);  // S.C発行,CB送信
  Wire.write(0x03);                 // Configuration register 選択
  Wire.write(0x40 | 0x80);          // one-shot mode 設定 0100 0000
  Wire.endTransmission();           // ストップ・コンディション
#endif

#ifdef SLEEP
  // スリープ
  Wire.beginTransmission(I2CAdrs);  // S.C発行,CB送信
  Wire.write(0x03);                 // Configuration register 選択
  Wire.write(0x60);                 // SLEEP mode 設定 0110 0000
  Wire.endTransmission();           // ストップ・コンディション
#endif
}

// メインループ
void loop(void) {
  uint16_t val;
  float tmp;
  long int ival;

#ifdef ONSHOT
  // ワンショット リトリガ再設定
  Wire.beginTransmission(I2CAdrs);  // S.C発行,CB送信
  Wire.write(0x03);                 // Configuration register 選択
  Wire.write(0x20 | 0x80);          // one-shot mode 設定
  Wire.endTransmission();           // ストップ・コンディション
  delay(240);
#endif

  Wire.requestFrom(I2CAdrs, 2);       // S.C発行,CB送信
  val = (uint16_t)Wire.read() << 8;   // データの読み出し(上位)
  val |= Wire.read();                 // データの読み出し(下位)

  ival = (long int)val;
  if (val & 0x8000) {       // 符号判定
    // 負数
    ival = ival - 65536;
  }

  tmp = (float)ival / 128.0;
  Serial.println(tmp, 2);     // xx.xx

  delay(1000);
}
