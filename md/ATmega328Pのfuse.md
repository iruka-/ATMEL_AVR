[2009-10](2009-10.md) 



- - - -
## ATmega328P用のブートローダー作成方法

（１）改造：以下のファイルを少しだけ編集
- arduino-0017/hardware/bootloaders/atmega/Makefile

		#atmega328: CFLAGS += '-DMAX_TIME_COUNT=F_CPU&gt;&gt;4' '-DNUM_LED_FLASHES=1' -DBAUD_RATE=57600
		atmega328: CFLAGS += '-DMAX_TIME_COUNT=F_CPU&gt;&gt;3' '-DNUM_LED_FLASHES=1' -DBAUD_RATE=57600
- つまり、MAX_TIME_COUNTをオリジナルの２倍にします。

<!-- dummy comment line for breaking list -->

（２）Make

- avr-gccのPATHを追加してmakeします。

		D:&gt; set PATH=%PATH%;D:\arduino-0017\hardware\tools\avr\bin;D:\arduino-0017\hardware\tools\avr\utils\bin
		D:&gt; cd \arduino-0017\hardware\bootloaders\atmega
		D:&gt; make clean
		D:&gt; make atmega328
- （３）出来たHEXをhidaspxでATmega328Pに焼きます。
- w.bat

		hidspx ATmegaBOOT_168_atmega328.hex
		hidspx -fl11111111
		hidspx -fh11011010
		hidspx -fx101
- これで、[Arduino400](Arduino400.md) 風のUSB-PIC経由によるArduino互換機でスケッチのuploadが可能になりました。

<!-- dummy comment line for breaking list -->

~

#### 応用

- AVRチップにATmega88やATmega168を使用する場合にも、
- MAX_TIME_COUNTの値を調整することでRESET後のタイムアウト時間（アプリ起動までのブート待機時間）を短くすることが出来るようです。

<!-- dummy comment line for breaking list -->

- (FT232RLの)DTR#とAVRのRESET間の0.1uFコンデンサと、RESETプルアップ抵抗(10kΩ)、それからRTSが接続されている場合はその抵抗（コンデンサ？）などの合成によるリセット時定数の違いにより、リセット後のスケッチ書き込みがうまくいかないときは
- Makefileの'-DMAX_TIME_COUNT='の項を調整してみてください。

<!-- dummy comment line for breaking list -->

~

~
- - - -
## ATmega328Pのfuseに関する驚愕の事実！

**ATmega328PのfuseビットはATmega88P/168Pと異なっています。**

- 拡張fuseのbit2:0と、High fuseのbit2:0が、（mega168Pと比べると）入れ替わっています。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/mega328f1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/mega328f1.png) 

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/mega328f2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/mega328f2.png) 

