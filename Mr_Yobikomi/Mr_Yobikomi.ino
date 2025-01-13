#include <Tone.h>
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
// ピン位置の定義
 // メロディ用ピエゾ素子
#define PIEZO 5
 // コーラス用ピエゾ素子
#define PIEZO2 6
#define BUTTON 2
#define LED 3

int switchstate = 0;
Tone piezo1;
Tone piezo2;

void change_switchstate() {  
  // HIGHの場合はLOWに、LOWの場合はHIGHにスイッチ状態を変更
  switchstate ^= HIGH;
  // 現在のswitchstateをSerialMonitorに出力
  Serial.print("switchstate=");
  Serial.println(switchstate);
  // スイッチ状態がHIGHの場合はLEDを点灯、LOWの場合は消灯
  digitalWrite(LED, switchstate);
  // ボタン押下時に切り替わりが頻繁に起きないようにするため1s待つ
  delay(1000);
}

void setup() {
  // DEBUG(シリアルモニタ)
  Serial.begin(9600);
  // 初期設定
  // 5ピンに音圧素子、2ピンにボタンを配置
//  pinMode(PIEZO,OUTPUT);
//  pinMode(PIEZO2,OUTPUT);
  piezo1.begin(PIEZO);
  piezo2.begin(PIEZO2);
  pinMode(BUTTON,INPUT);
  // 3ピンにLEDを配置
  pinMode(LED,OUTPUT);
  // 割り込み関数
  attachInterrupt(0, change_switchstate, RISING);
}

void loop() {
  // ボタンが押されているときに鳴らす
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
    // ララーシラファ#ラ
    piezo1.play(NOTE_A4, OEIGHTH);
    piezo2.play(NOTE_D3, OEIGHTH);
    delay(OEIGHTH);

    piezo1.play(NOTE_A4, QUARTER);
    piezo2.play(NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    piezo2.play(NOTE_FS3, OEIGHTH);
    delay(OEIGHTH);

    piezo1.play(NOTE_B4, OEIGHTH);
    piezo2.play(NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    piezo1.play(NOTE_A4, OEIGHTH);
    piezo2.play(NOTE_D3, OEIGHTH);
    delay(OEIGHTH);

    piezo1.play(NOTE_FS4, QUARTER);
    piezo2.play(NOTE_A3, OEIGHTH);
    delay(OEIGHTH);

    piezo1.play(NOTE_A4, OEIGHTH);
    piezo2.play(NOTE_FS3, OEIGHTH);
    delay(OEIGHTH);

    piezo2.play(NOTE_A3, OEIGHTH);
    delay(OEIGHTH);
  }
  // レレレミファ#ーミ
  piezo1.play(NOTE_D4, OEIGHTH);
  piezo2.play(NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  piezo1.play(NOTE_D4, QUARTER);
  piezo2.play(NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  piezo1.play(NOTE_D4, QUARTER);
  piezo2.play(NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  piezo1.play(NOTE_E4, OEIGHTH);
  piezo2.play(NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  piezo1.play(NOTE_FS4, OEIGHTH + QUARTER);
  piezo2.play(NOTE_B2, OEIGHTH);
  delay(OEIGHTH);

  piezo2.play(NOTE_FS3, OEIGHTH);
  delay(OEIGHTH);

  piezo2.play(NOTE_D3, OEIGHTH);
  delay(OEIGHTH);

  piezo1.play(NOTE_E4, OEIGHTH);
  piezo2.play(NOTE_FS3, OEIGHTH);
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