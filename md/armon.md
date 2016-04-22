[ARM](ARM.md) 　[stm32f103](stm32f103.md) 


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/ft2232jtag.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/ft2232jtag.jpg) 

## armon/armboot

## ■ 概要

- これは、[STM8S-Discovery付属のSTM32F103C8T6　CPU](stm32f103.md) 用の

<!-- dummy comment line for breaking list -->
HIDクラス・ブートローダー 兼 簡易モニターです。

~
このブートローダーを使用するためには、
- まず最初に、[OpenOCDが使用できるJTAGライター](hid_blaster.md) が必要です。
- 詳しくは、[stm32f103](stm32f103.md) の項目をお読みください。
- 将来的には、このブートローダーを用いてstm32cpuを使用したJTAGライターを製作するのが主な目的になっています。（鶏が先か、それとも・・・）
    - [OpenOCD　JTAGライターとして使えるようになりました。](stm8s_blaster.md) 

<!-- dummy comment line for breaking list -->

read more :[stm8s_blaster](stm8s_blaster.md) 　[arm_blaster](arm_blaster.md) 


~
このブートローダーを使用することによって、以下のようなことが出来るようになります。
- Cortex-M3用のプログラム開発。
- 内部メモリーやI/Oポートの確認、変更。

<!-- dummy comment line for breaking list -->

~
このブートローダーを使用することによって、以下のことがらを諦めなければなりません。
- STM8Sのプログラム開発。
- 元々焼かれていたST-Linkファームウェアに復元すること。

<!-- dummy comment line for breaking list -->


![http://www.akizukidenshi.com/img/goods/1/M-03457.jpg](http://www.akizukidenshi.com/img/goods/1/M-03457.jpg) 


- - - -
## ダウンロード。

|種別|ファイル|対応基板|
|STM32版|[armon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/armon.zip) |STM8S-DiscoveryのSTM32側/CQ-STARM/STBEE/STBEE Mini|
|LPC-1343版|[lpc-armon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/lpc-armon.zip) |LPCXpresso/TRZ-1010N|
|LPC2388版|[arm7mon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/arm7mon.zip) |CQ-FRK-NXP|

~
いずれの版も、使用に関しては自己責任でお願いします。
- ファームウェアソース、HEXファイル、Host側ソース、EXEファイルを含みます。
- Linuxホストから使用する場合はHost側を再コンパイルして使用してください。

<!-- dummy comment line for breaking list -->


## アプリケーション・サンプルソース
|種別|ファイル|対応基板|
|STM32版 仮想COMサンプル他|[sample.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/sample.zip) |STM8S-DiscoveryのSTM32側、他|
|LPC1343版 仮想COMサンプル他|[lpc-sample.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/lpc-sample.zip) |LPCXpresso(LPC1343)、他|
|LPC2388版 仮想COMサンプル他|[arm7sample.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/arm7sample.zip) |CQ-FRK-NXP-ARM|

2011-01-03: sample.zipを更新しました。仮想COMポートデバイスにて受信割り込みハンドラー関数がコンパイルされていない問題を修正しました。

2011-01-05: lpc-sample.zipを更新しました。仮想COMポートデバイスの登録に失敗するバグを修正しました。


~
いずれの版も、使用に関しては自己責任でお願いします。
- アプリケーションというのは簡単なUSBデバイス作成サンプルのことです。
- CodeSourcery G++ Liteを使用してビルドすることが出来ますが、実際に動作させるにはそれなりの付加回路(SD-CARDとか)が必要だったり、基板ごとの仕様に合わせて手直しが必要な場合があります。
- armbootブートローダーを用いないで単独使用したい場合は、開始番地を2000番地から0000番地へ変更する必要があります。

<!-- dummy comment line for breaking list -->
(Makefile内にROMADRSというmake変数が用意してあります。)

- - - -
## ■ 現在のステータス

- ・コンパイラ "CodeSourcery G++ Lite" にて、ビルドは通ります。
- ・通電すると、LEDが点灯します。
- ・HIDクラスとしてＰＣから認識されます。
- ・host/armon.exe を起動すると、メモリーダンプなどが実行できます。
- ・host/armboot.exe を起動すると、HEXファイルをFlashに書き込むことが出来ます。

<!-- dummy comment line for breaking list -->

- SWIM端子のGND <=> RESET# 間にBOOT JUMPERを挟んでください。 JUMPER CLOSE で、BOOTLOADERが起動します。
- JUMPER OPEN  で、0x0800_2000番地からのファームウェアが起動します。

<!-- dummy comment line for breaking list -->

- STM8S-Discoveryのほか、CQ-STARM 、STBee 、 STBee Miniでの動作確認を行いました。

<!-- dummy comment line for breaking list -->



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/stbee.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/stbee.jpg) 

