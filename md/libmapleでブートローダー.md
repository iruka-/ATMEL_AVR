[2011-09](2011-09.md) 　[libmapleで仮想COM](libmapleで仮想COM.md) 

- - - -
## libmapleを使って仮想COMベースのブートローダーを作る。

ダウンロード：
- [cdcboot.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/cdcboot.zip) 

<!-- dummy comment line for breaking list -->

説明：

- Ｃ言語のみで仮想COMフレームワークを構築しています。
    - ファームウェア、ホスト側ユーティリティ、およびソースコードを含みます。
- ブートローダーとして使用できます。
    - アプリケーションフレームワークとしても使用できます。

<!-- dummy comment line for breaking list -->


~
- - - -
armon/armboot との違い：
- armon/armbootはUSBのHIDクラスを使用していますが、こちらはCDCクラス（仮想COM）となります。
- CDCクラスになるため、デバイスドライバーの組み込みが必要です。（Windowsの標準仮想COMドライバーになります）

<!-- dummy comment line for breaking list -->

- CDCクラスは、転送速度がHIDデバイスよりも**高速**です。

<!-- dummy comment line for breaking list -->

- ブートローダーのサイズがHIDのもの0x2000(8kB)から0x3000(12kB)に増加しています。

<!-- dummy comment line for breaking list -->






- - - -

- 簡易モニターのホスト側ソフトは cdcboot/host/cdctool.exe を使用します。
- FLASH書き換えのホスト側ソフトは cdcboot/host/cdcboot.exe を使用します。

<!-- dummy comment line for breaking list -->

- CQ-STARM , STM8S-DISCOVERY , STBee , STBeeMini の４基板をサポートしました。
- 常駐サイズは12kB（実際のコードサイズは10kB程度）です。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
現バージョンの問題点
- STBeeのみ、アプリケーションモードへの切り替えが、ややうまくいきません。
- （cdcboot.exe -r がうまくいかない）
- とりあえずアプリ書き込み後、USBケーブルの挿しなおしでアプリケーション起動になります。

