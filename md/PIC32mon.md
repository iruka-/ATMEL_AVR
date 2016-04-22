関連リンク　[Pinguinoで遊ぼう](Pinguino.md) 　[ブートローダーを作る](HIDBootX.md) 　[USB仮想シリアル](PIC32MX_USBCDC.md) 　[USBカスタムデバイス](USB_CUSTOM.md) 　[USBホスト](USB_HOST.md) 　[2013-02](2013-02.md) 　[2013-03](2013-03.md) 



## PIC32MXでUSB高速カスタムデバイスを実装し、Windows(WinUSB.DLL)でチップを制御します。


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg) 

- シンプルなUSBバルクデバイスを用意して、それを介してPIC32MX用の簡易モニターを実装してみます。
- Windowsホスト側は、WinUSB.dllを呼び出してデバイスを制御します。

<!-- dummy comment line for breaking list -->

**ダウンロード**
- [pic32mon_adc.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic32mon_adc.zip) 

<!-- dummy comment line for breaking list -->

- ビルドには、Pinguino-gccを、Windowsホスト側はMinGW32-gccを用います。

<!-- dummy comment line for breaking list -->

- デバイス上のメモリーダンプ、メモリー書き換え、逆アセンブル、Ｉ/Ｏポートの参照、書き換えが可能です。

<!-- dummy comment line for breaking list -->


- 実行例

		C:\Documents and Settings\&gt;pic32mon
		MIPS&gt; d ffff
		* PIC32MX220F032B Memory Map
		-------------:-----------------------
		BOOT-ROM(3k) : BFC0_0000 ~ BFC0_0BFF
		FLASH  (32k) : 9D00_0000 ~ 9D00_7FFF
		S-RAM   (8k) : A000_0000 ~ A000_1FFF
		PORT         : BF80_0000 ~ BF88_FFFF
		-------------:-----------------------
		Need Help ? &gt;  please type help
		MIPS&gt; l 1fc00000
		1fc00000 3c1a9fc0       lui     k0,0x9fc0
		1fc00004 275a0010       addiu   k0,k0,16
		1fc00008 03400008       jr      k0
		1fc0000c 00000000       nop
		1fc00010 401a6000       mfc0    k0,$12
		1fc00014 7f5a04c0       ext     k0,k0,0x13,0x1
		1fc00018 13400002       beqz    k0,$1fc00024
		1fc0001c 00000000       nop
		1fc00020 00000000       nop
		1fc00024 3c1da000       lui     sp,0xa000
		1fc00028 27bd2000       addiu   sp,sp,8192
		1fc0002c 3c1ca001       lui     gp,0xa001
		1fc00030 279c8000       addiu   gp,gp,-32768
		1fc00034 40096002       mfc0    t1,$12,2
		1fc00038 01205820       add     t3,t1,zero
		1fc0003c 7d2a1e80       ext     t2,t1,0x1a,0x4
		1fc00040 7d494984       ins     t1,t2,0x6,0x4
		1fc00044 40896002       mtc0    t1,$12,2
		1fc00048 41dce000       wrpgpr  gp,gp
		1fc0004c 408b6002       mtc0    t3,$12,2
		MIPS&gt; d a0000000
		a0000000 12 03 4d 00 53 00 46 00  54 00 31 00 30 00 30 00
		a0000010 ee 00 00 00 28 00 00 00  00 01 04 00 01 00 00 00
		a0000020 00 00 00 00 00 01 57 49  4e 55 53 42 00 00 00 00
		a0000030 00 00 00 00 00 00 00 00  00 00 00 00 8e 00 00 00
		a0000040 00 01 05 00 01 00 84 00  00 00 01 00 00 00 28 00
		a0000050 44 00 65 00 76 00 69 00  63 00 65 00 49 00 6e 00
		a0000060 74 00 65 00 72 00 66 00  61 00 63 00 65 00 47 00
		a0000070 55 00 49 00 44 00 00 00  4e 00 00 00 7b 00 35 00
		a0000080 38 00 64 00 30 00 37 00  32 00 31 00 30 00 2d 00
		a0000090 32 00 37 00 63 00 31 00  2d 00 31 00 31 00 64 00
		a00000a0 64 00 2d 00 62 00 64 00  30 00 62 00 2d 00 30 00
		a00000b0 38 00 30 00 30 00 32 00  30 00 30 00 63 00 39 00
		a00000c0 61 00 36 00 36 00 7d 00  00 00 00 00 d6 0b 00 a0
		a00000d0 00 0c 00 a0 00 00 00 00  00 00 00 00 00 00 00 00
		a00000e0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
		a00000f0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
		MIPS&gt; p
		          TRISA(0xbf886010) 0x0000001f 00000000_00000000_00000000_00011111
		          PORTA(0xbf886020) 0x00000010 00000000_00000000_00000000_00010000
		           LATA(0xbf886030) 0x00000008 00000000_00000000_00000000_00001000
		          TRISB(0xbf886110) 0x00002fbf 00000000_00000000_00101111_10111111
		          PORTB(0xbf886120) 0x000001b0 00000000_00000000_00000001_10110000
		           LATB(0xbf886130) 0x00004b16 00000000_00000000_01001011_00010110
		MIPS&gt; q
		Bye.
- mips16命令を逆アセンブルしたい場合は、アドレスのLSBを1にして（つまり１を足して）番地指定します。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## A/D変換を行なって結果をWindowsのコンソールに表示する。

