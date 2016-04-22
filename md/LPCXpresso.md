[ARM](ARM.md) 　[2010-08](2010-08.md) 　[lpc-armon](lpc-armon.md) 　[LPCXpresso1768](LPCXpresso1768.md) 

- - - -

LPCXpresso(LPC3154+LPC1343) のターゲット基板のみを使って、**USBデバイス作成**を実験してみました。

- まずは手始めに、一番簡単な**HIDデバイス**です。
- 今回は、eclipse IDEなどを一切使用せず、**arm用のgccとmakeだけ**でビルドしました。

<!-- dummy comment line for breaking list -->


- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/xpresso2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/xpresso2.jpg) 

<!-- dummy comment line for breaking list -->

LPCXpressoは秋月電子にて２,８００円で入手可能です。
- [http://akizukidenshi.com/catalog/g/gM-03598/](http://akizukidenshi.com/catalog/g/gM-03598/) 

<!-- dummy comment line for breaking list -->


~
~
~

- - - -

■ 概要

	 これはNXPのLPCXpresso (LPC1343) ターゲットの簡単なHIDデバイスサンプルです。

	 内容は、ほぼExamplesのままですが、通常のMakefileでビルドできるように再構成
	 してあります。

	 特筆すべき点は、わずか 404バイト(10進)でHIDデバイスとして機能することです。

- - - -

■ ハードウェアの準備



基板分離:
	  （１）LPCXpresso のLPC-Link側とターゲット側基板を切り離します。

ＵＳＢ配線:

- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/lpc1343lqfp48.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/lpc1343lqfp48.png) 

<!-- dummy comment line for breaking list -->

	  （２）ターゲットLPC1343基板の USB-DM (D-) とUSB-DP (D+) をUSBコネクタ
		 に接続（配線）します。
	
	  （３）USB-DP (D+) は、1.5kΩを通して 3.3Vにプルアップします。（アクティブpull-upでなくてよい）
	
	  （４）FT/GPIO 端子をジャンパーピンで、GNDに落とします。（アプリ実行時はジャンパーオープンします）
	
	  （５）USB-VBUS端子をＵＳＢの5V端子に繋ぎます。（直結でも動きますが、気になる場合は2kΩ程度の抵抗を入れます）
	
	  （６）ＵＳＢの5V端子から3.3Vレギュレータを使用してLPC1343に3.3V供給するようにします。

ＭＳＤデバイス動作確認:
	  （７）配線確認後、USBでＰＣと接続するとLEDが点灯します。

	  （８）数秒から１０秒程度でＰＣ側がＵＳＢマスストレージデバイスと認識し、32kBの
	 　　　 USBドライブがマイコンピュータ上に現れます。

- - - -
補足:

	   分離した基板のLPC-Link側は今回は全く使用しません。

	   もし、LEDが点灯ではなくて点滅を行っている場合は、USBブートローダーが起動していない
	 　可能性が高いです。FT/GPIO端子がGNDになっているか再確認します。

	   USBデバイスが現れない場合はシリアルブート状態の可能性があります。USB-VBUS端子が5V
	   (Highレベル)になっているか再確認します。

- ＣＱ出版TRZ1010N基板を使用する場合は、USBコネクタ等の配線は不要で、**基板を加工する必要はありません。**
- ただし、ブートジャンパー(Xtalのすぐ近くに配置された3PINのピンヘッダー)にピンヘッダーもしくはスイッチ

<!-- dummy comment line for breaking list -->
等を配線して、再度別のファームを書き込むことができるようにしたほうが便利です。
- ブートジャンパーを配置しない場合は、最初に書き込んだファームウェアを永久に使い続けるか、もしくは
- ファーム更新時に3PINの下２本をリード線のようなものでショートさせてからUSBを接続する、などの技が必要になります。

<!-- dummy comment line for breaking list -->




- - - -


■ ダウンロード

404バイトのHIDデバイス.(LPCXpressoのみで動作)
- [usbhid-rom-tiny.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/usbhid-rom-tiny.zip) 
    - BOOT-ROMを呼び出すことと、トリッキーなことをしてサイズ圧縮を図っているようです。
    - 原因は分かりませんが、TRZ1010N基板では正常動作しません。

<!-- dummy comment line for breaking list -->

- - - -

1.2kバイトのHIDデバイス.(LPCXpresso / TRZ1010N 両対応)
- [usbhid-rom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/usbhid-rom.zip) 
    - BOOT-ROMを呼び出すことでサイズ圧縮を図っているようです。

<!-- dummy comment line for breaking list -->


