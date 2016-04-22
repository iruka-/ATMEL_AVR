[2013-12](2013-12.md) 

関連リンク　[PIC32MX]]　[[Pinguinoで遊ぼう](Pinguino.md) 　[ブートローダーを作る](HIDBootX.md) 　[USB仮想シリアル](PIC32MX_USBCDC.md) 　[USBカスタムデバイス](USB_CUSTOM.md) 　[USBホスト](USB_HOST.md) 　[2013-02](2013-02.md) 　[2013-03](2013-03.md) 



- - - -
## ブートローダーって、何？

[ブートローダーを作る](HIDBootX.md) を参照のこと。　

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg) 





## ブートローダーのための開発環境を用意する。


Pinguino Rev4 easyPackだけで完結します。(MicrochipのIDE環境やCygwinなどは不要です。)

#### [Pinguino4.X](http://wiki.pinguino.cc/index.php/Main_Page/ja)
- ダウンロード: [http://wiki.pinguino.cc/index.php/Category:IDE_Downloads](http://wiki.pinguino.cc/index.php/Category:IDE_Downloads) 
    - アーカイブを入手して、 7zアーカイバーで（D:\などに）展開します。
    
    		D:\x4-easy-rev999\
    - ~
- Windows側のプログラムのビルドには MinGW32 を使用します。 (www.mingw.org)

<!-- dummy comment line for breaking list -->

~
## ブートローダーのソースを入手する。

- **ダウンロード:**[uartboot.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/uartboot.zip) 

<!-- dummy comment line for breaking list -->

    - 追記：2014-0927: PIC32MX250 (128kB Flash) 対応しました。

<!-- dummy comment line for breaking list -->

~
## ブートローダーをコンパイルしてみる。
- 上記アーカイブはすでにPinguino用gccでビルド出来るようにしています。
- Pinguino用gccに実行パスを通します。(setenv.bat)

		PATH D:\x4-easy-rev999\win32\p32\bin;%PATH%
- ~
- *ブートローダーをチップに書き込む。 [#e03f1892]
- \uartboot\ ディレクトリでmakeした後、w.batを実行。

<!-- dummy comment line for breaking list -->

~
~
## ブートローダーを使ってPIC32MXの中を覗いてみる。
- uartboot のファームウェアにはPIC32MXのメモリーを読み書きするコマンドが含まれています。
- これを利用すると、PIC32MXの中を簡単に覗くことができます。
- やってみましょう。

<!-- dummy comment line for breaking list -->

- uartboot/hostpc/uartmon32.exe を起動します。

<!-- dummy comment line for breaking list -->

	MIPS> d a0000000
	a0000000 00 00 00 00 01 00 00 00  04 03 09 04 e4 16 00 9d
	a0000010 20 04 00 a0 30 04 00 a0  00 00 00 a0 00 00 db 13
	a0000020 01 01 00 00 ff ff ff ff  00 80 00 1d 20 00 00 00
	a0000030 38 00 00 a0 00 00 00 00  01 01 07 05 01 03 40 00
	a0000040 00 00 00 00 20 00 00 00  00 01 00 00 10 04 00 a0
	a0000050 38 04 00 a0 01 00 00 00  01 00 00 00 01 00 00 00
	a0000060 00 00 00 00 20 04 00 a0  00 00 00 00 08 04 00 a0
	a0000070 00 00 00 00 00 00 00 00  80 06 00 02 00 00 29 00
	a0000080 00 00 02 00 00 04 00 a0  01 00 00 00 00 00 00 00
	a0000090 14 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000a0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000b0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000c0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000d0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000e0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000f0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	MIPS> l bfc00000
	bfc00000 3c1a9fc0       lui     k0,0x9fc0
	bfc00004 275a0010       addiu   k0,k0,16
	bfc00008 03400008       jr      k0
	bfc0000c 00000000       nop
	bfc00010 401a6000       mfc0    k0,$12
	bfc00014 7f5a04c0       ext     k0,k0,0x13,0x1
	bfc00018 13400002       beqz    k0,$bfc00024
	bfc0001c 00000000       nop
	bfc00020 00000000       nop
	bfc00024 3c1da000       lui     sp,0xa000
	bfc00028 27bd2000       addiu   sp,sp,8192
	bfc0002c 3c1ca001       lui     gp,0xa001
	bfc00030 279c8000       addiu   gp,gp,-32768
	bfc00034 40096002       mfc0    t1,$12,2
	bfc00038 01205820       add     t3,t1,zero
	bfc0003c 7d2a1e80       ext     t2,t1,0x1a,0x4
	bfc00040 7d494984       ins     t1,t2,0x6,0x4
	bfc00044 40896002       mtc0    t1,$12,2
	bfc00048 41dce000       wrpgpr  gp,gp
	bfc0004c 408b6002       mtc0    t3,$12,2
	MIPS> p
	          TRISA(0xbf886010) 0x0000001f 00000000_00000000_00000000_00011111
	          PORTA(0xbf886020) 0x00000010 00000000_00000000_00000000_00010000
	           LATA(0xbf886030) 0x00000008 00000000_00000000_00000000_00001000
	          TRISB(0xbf886110) 0x00002fbf 00000000_00000000_00101111_10111111
	          PORTB(0xbf886120) 0x000001b0 00000000_00000000_00000001_10110000
	           LATB(0xbf886130) 0x00008b16 00000000_00000000_10001011_00010110
	MIPS>q
- 逆アセンブル、メモリーダンプ、ポートの状態監視が出来ます。
- mips16の命令列を逆アセンブルしたい場合は、l（エル)コマンドの引数に与える開始番地のLSBを立ててください。（奇数番地にします）-- ARM<->Thumbと同様です。

<!-- dummy comment line for breaking list -->

- ポートアドレスをポート名から知りたいときは、その名称の途中までをタイプすると教えてくれます。

		MIPS&gt; p port
		          PORTA(0xbf886020)
		       PORTACLR(0xbf886024)
		       PORTASET(0xbf886028)
		       PORTAINV(0xbf88602c)
		          PORTB(0xbf886120)
		       PORTBCLR(0xbf886124)
		       PORTBSET(0xbf886128)
		       PORTBINV(0xbf88612c)
- ポートアドレスの全一覧は、

		MIPS&gt; p ?
- ポート内容一覧は、

		MIPS&gt; p *
- という具合です。

<!-- dummy comment line for breaking list -->



- - - -
## Pinguinoのブートローダーとして使うには

- mphidflash.exe の代わりに uartflash32.exe を呼び出すようにしてください。
- （もっとも単純な手段としては、exeファイルを上書きすることです。）

<!-- dummy comment line for breaking list -->






~
~
~
~
- - - -
## 参考リンク

以下のリンクは両方とも熟読すること。でないとMIPSとかさわっちゃだめ。
#### とても参考になる。

『はじめて読む MIPS(リローデッド)』 by 中森章 - CQ出版社 
- www.cqpub.co.jp/interface/TechI/Vol39/app/mips_asm.pdf

<!-- dummy comment line for breaking list -->

PIC32ファミリ　リファレンスマニュアル　ＣＰＵ（日本語）
- [http://ww1.microchip.com/downloads/jp/DeviceDoc/61113D_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61113D_JP.pdf) 

