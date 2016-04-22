*MINTIAケースに仕込んだHIDaspx [#ze0e7b67]

- HIDaspxハードウェアが足りなくなったので、勢いで作ってしまいました。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0114.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0114.jpg) 

~
~

- 本当は、このMINTIAケースにはPIC　18F14K50を入れる予定でした。
- GREEN HOUSEの超小型USBメモリースティックタイプのmp3プレイヤーを入れる案もあったのですが、
- むしろUSB-Aコネクタ形状のままのほうが使いやすいことに気づいたのでバラすのをやめました。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0115.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0115.jpg) 

~
~
- PIC18FのままではAVRやPICが焼けません。
- HIDaspxを入れたほうが実用性が高いことに気づきました。

<!-- dummy comment line for breaking list -->



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0116.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0116.jpg) 

~
~
- 外見は今のところほぼMINTIAですが、ISPケーブルを引き出す予定なので
- いずれはしっぽ付きになります。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0117.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/min0117.jpg) 

- 回路は超手抜きです。LED省略、水晶用負荷コンデンサ省略、レギュレータ省略。５Ｖ動作。
- HIDmonにて**動作確認済み**です。
- 基板の厚みが1mmあるので、mini-Bコネクタのところだけヤスリで薄く削っています。
- mini-Bコネクタは瞬間接着剤で止めています。

<!-- dummy comment line for breaking list -->

- - - -

最小構成の回路図 兼 配線図

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/sch-2313.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/sch-2313.png) 

- 水晶の負荷コンデンサ(22pF x 2)を省略しても大抵は動作しますが、12MHz発振周波数の精度が悪くなります（ずれます）
- USB D+ D-とGND間の青色LED x 2 を省略しても大抵は動作しますが、ホストＰＣによってはUSB認識出来ないことがあります。
- 青色LEDは3.3VツェナーDiの代用品、兼USBバスモニタになります。赤色LEDは順方向電圧が異なるので使用できません。

<!-- dummy comment line for breaking list -->

~

~
~
~
- - - -
- 秋月PIC 18F2550基板に接続して、[PICライター](PICspx.md) として使えます。

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m1.jpg) 

~
~
~
- - - -
- 古い秋月PICプログラマのZIFソケットのみを再利用して、ATtiny2313書き込みアタッチメントを取り付けたところです。

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m2.jpg) 

~
~
~
- - - -
- MINTIAの中身（ISPケーブル配線済み写真）

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m3.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m3.jpg) 


- 部品表

<!-- dummy comment line for breaking list -->
|MINTIA|ケース|1|
|秋月ユニバーサル基板||1|
|USB mini-Bコネクタ||1|
|ATtiny2313|HIDaspxファーム書き込み済み|1|
|水晶|12MHz|1|
|抵抗|2.2kΩ|1|
|抵抗|75Ω|2|
|電解コンデンサ|16v 10uF|1|
|ポリスイッチ|500mA|1|

- 部品をいろいろ端折っているので突っ込みどころはいっぱいあります。

<!-- dummy comment line for breaking list -->

- ファームの更新はMISO/MOSIが反転したケーブルにて普通のAVRライターを繋いで、
- RESET端子をテスター等で電源GNDに繋ぐという荒業を使います。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m4.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m4.jpg) 

~
~
~
- - - -
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m5.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m5.jpg) 

~
~
~
- - - -
- 後ろにあるのは、WS☆Nakさんにて市販されているHIDaspx基板をブレッドボートに刺したものです。

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m6.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m6.jpg) 


~
~
~
- - - -
- ICSPターゲット用tiny2313基板を繋いだところです。

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/t1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/t1.jpg) 

~
~
~
- - - -
- ターゲット基板の詳細

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/t2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/t2.jpg) 　
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/t3.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/t3.jpg) 　
赤外線ビビビ用なので、内蔵ＲＣ８ＭＨｚ最小構成

