[ARM](ARM.md) 　[LPCUSB](LPCUSB.md) 

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/cq-frk-nxp.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/cq-frk-nxp.jpg) 

## interface 2009-05付録基板用のHIDブートローダー兼モニター

[armon/armboot](armon.md)  移植済みの基板リスト(LPC2388用はSTM32からバックポートしました)

|アーキテクチャー|ＣＰＵ(ベンダー)|基板名|FLASH容量|SRAM容量|
|Cortex-M3|STM32(STMicro)|[STM8S-Discovery](stm32f103.md) のSTM32側|64kB|20kB|
|||[CQ-STARM](armon.md) 　DesignWave 2008-05付録|128kB|20kB|
|||[STBEE](armon.md) 　ストロベリーリナックス|512kB|64kB|
|||[STBEE　Mini](armon.md) |128kB|20kB|
||LPC1343(NXP)|[LPCXpresso](LPCXpresso.md) 　NXPセミコンダクターズ|32kB|8kB|
||       |[TRZ1010N](LPCXpresso.md) 　トラ技増刊「ARMマイコン パーフェクト学習基板」|32kB|8kB|
|ARM7TDMI|LPC2388(NXP)|COLOR(BLUE){CQ-FRK-NXP-ARM}(*これ)|512kB|64kB|

~
CPUクロックは全て72MHz

## 特徴
- 各ブートローダーのサイズはいずれも8kB(未満)です。
- HIDデバイスなのでドライバーソフトの組み込み不要で、すぐ使えます。
- ブートローダーと同じソースのアプリケーションが用意されていて、ユーザー定義関数が簡単に実行できます。
    - （printf関数の結果をＰＣのコンソールに表示できます）
- ブートローダーとアプリケーションの間を自由に行き来できます（bootコマンドにて）
- アプリケーションモードの状態からでも、そのまま更新されたプログラムの再書き込みと実行がすぐ出来ます。（コマンドラインツールarmboot）
- メモリーダンプ、GPIOポートやI/Oレジスタの内容表示が可能です（現在はシンボルは使えません。番地を与える形式）
- 任意番地からの実行も可能です。
- 逆アセンブラ内蔵です。（armon.exe側が処理します）

<!-- dummy comment line for breaking list -->

## 弱点
- (CPU内部)レジスタダンプ機能はありません。（実装してもあまり意味は無いかも。）
- (CPU内部)レジスタはPIC版やAVR版ではRAMメモリー空間にマップされているため表示が可能ですが、ARM版では出来ません。
- printfデバッグは可能ですがトレース実行機能などはありません。
- HID以外のデバイスを書いてテストする場合はprintfは自分で実装する必要があります。（場合によってはシリアル側へ吐くなど）

<!-- dummy comment line for breaking list -->

## 利点
- PIC版,AVR版とほぼ同じ感覚で利用できます。（ソースツリーの大半は使い回しです）
- 上記のそれぞれのARMデバイスでほぼ共通の操作が出来ます。（実行ファイルは別になる場合があります）
- 将来PIC18F/AVRライター機能を追加可能です。（GPIOポートを割り当てるだけで、ライター機能そのものはPIC版に実装済みです）
- ソース書き換え→実行のターンアラウンドが短い。

		armboot.exe -r -nv main-2000.hex という１行バッチを走らせるだけ。非常に短時間です。
    - DFUの場合はelf->dfuへの変換とDFuSeという面倒なGUIを操作する必要があり、さらに、USBの挿抜とブートジャンパーの切り替え操作まで必要になります。
    - 内蔵BOOTROMによるUSB-MSCクラスローダーやシリアルローダーを使用する場合にも、ジャンパーの切り替えが必要で、USBドライブ認識に時間が掛かることが多いですが、（ブートローダーとアプリケーションが）互いにHIDデバイスの場合はそのような煩雑な動作は不要になります。

<!-- dummy comment line for breaking list -->



## ダウンロード

|種別|ファイル|対応基板|
|LPC-2388ブートローダー |[arm7mon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/arm7mon.zip) |CQ-FRK-NXP-ARM|


## アプリケーション・サンプルソース
|種別|ファイル|対応基板|
|LPC2388版 仮想COMサンプル他|[arm7sample.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/arm7sample.zip) |CQ-FRK-NXP-ARM|

~

- 今回のLPC2388版は、STM32/LPC1343版からバックポートしました。
- ~~逆アセンブラの表示がややおかしいです。~~ --- 直しました。
- FLASHへの書き込み機能はあります ~~が、アプリケーションのブートがまだうまくいきません。~~ -- ＯＫになりました。
- Thumbコードを逆アセンブルしたいときは、'l'コマンドに与える番地を奇数に(LSBを1)してください。

<!-- dummy comment line for breaking list -->


## ビルド環境の構築方法

- [WinARMビルド環境の構築方法](WinARM.md) 
- [ARMクロスコンパイラ構築（Linux）](2010-04#b817f226.md) 
- [CodeSourcery_G++_Lite](http://www.lineo.co.jp/modules/codesourcery/editions.html) 　←今のところこれが一番お勧めです。

<!-- dummy comment line for breaking list -->


- - - -
## ビルド環境の注意など
- WinARMはlibgccのfloat関数が抜けているので、一部差し替えの必要があります。
    - また、libc.aの_sbrk_rや_open_r,_read_rなども抜けています。（組み込み用途でファイルオープンは普通しませんが）
- CodeSourcery_G++_Liteはコマンドライン版ですが無償で使用できます。また、gccのバージョンが新しい(4.4.1)ためか、生成コードサイズが小さくなります。

<!-- dummy comment line for breaking list -->

- CodeSourcery_G++_Liteには makeやls,catといったunix標準コマンドがあまり含まれていません(cs_make.exeは付いています)ので、WinAVRを別途インストールされている方は、そちら（C:\WinAVR\utils\bin\）にもPATHを通しておいたほうが良いでしょう。

