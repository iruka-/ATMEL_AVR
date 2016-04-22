**キーボードまにあ!!!(スリー) for Atmel AVRマイコン [#s9b8292b]

いきなりダウンロード：
- [アーカイブ：keybrd-mania-1211.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/keybrd-mania-1211.zip) 
- オープンソース、HEXファイル込みです。

<!-- dummy comment line for breaking list -->

説明：
- ATmega88あるいはATtiny2313に**PS/2タイプのキーボード**を繋いで、**押されたキーに反応するプログラム**を作ることが出来ます。

<!-- dummy comment line for breaking list -->

同梱サンプルプログラム：
- ATmega88あるいはATtiny2313にPS/2タイプのキーボードを繋いで、**電子楽器風のおもちゃ**を作っています。
    - PS/2キーボード側の仕様制限にもよりますが、一応和音が鳴らせます。
- ATmega88のほうでは、**モールス信号発生器風のおもちゃ**にもなります。
    - ファンクションキーを押すことで、電子楽器とモールス発生器が切り替わります。

<!-- dummy comment line for breaking list -->

割り込みリソース：
- ATmega88では、PCINT8かTIMER1のどちらかを選択できます。
- ATtiny2313では、TIMER1割り込みでPS/2キーボードからのCLOCKをサンプリングしています。

<!-- dummy comment line for breaking list -->

用意するもの：
- ATmega88/168あるいはATtiny2313を実装した簡単な実験基板（何かを流用しても可。たとえばArduino基板とか）CPUは5V動作に限る。
- AVR書き込み器([ＨＩＤａｓｐｘ](ＨＩＤａｓｐｘ.md) を推奨。でも何でも良い)
- PS/2タイプのキーボード(USBは不可。USB/PS2両用はたぶん可）とそれをAVRに繋ぐためのメスコネクタのようなもの・・・。((PS/2タイプのMini-DINコネクタはたぶん入手性悪いと思います。PS/2延長ケーブルを買ってきてぶったぎるか、USB/PS2両用キーボードを探してきて、USBコネクタで受けてみるとか・・・。キーボードはキータッチにこだわらないと演奏の面白さ半滅です))
- スピーカーかライン出力のオーディオ。

<!-- dummy comment line for breaking list -->

- - - -
### 積み残し問題
- 音質にやや不満点があります。（今後の課題）
- キーボードからのDATA受信のエラー処理(stop,parity等)を実装していません。
- キーボードへのデータ送信(LED点灯やモード、リピート等設定)を実装していません。
- ATtiny2313では、クロックが低いため音程が低いです。（20MHzにすればATmega88と同等になるはずです）

