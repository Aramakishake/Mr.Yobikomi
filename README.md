# Mr.Yobikomi
## 概要
呼び込みくんのBGMをArduinoと圧電ブザーを使って演奏する
## 回路のつなぎ方
とりあえず写真撮って載せとく
## 音程
A4 = 440 Hz
半音上がると、
2^(1/12)≒1.059倍、周波数が高くなる
→今回は
## テンポ
BPM128と仮定する
1minに128*4分音符/休符が入る
4分音符の長さ(s) = 60/128 (s) = 15/32 (s) = 15000/32 (ms) = 468.75 (ms)
ただし、tone関数のパラメタのトーン持続時間の型はunsigned longのようなので
(参考：https://nobita-rx7.hatenablog.com/entry/28248243、詳細は要検証)
とりあえず469(ms)に丸めるか？
→BPM125だと計算楽では...？
4分音符の長さ(s) = 60000/125 (ms) = 480(ms)
◯ひとまず、BPM125とする
つまり、
4分音符 = 480ms
8分音符 = 240ms
とする
## メロディ(メモ)
ソソーラソミソ ソソーラソミソ
ドドドレミーレ ミーーソソーー
ドドドレミーー ドドドレミー
レレレドレミ ソファミレ
##