- ユーザー関数

		void user_cmd(int arg)
		{
		int i,j;
		analog_init();
		//
		//	printf()テストの実行.
		//
		for(i=0;i&lt;100;i++) {
			j=analogRead(APIN);
			printf("j=%d\n",j);
		}
		}
- ユーザー関数の実行

		D:&gt; pic32mon.exe
		MIPS&gt; user
		j=285
		j=272
		・・・
		MIPS&gt; q
		D:&gt;
- ユーザー関数は自由に作成できます。

<!-- dummy comment line for breaking list -->


~
~
~

~
## USB簡易モニターを使って、対話的にポートアクセスを試してみる。
	D:> pic32mon.exe
	TARGET DEV_ID=22 VER=1.1(Application) FLASH=9d006200,8000
	MIPS> p
	          TRISA(0xbf886010) 0x0000001f 00000000_00000000_00000000_00011111
	          PORTA(0xbf886020) 0x00000010 00000000_00000000_00000000_00010000
	           LATA(0xbf886030) 0x00000008 00000000_00000000_00000000_00001000
	          TRISB(0xbf886110) 0x00002fbf 00000000_00000000_00101111_10111111
	          PORTB(0xbf886120) 0x000001b0 00000000_00000000_00000001_10110000
	           LATB(0xbf886130) 0x000061b9 00000000_00000000_01100001_10111001
	MIPS>
- **p**コマンドを打つと、主要なポートレジスタ(SFR)の値を１６進/２進数で表示してくれます。
- ポートの値を書き換えたい場合は、以下のようにします。

		MIPS&gt; p TRISBset 8000
		       TRISBSET(0xbf886118) 0x00000000 00000000_00000000_00000000_00000000
- **p TRISB afbf**という風に直接１６進数でPORTに書き込む方法もありますが、
- ここでは、TRISBset という、PORTの値を読んで、OR書きしてくれる便利なポート(TRISBset)にMSBだけ立てるコマンドを書き込みます。すなわち、

		TRISB = TRISB or 0x8000;
- をTRISBsetレジスタに書き込むことで実行してくれます。（このOR書きポートはPIC32MXのsfrに存在する機能です。）

<!-- dummy comment line for breaking list -->

~
- これでLED端子が出力モードから入力モードに切り替わるので、LED点滅が消えます。

<!-- dummy comment line for breaking list -->

- 元に戻すには、

		MIPS&gt; p TRISBclr 8000
- と、します。TRISBの値がどうなったか知りたいならいつでも

		MIPS&gt; p TRISB
- とします。(pだけでもOKです)

<!-- dummy comment line for breaking list -->

~
次です。
- PIC32MXには、PPS(Peripheral Pin Select)という機能があり、例えばSPI機能の各ピンを、ある程度の自由度でいくつかのPORTのどれかを選んで動的に割り当てることが可能です。
- それを行なうレジスタの名称は、RPBxxR(xxは数字)というものがあります。
- まず、RPB　で始まるsfrレジスタを列挙します。

		MIPS&gt; p rpb
		          RPB0R(0xbf80fb2c)
		          RPB1R(0xbf80fb30)
		          RPB2R(0xbf80fb34)
		          RPB3R(0xbf80fb38)
		          RPB4R(0xbf80fb3c)
		          RPB5R(0xbf80fb40)
		          RPB7R(0xbf80fb48)
		          RPB8R(0xbf80fb4c)
		          RPB9R(0xbf80fb50)
		         RPB10R(0xbf80fb54)
		         RPB11R(0xbf80fb58)
		         RPB13R(0xbf80fb60)
		         RPB14R(0xbf80fb64)
		         RPB15R(0xbf80fb68)
- 次に、RPB4Rのレジスタの値を確認します。

		MIPS&gt; p rpb4r
		          RPB4R(0xbf80fb3c) 0x00000000 00000000_00000000_00000000_00000000
- このレジスタに書き込む値については、PIC32MX 1xx/2xxのマニュアルを参照願います。
- 例えば

		MIPS&gt; p rpb4r 1
- という値を書き込むと、PB4はU1TXの出力が割り当てられます。

		MIPS&gt; p ?
- と打ち込むと、全部のsfrを列挙してくれます。
- **?** の代わりに、うろ覚えのポート名称の最初の数文字を打ち込むと、マッチしたポート名をその番地とともに列挙してくれるというわけです。

<!-- dummy comment line for breaking list -->

~
## インタラクティブな開発方法。
- user_cmd()関数に簡単なスケッチのようなものを書いて、ビルドします。
- ブートローダー(HIDBoot.X)が起動している場合は、そのまま w.bat バッチコマンドでプログラムを書き込みます。
- PIC32mon（このファームウェア）が起動している場合は、以下のようなバッチファイルを書いて、プログラムを書き込みます。
- **go.bat**

		hostpc\pic32mon -iboot.cmd
		sleep 2
		hex2dump -f main32.hex boot.hex
		mphidflash -r -n -w boot.hex
- **boot.cmd**は、以下の１行のみでOKです。

		boot
- プログラムを書き込んで実行したら、

		hostpc\pic32mon.exe
- を起動して、

		MIPS&gt; user
- でuser_cmd()関数を実行します。
- user_cmd()関数内で結果をprintするか、あるいは、実行終了後、

		MIPS&gt; p ＜ポート名称＞
- あるいは

		MIPS&gt; d ＜メモリー番地＞
- などにて、所定の結果を得ます。

<!-- dummy comment line for breaking list -->



~
~




~





~
~
~

~

