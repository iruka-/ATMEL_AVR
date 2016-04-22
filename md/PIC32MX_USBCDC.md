[PIC32MX](PIC32MX.md) 



## PIC32MXで仮想シリアルポートを実装してみる。


**２通りの方法があります**

- １つめは、Pinguinoのお手軽スケッチをそのまま使う方法です。

<!-- dummy comment line for breaking list -->

- もうひとつは、MicroChipのUSB-CDCサンプルを使います。

<!-- dummy comment line for breaking list -->

~
## Pinguinoのスケッチを使用してエコーバックサンプル。

- スケッチを使用するといっても、pde拡張子のままではROMに焼いたりいろいろ改造できませんので、

<!-- dummy comment line for breaking list -->
例によって、Pinguino環境でMakefileその他を用意してコマンドライン開発する環境に移します。


- user.c

		unsigned int counter=0;
		void setup()
		{
		pinmode(13, OUTPUT);
		}
		#
		void loop()
		{
		static int cnt=0;
		static char buffer[256];
		int numBytesRead = USB_Service_CDC_GetString( buffer );
		if(numBytesRead&gt;0) {
			CDCputs(buffer,numBytesRead);
		}
		cnt++;
		if(	cnt &gt;= 500000 ) {
			cnt=0;
			toggle(13);
		}
		}
- ダウンロード:[cdcecho.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcecho.zip) 

<!-- dummy comment line for breaking list -->

- こうして出来た仮想シリアルポートのエコーバックサンプルに対して、規則的な繰り返しの文字データを送信し、同時に受信を行なって、その時間を計ります。（ベンチマーク）

<!-- dummy comment line for breaking list -->

	E:\cdcecho>w32term -p
	==== COM Port List====
	COM50:TxQue=0,RxQue=16384,Baud=0x66b70,BaudMax=0x10000000,Bits=0xc,SubType=6,Capa=cd
	COM1:TxQue=0,RxQue=4096,Baud=0x1006ffff,BaudMax=0x10000000,Bits=0xf,SubType=1,Capa=ff
	
	E:\cdcecho>w32term -c50 -t
	Entering Benchmark: Escape=^A
	..........................................................................................
	..........................................................................................
	......................................................................
	Benchmark end. 32000 bytes/812 ms,  39408 bytes/s (394080 bps)
	
	E:\cdcecho>

- 毎秒４０ｋＢのスループットになっていることがわかります。ボーレートでは(start/stop bitを加味すると)394kBPSです。
- ちょっと遅いですね。

<!-- dummy comment line for breaking list -->

## ---考察します。

- Pinguinoライブラリのcdcサンプルがどのような実装になっているかはあくまで推測です。
- 毎秒40kBしか送り返せない、ということは、USBの1フレーム時間(1mS)に40byteしか返送されていません。
- FullSpeed(12Mbps)USBのパケット長は64byteなので、１フレーム時間にわずか1パケットしか返送されていません。
- しかも64byteではなくて40byteです。

<!-- dummy comment line for breaking list -->

~
- これはおそらく、返送ルーチンがバイト指向になっていて、上限が40byteに制限されているのではないか、と推察します。

<!-- dummy comment line for breaking list -->

- 以前にもARMのmapleライブラリのcdc実装がそれに似た感じになっていました。
- FullSpeed USBの性能としては、１フレーム時間に少なくとも10パケット程度は送出できます。
- これが、なぜ1パケットに制限されるか、というと、それはCDC ACMクラスのWindowsドライバーがそうなっているからで、１回のシリアルデータのバルク転送パケットのサイズが64byte未満だった場合は、後続データ無しとみなして、その１フレーム時間(1mS)内には次のパケットを受け取らないという仕様になっているからです。

<!-- dummy comment line for breaking list -->

~
- USB-CDCデバイスでせっかくバルク転送が使用できるのに、HIDデバイス未満の転送速度しか得られないというのは**なんだか納得がいきません**

<!-- dummy comment line for breaking list -->

~
## ---次は、MicroChipのUSB-CDCサンプルを使って試してみます。

- ソースは長いのですが、一応肝の部分だけ抜粋します。

		void ProcessIO(void)
		{
		BYTE numBytesRead;
		BlinkUSBStatus();
		if((USBDeviceState &lt; CONFIGURED_STATE)||(USBSuspendControl==1)) return;
		if(buttonPressed) {
			if(stringPrinted == FALSE) {
				if(mUSBUSARTIsTxTrfReady()) {
					putrsUSBUSART("Button Pressed -- \r\n");
					stringPrinted = TRUE;
				}
			}
		} else {
			stringPrinted = FALSE;
		}
		if(USBUSARTIsTxTrfReady()) {
			numBytesRead = getsUSBUSART(USB_Out_Buffer,64);
			if(numBytesRead != 0) {
				memcpy(USB_In_Buffer,USB_Out_Buffer,64);
				putUSBUSART(USB_In_Buffer,numBytesRead);
			}
		}
		CDCTxService();
		}
- 実際の応答部分は、最後の８行分のところだけです。
- こちらのソースでは、受け取ったUSBパケットをそっくりそのままWindowsに返送しています。
- なので、１フレーム時間(1mS)に複数個のパケット(64byteパケットが連続して、最後に64byte未満のパケットが来る)が送られてきてもそれをそっくりそのまま返送するので効率がよいのです。
- （もちろん、バイト指向で再組み立てする方法でも64byte未満パケットに分断さえしなければスループットは低下しません。）

