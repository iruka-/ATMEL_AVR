*PIC18FによるHID genericなmonitor [#eb7ddb50]

元ねた
[mcc18](mcc18.md) 　[usbserial](usbserial.md) 　[pic18hidkey](pic18hidkey.md) 


	■ 概要

	 これは、MicroChipが提供している PIC18 のUSBアプリケーションフレームワーク
	 に含まれているHID-Bootloader をもとにして作成した PIC18F専用の簡易モニタです。

	■ 現在のステータス

	 PIC 18F2550上で動作します。

	 まず、hidmon-2550を使用してファームウェアを書き込みます。
	 C:> picboot -r firmware/picmon-18F2550.hex

	 次に、picmonitor を起動します。
	 C:> cd host
	 C:host> picmonit.exe 

	 現在実装されているのは HIDCMD_PEEK と POKEだけです。
	TARGET DEV_ID=25
	PIC> d
	000000 00 00 40 00 20 78 02 00
	000008 08 41 22 68 05 00 07 20
	000010 bf 04 02 80 ac 40 84 41
	000018 5d 44 88 83 05 03 c4 41
	000020 fe 20 85 84 0c e9 24 cb
	000028 36 50 8c 22 22 f8 51 21
	000030 04 44 25 dc 02 c0 31 14
	000038 81 09 48 d4 43 10 d6 35
	PIC> l 0
	0000 6af7        clrf   0xf7, 0
	0002 6af8        clrf   0xf8, 0
	0004 d054        bra    0xae
	0006 d330        bra    0x668
	0008 ef04 f004   goto   0x808
	000c d05d        bra    0xc8
	000e d27b        bra    0x506
	0010 d328        bra    0x662
	0012 d32c        bra    0x66c
	0014 d32f        bra    0x674
	0016 d345        bra    0x6a2
	0018 ef0c f004   goto   0x818
	001c 0112        movlb  0x12
	001e 0200        mulwf  0, 0
	0020 0000        nop
	0022 4000        rrncf  0, 0, 0
	0024 04d8        decf   0xd8, 0, 0
	0026 003c        dw     0x3c  ;unknown opcode
	PIC> q
	Bye.
	
	C:>

	==================================================
	■ hidmon-14k50 や hidmon-2550との違い

	・一応、Ｃ言語で書かれています。(mcc18)

	・コードサイズが大きいです。(4kB弱)

	・USB のPIDが違います。

	・プロトコルも違います。

	・HID Reportの転送方法が異なります。
	  hidmonでは、全てコントロール転送(HidD_SetFeature/HidD_GetFeature)
	  で行っていますが、picmonit.exeでは、EndPoint1に対するインタラプト転送
	  によりデータの受け渡しを行っています。

	■ 何に使えますか？

	・picmonit.exe 上のコマンドと、firmware上のコマンド受け取り処理の対を
	　自分で追加することにより、いろいろな機能を追加することが出来ます。



	■ 応用１
	  -------------------------------------------------------------------------
	  PS/2 タイプのキーボードを繋いで、打鍵されたキーコードを１６進ダンプします
	  -------------------------------------------------------------------------

	  接続：
	    
	    PORTB.bit0 <---- PS/2 KeyBoard CLK
	    PORTB.bit1 <---- PS/2 KeyBoard DATA

	  使い方：
	    （１）firmware/picmon-18F2550.hex を焼きます。
	    （２）ＰＣ上にて、picmon/picmonit.exe を起動します。

	    普通にメモリーダンプ('D'コマンド)や逆アセンブル('L'コマンド)が使えることを
	    確認します。
	    
	    （３）userコマンドを入力します。

	    PIC> user
	    
	      そして、繋いだPS/2キーボードを打鍵すると、コンソールに１６進でコード表示が
	      出ます。
	      
	      ８０個のコードが表示されるか、もしくはパソコン側から[ESC]キーを押したら終了です。


	■ 応用２
	  -------------------------------------------------------------------------
	  PIC上からprintfやputsを試せます。
	  -------------------------------------------------------------------------

	  firmware/Makefile に記述している OBJ項の keybtest.o を usercmd.o と置き換えます。
	  firmwareディレクトリで make clean して make します。
	  firmware/picmon-18F2550.hex を焼きます。
	  ＰＣ上にて、picmon/picmonit.exe を起動します。
	  そしてuserコマンドを入力します。
	    PIC> user
	  これで、PIC上からprintされた文字をＰＣ上で見ることが出来ます。
	  
	  usercmd.c をいろいろ書き換えて試すことが出来ます。



- - - -
**ダウンロード**（usercmdを有効にした新バージョン）
- [pic18spx-2010-0416.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18spx-2010-0416.zip) 

<!-- dummy comment line for breaking list -->


**ダウンロード**（ps2keyboardを接続する旧バージョン）
- [picmonitor.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/picmonitor.zip) 

