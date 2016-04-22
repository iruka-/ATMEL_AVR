[2011-05](2011-05.md) 

**なんだかよくわからない工作シリーズ**


- 秋月Arduino基板（１５０円）を買ったので、作ってみました。
- 上の基板は以前製作したもの。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/akiduino.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/akiduino.jpg) 

- 上：[Arduino400](Arduino400.md) 
- 下：今回製作した、秋月１５０円基板を使用したArduino互換機

<!-- dummy comment line for breaking list -->


- - - -

見たところ、区別が付かない・・・ははｈａ
- 回路的にもファームウェア的にも上下は同じものだったりする。

<!-- dummy comment line for breaking list -->

- 材料費

<!-- dummy comment line for breaking list -->
|秋月基板|150円|
|PIC18F14K50|200円|
|ATmega328P|250円|
|Xtal 12MHzと16MHz|計100円|
|USB Bコネ|50円くらい?|
|各種コネクタとか|まあ150円くらい?|
|その他LEDとかC,R|まあ200円くらい?|
|合計|1100円|

- 確かに基板そのものは安いんだけど、全部あわせるとそれなりに（お金が）掛かる。

<!-- dummy comment line for breaking list -->

- 基板の厚みがやや薄い（1mmあるかないかくらい）。強度的にやや心許ない。シールドをたくさん持っていて抜き差しする人には向かないだろうと断言してあげよう。（まあ、１５０円なんで、許してちょ、みたいなノリ）

<!-- dummy comment line for breaking list -->


- - - -

- ハンズマンなどで売っている超小型アタッシュケース風の入れ物に入れてみた。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/ar1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/ar1.jpg) 

- 実際は基板の幅を0.5mm程度ヤスリで削らないと入らない・・・

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/ar2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/ar2.jpg) 



- Arduino-0022をWindows機にinstall。
- bootloaderは、両方ともoptiboot-328を焼いている。
- どうしてFT232RLをマウントしなかったかって？---手持ちのAE-UM232はあったんだけど、USBコネクタの向きが気に入らなかったんだな。

<!-- dummy comment line for breaking list -->

- boards.txtの選択で、「Arduino Duemilanove or Nano /w mega328」 を選ぶと、スケッチの書き込みに失敗するくせに、「Arduino Uno」を選ぶとうまくいくのはなぜなんだ？

<!-- dummy comment line for breaking list -->


- - - -
- ちなみに上の基板は、Arduinoとピン配置の互換性はゼロであるかわりに、
- ATmega328を抜いて、PIC側ファームを切り替えることで、AVR/PIC/ARM用ライターにもなる。
- わざわざ抜かなくても、これと似たPIC18F14K50基板は５枚くらい持っていたり・・・

<!-- dummy comment line for breaking list -->

- - - -
- せっかくArduinoピン互換の基板が手に入ったので、シールドを物色しようとしたんだけれど、いまひとつ欲しいシールドがなかった。

<!-- dummy comment line for breaking list -->

- **バニラシールド**はちょっと欲しいと思ったけれど、そんな用途なら普通にユニバーサル基板で作れば良かったのかも。

<!-- dummy comment line for breaking list -->

- この基板のそばにブレッドボードを置いて、ジャンパー飛ばしながら回路作るのも一興か・・・。

<!-- dummy comment line for breaking list -->

**あいかわらず本末転倒であった**

~
- ところで、Arduino用の**ブレッドボードシールド**って、ないのかなぁ？

<!-- dummy comment line for breaking list -->

