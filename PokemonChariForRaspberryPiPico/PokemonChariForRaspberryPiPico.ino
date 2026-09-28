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

// Aメロ楽譜
Note scoreAmero[] = {
  // ソーファーミー
  {NOTE_G5  , NO_NOTE  , OEIGHTH },
  {NOTE_G5  , NO_NOTE  , OEIGHTH },
  {NOTE_G5  , NO_NOTE  , OEIGHTH },
  {NOTE_F5  , NO_NOTE  , OEIGHTH },
  {NOTE_F5  , NO_NOTE  , OEIGHTH },
  {NOTE_F5  , NO_NOTE  , OEIGHTH },
  {NOTE_E5  , NO_NOTE  , OEIGHTH },
  {NOTE_E5  , NO_NOTE  , OEIGHTH },
  // ソッソラソッソラソファミレ
  {NOTE_G5  , NO_NOTE  , OEIGHTH },
  {NOTE_G5  , NO_NOTE  , ONE16TH },
  {NOTE_A5  , NO_NOTE  , ONE16TH },
  {NOTE_G5  , NO_NOTE  , OEIGHTH },
  {NOTE_G5  , NO_NOTE  , ONE16TH },
  {NOTE_A5  , NO_NOTE  , ONE16TH },
  {NOTE_G5  , NO_NOTE  , OEIGHTH },
  {NOTE_F5  , NO_NOTE  , OEIGHTH },
  {NOTE_E5  , NO_NOTE  , OEIGHTH },
  {NOTE_D5  , NO_NOTE  , OEIGHTH },
  // ミードーレーシードシラシソー
  {NOTE_E5  , NO_NOTE  , OEIGHTH },
  {NOTE_C5  , NO_NOTE  , OEIGHTH },
  {NOTE_D5  , NO_NOTE  , OEIGHTH },
  {NOTE_B4  , NO_NOTE  , OEIGHTH },
  {NOTE_C5  , NO_NOTE  , ONE16TH },
  {NOTE_B4  , NO_NOTE  , ONE16TH },
  {NOTE_A4  , NO_NOTE  , ONE16TH },
  {NOTE_B4  , NO_NOTE  , ONE16TH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
  // ミードーレーシードシラシソー
  {NOTE_A4  , NO_NOTE  , OEIGHTH },
  {NOTE_C5  , NO_NOTE  , OEIGHTH },
  {NOTE_A4  , NO_NOTE  , ONE16TH },
  {NOTE_A5  , NO_NOTE  , ONE16TH },
  {NOTE_G5  , NO_NOTE  , ONE16TH },
  {NOTE_F5  , NO_NOTE  , ONE16TH },
  {NOTE_E5  , NO_NOTE  , ONE16TH },
  {NOTE_F5  , NO_NOTE  , ONE16TH },
  {NOTE_G5  , NO_NOTE  , ONE16TH },
  {NOTE_E5  , NO_NOTE  , ONE16TH },
  {NOTE_D5  , NO_NOTE  , OEIGHTH },
  {NOTE_D5  , NO_NOTE  , OEIGHTH },
};

Note scoreSabi[] = {
  {NOTE_G4  , NO_NOTE  , OEIGHTH },
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
const int scoreAmeroLength = sizeof(scoreAmero) / sizeof(scoreAmero[0]);
const int scoreSabiLength = sizeof(scoreSabi) / sizeof(scoreSabi[0]);
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
    delay(note.duration);
    stopPWMTone(PIEZO);
    stopPWMTone(PIEZO2);
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
  while ( switchstate == HIGH ) {
    // 初回はLEDが光らない場合があるので、ここでswtichstateをLEDに反映
    digitalWrite(LED, switchstate);
    playScorePWM();
    stopPWMTone(PIEZO);
    stopPWMTone(PIEZO2);
  }
}

void playScorePWM() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < scoreAmeroLength; i++) {
      playNote(scoreAmero[i]);
    }
  }
  // for (int i = 0; i < scoreSabiLength; i++) {
  //   playNote(scoreSabi[i]);
  // }
}