約4kバイトのHIDデバイス.(LPCXpresso / TRZ1010N 両対応)
- [usbhid.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/usbhid.zip) 
    - BOOT-ROMに依存しない場合はコードサイズが4kになります。

<!-- dummy comment line for breaking list -->

COLOR(RED){New!}~
**LPC1343用 HID簡易モニター（armon） **.(LPCXpresso / TRZ1010N 両対応)
- [lpc-armon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/lpc-armon.zip) 
    - ~~ブートローダーとしての機能はありませんが~~、BOOT-ROMの解析くらいには使えます。
    - ブートローダーとして使えるようになりました。

<!-- dummy comment line for breaking list -->

read more : [lpc-armon](lpc-armon.md) 

~

- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/1343a.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/1343a.jpg) 

<!-- dummy comment line for breaking list -->

- - - -

■ 使い方

バックアップ:
	  （１）出現したUSBドライブ内に "firmware.bin" という名前のファイルがあるはずなので
	        それをパソコンにバックアップしておきます。

ファーム書き換え:
	  （２）出現したUSBドライブ内の "firmware.bin" を消します。
	  （３）このアーカイブを展開した中の src/firmware.bin を USBドライブのルートディレクトリ
	 　　　 にコピーします。 
	      （もしかしたら、元のfirmware.bin にそのまま上書きしてもうまくいくかもしれません）

実行:
	  （４）FT/GPIO端子をGNDに落としていたジャンパーを外して、USBケーブルを挿しなおします。
	  （５）すると、Windows側が、HIDデバイスを認識すれば成功です。

	  （６）成功したら、
	        usbhid_rom_tiny/LPC1343 HID Demonstration.exe . . . . . Windows側のHIDクライアントソフト
	        を立ち上げて、ＧＵＩ画面のＬＥＤチェックＢＯＸをマウスクリックすると基板上ＬＥＤ
	 　　　 が点灯/消灯します。


■ プログラムの再ビルド方法

	  ここでは LPCXpresso が用意した開発環境を使わずに、普通のARM用gccとMakeを使ってビルドします。

	  （１）CodeSourcery G++ Lite(ARM用:無償)をインストールします。
- [http://www.lineo.co.jp/modules/codesourcery/editions.html](http://www.lineo.co.jp/modules/codesourcery/editions.html) 
- [http://www.codesourcery.com/sgpp/lite/arm/download.html](http://www.codesourcery.com/sgpp/lite/arm/download.html) 

<!-- dummy comment line for breaking list -->

- arm-none-eabi-gcc を使用するため、インストールしたG++Liteのbinディレクトリに実行パスを通しておきます。

<!-- dummy comment line for breaking list -->

	  （２）WinARMもしくはWinAVR をインストールして、C:/WinARM/utils/bin 
	        もしくは、C:/WinAVR/utils/bin のみも実行PATHを通しておきます。
	        （これは、make.exe や rm.exe などのunix系コマンドを使うためです）

- [http://winavr.sourceforge.net/](http://winavr.sourceforge.net/) 
- [http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/) 

<!-- dummy comment line for breaking list -->


	  （３）src/ ディレクトリにて普通にmakeしてください。


■ ディレクトリ構成

	src/		. . . . . ファームウェア部分のソース
	inc/		. . . . . ファームウェア部分のinclude関係
	HW/		    . . . . . 機種依存ライブラリ（詳細は下記）
	HW/CMSISv1p30_LPC13xx/cmsis_readme.txt
	HW/CMSISv1p30_LPC13xx/history.txt
	HW/CMSISv1p30_LPC13xx/docs/CMSIS changes.htm
	HW/CMSISv1p30_LPC13xx/docs/CMSIS debug support.htm
	HW/CMSISv1p30_LPC13xx/docs/CMSIS_Core.htm
	HW/CMSISv1p30_LPC13xx/docs/License.doc
	HW/CMSISv1p30_LPC13xx/inc/core_cm3.h
	HW/CMSISv1p30_LPC13xx/inc/LPC13xx.h
	HW/CMSISv1p30_LPC13xx/inc/system_LPC13xx.h
	HW/CMSISv1p30_LPC13xx/src/core_cm3.c
	HW/CMSISv1p30_LPC13xx/src/system_LPC13xx.c
	
	usbhid_rom_tiny/LPC1343 HID Demonstration.exe . . . . . Windows側のHIDクライアントソフト
	usbhid_rom_tiny/srec2bin.c                    . . . . . srec2bin.exe のソース.

~
## LPC1343用のHIDブートローダーに関する補足説明

read more : [lpc-armon](lpc-armon.md) 