あ、あるんだ・・・
- [http://d.hatena.ne.jp/OkibiWorksLabo/20100504/breadboard](http://d.hatena.ne.jp/OkibiWorksLabo/20100504/breadboard) 

<!-- dummy comment line for breaking list -->

とか
- [http://www.geocities.co.jp/arduino_diecimila/make/proto_shield/index.html](http://www.geocities.co.jp/arduino_diecimila/make/proto_shield/index.html) 

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## PIC18F14K50のusbserialファームの問題点
- どうやらfirmataなスケッチがATmega328に焼かれている状態ではPIC18F14K50が頻繁にハングするようだ。
- Blinkのようなスケッチだと問題ない。
- 今ごろ発覚
- PICのusbserialはRxDから受け取ったデータをPC側に引き取ってもらえない状態(teraterm等が起動していない状態)

<!-- dummy comment line for breaking list -->
だと内部で死んでしまわれるようだ。
- ~~これ直さなきゃだわ。~~ ---> 修正済みです。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## USBserialがファーム内部でハングする問題。

- MicroChip製ライブラリに元からあった問題のようだ。
- RS232c受信のオーバーランを考慮していないコード

<!-- dummy comment line for breaking list -->

	void ProcessIO(void)
	{   
	   //Blink the LEDs according to the USB device status
	   BlinkUSBStatus();
	   // User Application USB tasks
	   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	if (RS232_Out_Data_Rdy == 0)  // only check for new USB buffer if the old RS232 buffer is
	{						  // empty.  This will cause additional USB packets to be NAK'd
		LastRS232Out = getsUSBUSART(RS232_Out_Data,64); //until the buffer is free.
		if(LastRS232Out > 0)
		{	
			RS232_Out_Data_Rdy = 1;  // signal buffer full
			RS232cp = 0;  // Reset the current position
		}
	}

	if(RS232_Out_Data_Rdy && mTxRdyUSART())
	{
		putcUSART(RS232_Out_Data[RS232cp]);
		++RS232cp;
		if (RS232cp == LastRS232Out)
			RS232_Out_Data_Rdy = 0;
	}

	if(mDataRdyUSART())
	{
		USB_Out_Buffer[NextUSBOut] = getcUSART();
		++NextUSBOut;
		USB_Out_Buffer[NextUSBOut] = 0;
	}

	if((USBUSARTIsTxTrfReady()) && (NextUSBOut > 0))
	{
		putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
		NextUSBOut = 0;
	}

	   CDCTxService();
	}		//end ProcessIO


- USB_Out_Bufferは６４バイトしかないのに、

			USB_Out_Buffer[NextUSBOut] = getcUSART();
			++NextUSBOut;
- NextUSBOutが６４以上になる場合を全く考慮していない。

<!-- dummy comment line for breaking list -->

- 実際にはUSB-CDCがPCと接続されて稼動中であれば、1mSごとにRS232Cの受信データを引き取ってくれるので、
- 1mSあたり64byte 、すなわち、64kB/s以下の速度なら、NextUSBOutが64を超えることはないのだろう。

<!-- dummy comment line for breaking list -->

- しかし、PCと接続していないとか、接続していてもCDCドライバーがデータ引取りをやってくれない状態で
- Arduinoから64byte以上のデータが送られてくるとバッファオーバーランで死亡する。

<!-- dummy comment line for breaking list -->

~
    - > 2011/7/10 usbserial-uno.zipを修正済みです。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 参考になる。

秋月ATMEGA168/328用IOボードでArduino
- [http://www.geocities.co.jp/arduino_diecimila/make/ae-atmega/index.html](http://www.geocities.co.jp/arduino_diecimila/make/ae-atmega/index.html) 

<!-- dummy comment line for breaking list -->

- どうせ150円だし、基板厚が薄いので、AE-UM232部分を切ってしまって使うのもありかもしれない。

<!-- dummy comment line for breaking list -->


~
- その場合は、Classicなシリアル通信タイプのArduinoと割り切って使う方法と、
- 以下のようにMetaBoard互換の回路を付加して、UM232要らずでUSBを使う方法もある。
    - [http://www.geocities.co.jp/arduino_diecimila/make/usbasp/index.html](http://www.geocities.co.jp/arduino_diecimila/make/usbasp/index.html) 

<!-- dummy comment line for breaking list -->

~

秋月電子製AE-ATmega基板の改造 - COMOPYさん
- [http://elec.d-shell.jp/COMOPYx_Site/Mod-AE-ATmega.html](http://elec.d-shell.jp/COMOPYx_Site/Mod-AE-ATmega.html) 
- ATmega8u2を実装されています。写真がとても綺麗。

