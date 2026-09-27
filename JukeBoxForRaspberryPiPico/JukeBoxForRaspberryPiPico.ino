// #include <Tone.h>
#include "hardware/pwm.h"
#include "MusicDefs.h"
 // チャタリング防止のための時間(ms)
#define BUTTON_DULATION 200

// テンポ(BPM 125)
#define ONE16TH 120 // 16分
#define OEIGHTH 240 // 8分
#define QUARTER 480 // 4分
#define HALF 960 // 2分

// ピン位置の定義
 // メロディ用ピエゾ素子
#define PIEZO 5
 // コーラス用ピエゾ素子
#define PIEZO2 6
#define BUTTON 1
#define LED 0

// 楽譜
struct Note {
  uint melodyFreq;
  uint bassFreq;
  uint duration;
};

// 楽譜
Note score[] = {
  // お気に入りの唄～ (D)
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NOTE_C5  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_C5  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_E4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  // 一人～ (D)
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_A4  , NO_NOTE  , ONE16TH },
  {NOTE_AS4 , NO_NOTE  , ONE16TH },
  // 聴いてみるの～ (Bm)
  {NOTE_AS4 , NO_NOTE  , OEIGHTH },
  {NOTE_AS4 , NO_NOTE  , OEIGHTH },  
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NOTE_AS4 , NO_NOTE  , OEIGHTH },
  {NOTE_A4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_E4  , NO_NOTE  , ONE16TH },
  {NOTE_F4  , NO_NOTE  , ONE16TH },
  // ～オリ (Bm)
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  // ビアは淋しい
  {NOTE_GS4 , NO_NOTE  , OEIGHTH },
  {NOTE_GS4 , NO_NOTE  , OEIGHTH },
  {NOTE_GS4 , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , ONE16TH },
  {NOTE_GS4 , NO_NOTE  , ONE16TH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  {NOTE_DS4 , NO_NOTE  , OEIGHTH },
  {NOTE_F4  , NO_NOTE  , OEIGHTH },
  // 心～慰めて
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  // くれるか
  {NOTE_FS4 , NO_NOTE  , OEIGHTH },
  {NOTE_FS4 , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
  {NOTE_FS4 , NO_NOTE  , OEIGHTH },
  {NOTE_FS4 , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_A4  , NO_NOTE  , ONE16TH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  // ら～
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NO_NOTE  , NO_NOTE  , OEIGHTH },
};

volatile unsigned int lastInterrupt = millis();

volatile bool switchstate = 0;

void change_switchstate() {
  // チャタリング防止のため早すぎる再呼び出しの場合は早期return
  unsigned long now = millis();

  if (now - lastInterrupt < BUTTON_DULATION) {
    return;
  }
  // スイッチ反転
  switchstate = !switchstate;
  // スイッチがHIGHの場合はLEDを点灯、LOWの場合は消灯
  digitalWrite(LED, switchstate);
  // 最終呼び出し時間を更新
  lastInterrupt = now;
}


// PWMを使用して複数ブザーを鳴らす方式
const int scoreLength = sizeof(score) / sizeof(score[0]);

void setPWMTone(uint pin, uint freq) {

  uint slice = pwm_gpio_to_slice_num(pin);
  uint channel = pwm_gpio_to_channel(pin);

  gpio_set_function(pin, GPIO_FUNC_PWM);

  if (freq == 0) {
    pwm_set_enabled(slice, false);
    return;
  }

  uint32_t clock = 125000000;

  uint32_t divider = 16;
  uint32_t wrap = clock / divider / freq;

  pwm_set_clkdiv(slice, divider);
  pwm_set_wrap(slice, wrap);

  // Serial.print("slice=");
  // Serial.println(slice);

  pwm_set_chan_level(
      slice,
      channel,
      wrap / 2);

  pwm_set_enabled(slice, true);
}

void playNote(const Note& note)
{
    setPWMTone(PIEZO,  note.melodyFreq);
    setPWMTone(PIEZO2, note.bassFreq);
    // Debug
    // Serial.print("melodyFreq=");
    // Serial.println(note.melodyFreq);
    // Serial.print("bassFreq=");
    // Serial.println(note.bassFreq);
    delay(note.duration);
}

void stopPWMTone(uint pin)
{
    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_set_enabled(slice, false);

    // 念のためLOWに戻す
    digitalWrite(pin, LOW);
}

void setup() {
  // DEBUG(シリアルモニタ)
  Serial.begin(9600);
  // 初期設定
  // 5ピンに音圧素子、2ピンにボタンを配置
  pinMode(PIEZO,OUTPUT);
  pinMode(PIEZO2,OUTPUT);
  pinMode(BUTTON,INPUT);
  // 3ピンにLEDを配置
  pinMode(LED,OUTPUT);
  // 割り込み関数
  attachInterrupt(digitalPinToInterrupt(BUTTON), change_switchstate, RISING);
  // テストで音を鳴らしてみる
  setPWMTone(PIEZO, 660);
  setPWMTone(PIEZO2, 440);
  delay(50);
  stopPWMTone(PIEZO);
  stopPWMTone(PIEZO2);
}

void loop() {
  // (DEBUG)スイッチ状態を表示
  // Serial.println(switchstate);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  // switchstate = digitalRead(BUTTON);
  while ( switchstate == HIGH ) {
    // play_Mr_Yobikomi2();

    // 初回はLEDが光らない場合があるので、ここでswtichstateをLEDに反映
    digitalWrite(LED, switchstate);
    playScorePWM();
    stopPWMTone(PIEZO);
    stopPWMTone(PIEZO2);
  }
}

void playScorePWM() {
  for (int i = 0; i < scoreLength; i++) {
    playNote(score[i]);
  }
}

// 注)複数のピエゾ素子に対してtone関数を使うことはできないっぽい
// Toneライブラリを使って動くかどうか確かめる → 動く…動くぞ…！
// どうやって実現してるかソースを見ておくと楽しいかも
void play_Mr_Yobikomi2() {
  // 呼び込みくんの曲を演奏(ハーモニー込み)
  for (int ii=0; ii < 2; ii++){
    // ララーシラファ#ラ * 2
    tone(PIEZO, NOTE_A4, OEIGHTH);
    // tone(PIEZO2, NOTE_D3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO, NOTE_A4, QUARTER);
    // tone(PIEZO2, NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO2, NOTE_FS3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO, NOTE_B4, OEIGHTH);
    // tone(PIEZO2, NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO, NOTE_A4, OEIGHTH);
    // tone(PIEZO2, NOTE_D3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO, NOTE_FS4, QUARTER);
    // tone(PIEZO2, NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO, NOTE_A4, OEIGHTH);
    // tone(PIEZO2, NOTE_FS3, OEIGHTH);
    delay(OEIGHTH);

    // tone(PIEZO2, NOTE_A3, OEIGHTH);
    delay(OEIGHTH);
  }
  // // レレレミファ#ーミ
  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, QUARTER);
  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, QUARTER);
  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_FS4, OEIGHTH + QUARTER);
  // tone(PIEZO2, NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  // // ファ#ーララー
  tone(PIEZO, NOTE_FS4, QUARTER+OEIGHTH);
  // tone(PIEZO2, NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_A4, OEIGHTH);
  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_A4, HALF);
  // tone(PIEZO2, NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  // レレレミファ#ー (G)
  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_G3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_FS4, QUARTER + OEIGHTH);
  // tone(PIEZO2, NOTE_G3, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  // レレレミファ#ー (D)
  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_A3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_A3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_FS4, QUARTER + OEIGHTH);
  // tone(PIEZO2, NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_A3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_A3, OEIGHTH);
  delay(OEIGHTH);

  // ミミミレミファ#(E);
  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_E3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_B3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_GS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_D4, OEIGHTH);
  // tone(PIEZO2, NOTE_B3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_E3, OEIGHTH);
  delay(OEIGHTH);

  // tone(PIEZO2, NOTE_B3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_FS4, OEIGHTH);
  // tone(PIEZO2, NOTE_GS3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_B3, OEIGHTH);
  delay(OEIGHTH);
  // // ラソファ#ミ(Asus4 A)
  tone(PIEZO, NOTE_A4, OEIGHTH);
  // tone(PIEZO2, NOTE_A3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_E4, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_G4, OEIGHTH);
  // tone(PIEZO2, NOTE_D4, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_E4, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_FS4, OEIGHTH);
  // tone(PIEZO2, NOTE_A3, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_E4, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO, NOTE_E4, OEIGHTH);
  // tone(PIEZO2, NOTE_CS4, OEIGHTH);
  delay(OEIGHTH);

  tone(PIEZO2, NOTE_E4, OEIGHTH);
  delay(OEIGHTH);
}
/*
void play_Mr_Yobikomi() {
  // 呼び込みくんの曲を演奏(単音)
  // ララーシラファ#ラ
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,QUARTER);
  delay(QUARTER);
  tone(PIEZO,NOTE_B4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_FS4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(QUARTER);
  // ララーシラファ#ラ
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,QUARTER);
  delay(QUARTER);
  tone(PIEZO,NOTE_B4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_FS4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(QUARTER);
  // レレレミファ#ーミ
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_FS4,QUARTER+OEIGHTH);
  delay(QUARTER+OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  // ファ#ーララー
  tone(PIEZO,NOTE_FS4,QUARTER+OEIGHTH);
  delay(QUARTER+OEIGHTH);
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_A4,QUARTER+OEIGHTH);
  delay(HALF);
  // レレレミファ#ー
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_FS4,QUARTER+OEIGHTH);
  delay(HALF);
  // レレレミファ#ー
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_FS4,QUARTER+OEIGHTH);
  delay(HALF);
  // ミミミレミファ#
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_D4,OEIGHTH);
  delay(OEIGHTH);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(QUARTER);
  tone(PIEZO,NOTE_FS4,OEIGHTH);
  delay(QUARTER);
  // ラソファ#ミ
  tone(PIEZO,NOTE_A4,OEIGHTH);
  delay(QUARTER);
  tone(PIEZO,NOTE_G4,OEIGHTH);
  delay(QUARTER);
  tone(PIEZO,NOTE_FS4,OEIGHTH);
  delay(QUARTER);
  tone(PIEZO,NOTE_E4,OEIGHTH);
  delay(QUARTER);
}
*/