<!-- dummy comment line for breaking list -->

~
ビルド
- Pinguinoコンパイラでビルドしますが、MchipのヘッダーとMchipのUSBアプリケーションフレームワークが必要です。

<!-- dummy comment line for breaking list -->

## ダウンロード
- [cdcscope.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcscope.zip) 

<!-- dummy comment line for breaking list -->
~
- このcdcscope.zipで生成されるHEXファイルはBOOTLOADERからの書き込みとPicKit2からの書き込みの両方に対応しています。
- こういうふうになっています。

<!-- dummy comment line for breaking list -->

	BFC0_0000+---------------+
	         | Jmp _start    | <=== RESETで最初に実行される命令
	         |               |
	         |               |
	         |BOOTLOADERの一部|
	         +---------------+
	
	9D00_0000+---------------+
	         | BOOTLOADER    | BOOTLOADERに重なっている部分はアプリでは使用しません。
	         |               |
	         |               |
	         |               |
	9D00_2000+---------------+
	(_reset) | Jmp _start    | <=== BOOTLOADERからの起動により最初に実行される命令
	         | ============= |
	         | 割り込みVECTOR|
	         | ============= |
	         |_start:初期化部|
	         |               |
	         |   アプリ本体  |
	         |               |
	9D00_7FFF+---------------+
- BFC0_0000からの16バイトだけが、BOOTLOADERとアプリケーションで重複しますが、
- BOOTLOADERは9D00_2000〜9D00_7FFFの範囲内のみイレース、書き換えを行ない、それ以外の領域のHEXレコードを無視しますので、スタンドアローン、ブートローダー起動のどちらでも正しく起動します。

<!-- dummy comment line for breaking list -->

~
~
~






~
## ベンチマーク結果
	E:\cdcscope>w32term -c50 -t
	Entering Benchmark: Escape=^A
	..........................................................................................
	..........................................................................................
	......................................................................
	Benchmark end. 32000 bytes/141 ms,  226950 bytes/s (2269500 bps)
	
	E:\cdcscope>
- スループットは毎秒226kB (実効ボーレートで2.26M BPS)になりました。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## USB - シリアル変換器 (Serial Emulator) を作る。
- FTDI FT232RのようなUSB-シリアル変換器を作成します。
- これは、MicroChipのUSBフレームワークのCDC Serial EmulatorをビルドすればＯＫです。
- MX220F032B用に移植したものを用意しました。

<!-- dummy comment line for breaking list -->

**ダウンロード**
- [cdcserial.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcserial.zip) 

<!-- dummy comment line for breaking list -->

- ブートローダー起動、PicKit2ライター直接書き込みの両対応HEXになっています。
- Pinguinoのmips-gccとコマンドライン版Make(Pinguino付属)でビルドします。

<!-- dummy comment line for breaking list -->

~
## 移植の手順についての説明
（１）チップ品種の変換
- HardwareProfile.h というヘッダーが各品種ごとのヘッダーを選択的にincludeするようになっていますので、

		HardwareProfile_MX220F032B.h
- というヘッダーを用意して、それをインクルードするように書き換えます。
- HardwareProfile_MX220F032B.hヘッダーにはMX220F032Bの自作基板にあわせたピン配置の定義を書きます。

<!-- dummy comment line for breaking list -->

~
（２）crt0.Sの用意
- メモリー配置をブートローダー起動、PicKit2ライター直接書き込みの両対応にするためと、メモリー節約のために、少し改造したcrt0.Sを用意しています。
- これは、ramfuncのコピーを省略してサイズを0x180 byte以下に抑え、割り込みベクターの手前に収まるサイズにしたものです。
- ついでに、割り込みベクターから、割り込みハンドラーへのJmp命令も書き足しています。

<!-- dummy comment line for breaking list -->

~
（３）MakefileとObjs.makの用意。
- Objs.makはリンクしたいモジュールを列挙したものです。

<!-- dummy comment line for breaking list -->

~
（４）main.cの改造
- MchipのサンプルコードはPIC18FやLowPinCountPIC,PIC24F,dsPIC33など数多くの品種をカバーしているために、#ifdef〜#endifの嵐になっています。
- これでは、あまりに見通しが悪いので、PIC32MX以外の記述は思い切って削除します。
- つぎに、main()関数の先頭、かつ割り込みベクター設定があればその直後にそれを上書きする形で下記の記述を追加します。

		_CP0_SET_EBASE(_RESET_ADDR); 	// 割り込みベクターを9D00_2000 に変更する.
- _RESET_ADDR はリンカースクリプトから供給されるシンボルラベルです。

<!-- dummy comment line for breaking list -->

- 最後に、USBをポーリング実装でコンパイルするときに消滅する関数_USB1Interrupt()を補うための記述を入れます。

		#if defined(USB_POLLING)
		// for crt0.S vector dummy;
		// Dummy Function (Never Called)
		//
		void __attribute__((interrupt,nomips16,noinline)) _USB1Interrupt(){}
		#endif
- これはcrt0.SからJmp命令で飛んでくる関数ですが、ポーリングモードでは実在しませんので、ダミーとしてここに置きます。実際には割り込みませんのでここに飛んでくることはないです。
- （crt0.Sを毎回書き換えるのが面倒なのでＣレベルで対処）

