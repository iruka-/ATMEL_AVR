[2011-08](2011-08.md) 


- - - -
## libmapleを使って仮想COMベースのARMモニターを作る。

- とりあえず出来ました。

<!-- dummy comment line for breaking list -->

ダウンロード:
- CQ-STARM , STM8S-Discovery , STBee , STBeeMini の４基板に対応しています。

<!-- dummy comment line for breaking list -->

- **こちらから[[ダウンロード:https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/cdctool.zip]]してください。**

<!-- dummy comment line for breaking list -->

~
移植の顛末：
- 簡単に移植できるはずが・・・libmapleもCDC-仮想serialポートの実装は手抜きになっていて、それを知らずにハマりました。
    - ホストPCから1文字づつ送る ---> デバイス側も1文字単位で受け取る。＝問題なし。
    - ホストPCから40バイトくらいまとめて送る--->デバイス側でまず1バイトのヘッダーを受信して、ＯＫなら残り39バイトを受信する＝バグる。

<!-- dummy comment line for breaking list -->

    - バグ状況は、２度目のSerialUSB.read()では送信されたデータの2バイト目から40バイト目までを受け取るはずなのに、１バイト目から39バイト目までのデータを受け取って（コピーして）しまう。40バイト目は永遠に受け取れない。

<!-- dummy comment line for breaking list -->

- この手のソフトは、デバイス側とホスト側の両方を実装しないと試せないので、なかなかモチベーションが上がりません。

<!-- dummy comment line for breaking list -->

- CDC仮想comはバルク転送なのでデータをまとめて送るぶんには速いのですが、いわゆる普通のserialポートのようにPCから1バイト送ってはデバイスの応答を待つような書き方をすると、結局のところUSBの1フレーム＝1mS掛かる問題が発生して、１秒間にせいぜい５００回くらいしかハンドシェークできない結果になるようです。（下手をするとリアルなserialポートより遅くなります）

<!-- dummy comment line for breaking list -->
~
~
## 積み残し問題

未解決です。
- STM32の仮想COMポートの検索方法---現在は、一番後ろのCOM:を決めうち。
    - 後ろから、調べていく方法に変更したい。
- STM32の仮想COMポートの特定方法
    - 特別なボーレート(0xffffとか)で初期化された場合にのみ特定しハンドシェークするようにする。
- pinMode/digitalRead/digitalWrite相当のコマンドの追加
- レート指定、トリガー指定可能なデジタル/アナログinputコマンド
- それに対応するホスト側ウィンドウ

<!-- dummy comment line for breaking list -->

~
今回のフォーカスはAVR/PICライターではなく、シグナルサンプラーになる予定です。
- NXP系チップに移植する予定は当分ありません。手間がかかりすぎます。
- 入手性に関しても、LPC17xx系基板はXpresso以外にあまり手に入らないので除外です。

<!-- dummy comment line for breaking list -->




~
~


[続きを読む](2011-09.md) 


[仮想COMベースのブートローダー](libmapleでブートローダー.md)  製作編


~

~
~
~
~
以下、まだ書きかけです。

~
#### 予定稿

- libmapleを使って[仮想COMベースのブートローダー](libmapleでブートローダー.md) を作る。

<!-- dummy comment line for breaking list -->

- libmapleを使って仮想COMベースのAVR/PIC/PIC24/OpenOCDライターを作る。

<!-- dummy comment line for breaking list -->



仮想COMを使用する理由
- HIDよりも転送速度が速い。（はず）
- Linux上ではHIDよりもcom portのほうが扱いやすい。

<!-- dummy comment line for breaking list -->

- Windows上でも、STM32用の仮想COMドライバーを入れるだけなので、それほど心理的抵抗はないと思われる。

<!-- dummy comment line for breaking list -->

- LPC1114(AMRY基板)に簡単に移植できる。ホスト側は共通になるかも。