左から、
- [STBee　MINI](http://strawberry-linux.com/catalog/items?code=32105) 
- [STBee](http://strawberry-linux.com/catalog/items?code=32103) 
- [STM8S-Discovery](http://akizukidenshi.com/catalog/g/gM-03457/) のST-Link側のみ
- DesignWave2008-05 付録 CQ-STARM

<!-- dummy comment line for breaking list -->

- クロックは全部72MHz Cortex-M3
- Flash+SRAM容量は左から、(128K+20K),(512K+64K),(64K+20K),(128K+20K)

<!-- dummy comment line for breaking list -->

## ■ 使い方

- （１）JTAGライターを使用して、firmware/main-0000.hex を焼きます。
- （２）CN7の GND<=>SB1(RESET#)間を JUMPERでショートします。
- （３）USBでＰＣと接続するとLEDが点灯します。
- （４）host/armon.exe を起動して接続できれば成功です。メモリーダンプ('d')

<!-- dummy comment line for breaking list -->
　　　　 と、メモリー書き換え('ew')が出来ます。
- （５）host/armboot.exe  firmware/main-2000.hex を実行すると、0800_2000番地に

<!-- dummy comment line for breaking list -->
　         アプリケーションとしてのHIDmonを書き込めます。
- （６）書き込んだHIDmonを実行するには、CN7のジャンパーをオープンにしてから

<!-- dummy comment line for breaking list -->
　         USBケーブルを再接続してください。
- （７）アプリケーション側(0800_2000番地で動作するバージョン)の動作中はLEDが

<!-- dummy comment line for breaking list -->
　　　　　 １秒周期で点滅を行うことで動作の区別をつけるようにしました。

注意
- STM8S-Discovery以外の基板では、MakefileのBASEBOARD定義を切り替えてビルドを行ってください。
- STM8S-Discovery以外の基板では、0800_3000番地開始のHEXを作成したのち、そのHEXをDFUW.exeで書き込んでから、

<!-- dummy comment line for breaking list -->
0x0800_0000番地開始のブートローダーを書き込む(mkall.batとbb.bat)ことで**JTAGライターなしで**ファーム差し替えを行うことが可能です。
    - 但し、書き換えには細心の注意を払ってください。

<!-- dummy comment line for breaking list -->



## ■ userコマンドについて

	  ARM> user
	       ~~~~
- というコマンドを実行すると、usercmd.c に記述されたユーザールーチン

<!-- dummy comment line for breaking list -->
　 を起動することが出来ます。

- 現在は、簡単なprintf のテストが出来るようになっています。

<!-- dummy comment line for breaking list -->


## ■ ＴｏＤｏ

未実装項目

- ・HID以外のUSB Classへの移植
- ・~~I/Oポート名解決,I/Oダンプ~~ --- 少し実装(STM32のみ)
- ・簡易ロジアナ,オシロ
- ・AVR/PICライター機能追加

<!-- dummy comment line for breaking list -->

- ・バスエラーしたら死ぬ.
- ・readで無駄にFLASH全領域をREADする.
- ・RAMエリアに対する bootload＆run 機能
- ・簡易レジスタダンプ
- ・

<!-- dummy comment line for breaking list -->


実装完了項目

- ・高速化
- ・runコマンドが変.
- ・bootコマンド欲しい.
- ・(Application modeで)printが欲しい.
- ・armboot -r オプション.
- ・USB disconnect
- ・他の基板への移植(CQ-STARM,STBeeなど)
- ・逆アセンブラがPIC18Fのまま.

<!-- dummy comment line for breaking list -->

## ■ 補足

- （１） armboot.exeは '-r'オプション指定により、アプリケーションの自動起動が出来るようになりました。
    - 今までは BOOT JUMPERの変更を行ったあとで、USBケーブルの挿抜を行う必要がありました。

<!-- dummy comment line for breaking list -->

- （２） armon.exe の boot コマンドを用いることで、互いに相手側のファームウェアを再起動することが出来るようになりました。

<!-- dummy comment line for breaking list -->

    - 例： BOOTLOADERからAPPLICATION側に切り替える。
    
    		ARM&gt; boot 8002000  （2000でもＯＫ）
    - 例： APPLICATION側からBOOTLOADERに切り替える。
    
    		ARM&gt; boot 8000000  （0 でもＯＫ）
- （３）互いに、相手側のファームウェアを更新することが可能です。
    - b.bat  ・・・ BOOTLOADERからAPPLICATION側のファームを書き換えるバッチファイル。
    - bb.bat ・・・ APPLICATION側からBOOTLOADERのファームを書き換えるバッチファイル。

<!-- dummy comment line for breaking list -->

    - BOOTLOADER側の書き換えに失敗した（ファームウェアをバグらせてしまった）場合は、外部のJTAGライターが再度必要になります。

<!-- dummy comment line for breaking list -->

- （４）Linux版もすこしづつ動き始めました。
    - host/ディレクトリにて、ビルドしてみてください。
    - libusb-devが必要です。
    
    		$ sudo apt-get install libusb-dev
    - 実行は root権限で行なうか、該当デバイスをユーザー権限で使えるようにLinux側の(/etc/)設定を適宜行います。

<!-- dummy comment line for breaking list -->



## ■ ＦＡＱ

~
(750円)どうしてこんなに安いのですか？
- [秋月電子さんのお答え](http://akizukidenshi.com/catalog/faq/goodsfaq.aspx?goods=M-03457) 　

<!-- dummy comment line for breaking list -->

~
どうして、一度ファームを書き換えてしまうと、元に戻せなくなるのですか？
- それは、元のST-Linkファームが読み出し禁止モードで書き込まれているからです。
- 全く同じ機能をもつファームを（USBプロトコルアナライザーなどで解析して）自分で開発して、書き込めば、なんとなく元にもどるでしょう。

<!-- dummy comment line for breaking list -->

~
どうして、元に戻せなくなるのに、ファームを書き換えるのですか？
- こんなに安いので、**２個以上**買って使えばいいのです。
- １個はオリジナルファームのまま使います。
- 残りは全部改造してお好きなように。

<!-- dummy comment line for breaking list -->

- ほら、STM8Sも２個以上手に入るので、おトクでしょ？

<!-- dummy comment line for breaking list -->

    - （って、通販の布団圧縮袋じゃないんだから・・・）

<!-- dummy comment line for breaking list -->

~
どうして、こんなちっぽけな基板をわざわざ改造して使うんですか？
- ARMチップ単体を買って基板起こすの**面倒**じゃないですか。
- すでにLDOレギュレータとかUSB-Bコネとか水晶まで実装済みで、JTAGピンもBOOT JUMPERも立っているのって、便利じゃないですか。

<!-- dummy comment line for breaking list -->




## ■ ディレクトリ構成

	host/	    . . . . . ホストＰＣ側のツールソース(PICmon)
	firmware/	. . . . . ファームウェア部分のソース
	inc/		. . . . . ファームウェア部分のinclude関係
	HW/STM32_EVAL/  . . . ST評価ボードのポート定義
	HW/Libraries/   . . . ハードウェアライブラリ

	HW/Libraries/STM32F10x_StdPeriph_Driver/inc/
	HW/Libraries/STM32F10x_StdPeriph_Driver/src/
	HW/Libraries/STM32_USB-FS-Device_Driver/inc/
	HW/Libraries/STM32_USB-FS-Device_Driver/src/
	HW/Libraries/CMSIS/Core/
	HW/Libraries/CMSIS/Core/CM3/
	HW/Libraries/CMSIS/Core/CM3/startup/
	HW/Libraries/CMSIS/Core/CM3/startup/arm/
	HW/Libraries/CMSIS/Core/CM3/startup/gcc/
	HW/Libraries/CMSIS/Core/CM3/startup/iar/
	HW/Libraries/CMSIS/Core/Documentation/

## ARMビルド環境の構築方法
- [WinARMビルド環境の構築方法](WinARM.md) 
- [ARMクロスコンパイラ構築（Linux）](2010-04#b817f226.md) 
- [CodeSourcery_G++_Lite](http://www.lineo.co.jp/modules/codesourcery/editions.html) 　←今のところこれが一番お勧めです。

<!-- dummy comment line for breaking list -->

2011-12追記
    - ↑ CodeSourcery はmentor graphicsに買収されてしまいましたので、G++_Liteの入手性が悪くなっています。
    - 代替として、[leaflabs](http://leaflabs.com/) から、maple-ide-0.0.12-windowsxp32.zipを入手して使用することをお勧めします。
    - さいわいなことに、GCCのVersionも4.4.1なので**-Os**オプション指定によるバグも回避できます。
    - arm-none-eabi-gcc.exeの実行パスは、「maple-ide-0.0.12をインストールしたパス」の hardware/tools/arm/bin/ に通します。
    - もうひとつの選択肢としては、[netx-arm-elf-gcc](http://sourceforge.jp/projects/sfnet_netx-arm-elf-gc/) を使用する方法もあります。

<!-- dummy comment line for breaking list -->


- - - -
- WinARMはlibgccのfloat関数が抜けているので、一部差し替えの必要があります。
    - また、libc.aの_sbrk_rや_open_r,_read_rなども抜けています。（組み込み用途でファイルオープンは普通しませんが）
- CodeSourcery_G++_Liteはコマンドライン版ですが無償で使用できます。また、gccのバージョンが新しい(4.4.1)ためか、生成コードサイズが小さくなります。

<!-- dummy comment line for breaking list -->

- CodeSourcery_G++_Liteには makeやls,catといったunix標準コマンドがあまり含まれていません(cs_make.exeは付いています)ので、WinAVRを別途インストールされている方は、そちら（C:\WinAVR\utils\bin\）にもPATHを通しておいたほうが良いでしょう。

