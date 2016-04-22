[2011-01](2011-01.md) 

## STM32:なんちゃってマスストレージ・デバイス on STM8S-Discovery

ソースは[こちら](armon.md) にある、以下のこれ。

## アプリケーション・サンプルソース
|種別|ファイル|対応基板|
|STM32版 仮想COMサンプル他|[sample.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/sample.zip) |STM8S-DiscoveryのSTM32側、他|

- sample.zip内にmsdデバイスサンプルがあります。
- ハードウェアは、とりあえず、STM8S-DiscoveryのST-Link側だけを使用します。(CQ-STARMなどでも同様に可)
- ファーム書き込みは拙作 armboot.exe で行ないます。
- ファームを書き込んだら、ブートジャンパーをOPENにして、USB再接続するとＵＳＢ ＨＤＤデバイスが現れます。

<!-- dummy comment line for breaking list -->

## 差分はこちら
	Index: msc/src/scsi_data.c
	===================================================================
	--- msc/src/scsi_data.c (revision 290)
	+++ msc/src/scsi_data.c (working copy)
	@@ -32,7 +32,7 @@
	     0x80,          /* RMB = 1: Removable Medium */
	     0x02,          /* Version: No conformance claim to standard */
	     0x02,
	-
	+#if    0
	     36 - 4,          /* Additional Length */
	     0x00,          /* SCCS = 1: Storage Controller Component */
	     0x00,
	@@ -44,6 +44,20 @@
	     'D', 'i', 's', 'k', ' ', ' ', ' ',
	     /* Product Revision Level */
	     '1', '.', '0', ' '
	+#else
	+    36 - 4,          /* Additional Length */
	+    0x00,          /* SCCS = 1: Storage Controller Component */
	+    0x00,
	+    0x00,
	+    /* Vendor Identification */
	+    'M', 'a', 'x', 't', 'o', 'r', ' ', '5',
	+    /* Product Identification */
	+    'A', '1', '2', '0', 'J', '0', ' ', ' ', ' ',
	+    ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	+    /* Product Revision Level */
	+    '1', '.', '0', ' '
	+#endif
	+
	   };
	 uint8_t Standard_Inquiry_Data2[] =
	   {

- 文字数は変えません。
- ほんとうは（たぶん）SPIインターフェースの先にMMC/SDを接続しないと正しく動作しないような気もします。
- まあ、**フェイク**なので未配線のままテストします。

<!-- dummy comment line for breaking list -->

- これで、Maxtorのドライブがデバイス一覧のドライブに現れればＯＫです。

<!-- dummy comment line for breaking list -->

~
- SPIインターフェースの先にMMC/SDを接続して**実ドライブ**にしたいなぁ・・・。（SPIだしUSB FullSpeedなので遅いけど）

