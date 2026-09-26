// #include <Tone.h>
#include "hardware/pwm.h"
// テンポ(BPM 125)
#define OEIGHTH 240 // 8分
#define QUARTER 480 // 4分
#define HALF 960 // 2分

// 音程(多分Dメジャーキー)
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
// 音程(コーラス用)
#define NOTE_D3 147
#define NOTE_FS3 185
#define NOTE_A3 220
#define NOTE_B2 124
#define NOTE_G3 196
#define NOTE_E3 165
#define NOTE_B3 247
#define NOTE_GS3 208
#define NOTE_CS4 262
// 音程一帯が不要になった
// 理由：Tone.hでDefineしているため
// ピン位置の定義
 // メロディ用ピエゾ素子
#define PIEZO 5
 // コーラス用ピエゾ素子
#define PIEZO2 6
#define BUTTON 1
#define LED 0

// Tone piezo1;
// Tone piezo2;

// 楽譜
struct Note {
  uint melodyFreq;
  uint baseFreq;
  uint duration;
};

// Aメロを楽譜化
Note scorePhraseA[] = {
  {NOTE_A4, NOTE_D3, OEIGHTH}
};

volatile bool switchstate = 0;

void change_switchstate() {  
  switchstate = !switchstate;
  // HIGHの場合はLOWに、LOWの場合はHIGHにスイッチ状態を変更
  // switchstate ^= HIGH;
  // 現在のswitchstateをSerialMonitorに出力
  // Serial.print("switchstate=");
  // Serial.println(switchstate);
  // スイッチ状態がHIGHの場合はLEDを点灯、LOWの場合は消灯
  digitalWrite(LED, switchstate);
  // // ボタン押下時に切り替わりが頻繁に起きないようにするため1s待つ
  // delay(1000);
}

// const int scoreLength = sizeof(score) / sizeof(score[0]);

// void setPWMTone(uint pin, uint freq) {

//   uint slice = pwm_gpio_to_slice_num(pin);
//   uint channel = pwm_gpio_to_channel(pin);

//   gpio_set_function(pin, GPIO_FUNC_PWM);

//   if (freq == 0) {
//     pwm_set_enabled(slice, false);
//     return;
//   }

//   uint32_t clock = 125000000;

//   uint32_t divider = 4;
//   uint32_t wrap = clock / divider / freq;

//   pwm_set_clkdiv(slice, divider);
//   pwm_set_wrap(slice, wrap);

//   pwm_set_chan_level(
//       slice,
//       channel,
//       wrap / 2);

//   pwm_set_enabled(slice, true);
// }

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
  tone(PIEZO, NOTE_A4, OEIGHTH);
  tone(PIEZO2, NOTE_D3, OEIGHTH);
}

void loop() {
  // ボタンが押されているときに鳴らす
  Serial.println(switchstate);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  switchstate = digitalRead(BUTTON);
  while ( switchstate == HIGH ) {
    play_Mr_Yobikomi2();
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
    // piezo1.play(NOTE_A4, OEIGHTH);
    // piezo2.play(NOTE_D3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO, NOTE_A4, QUARTER);
    // tone(PIEZO2, NOTE_A3, OEIGHTH);
    // piezo1.play(NOTE_A4, QUARTER);
    // piezo2.play(NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    tone(PIEZO2, NOTE_FS3, OEIGHTH);
  //   piezo2.play(NOTE_FS3, OEIGHTH);
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

  // // レレレミファ#ー (G)
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

  // // レレレミファ#ー (D)
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

  // // ミミミレミファ#(E);
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