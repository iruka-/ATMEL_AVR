----
## ＨＩＤｍｏｎとは

- ＨＩＤａｓｐ側のメモリーを読み書きするツールです。
- 但し、ＨＩＤａｓｐ側のＲＡＭといってもほんの１０バイトくらいしか余っていませんので、読み書きしたところでどうなるものでもありません。
- 実際には、汎用ＩＯのようにポートをホストＰＣ側から触って遊ぶツールとなります。
- 不幸にもＵＳＢの接続ポートのDDRレジスタなどを触ってしまうと通信が途絶します。
- ＬＥＤを手動で点灯させたり、ＰＷＭを設定して（繋いだ）スピーカーから音を出して遊ぶとかそんな感じになります。
- 詳細については　[AVR_Monit](AVR_Monit.md) 　の項目をお読みください。

<!-- dummy comment line for breaking list -->



- サンプルスクリプトで１ＭＨｚのＣＷ（電信）電波を出すものを入れておきました。
- PB2に短いアンテナを繋げて、ＡＭラジオで受信してください。

<!-- dummy comment line for breaking list -->

- graphコマンドを追加しました。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/b.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/b.png) 

	AVR> graph pind のようにして使います。
- 省略すると pinb をグラフ表示します。
- ESCキーで終了してください。
- サンプリングレートは今のところ全力のままです。

<!-- dummy comment line for breaking list -->

## ＡＴｔｉｎｙ２３１３用ＨＩＤｍｏｎ

[ＭＩＲＲＯＲ　ＤＯＷＮＬＯＡＤ](ＤｏｗｎＬｏａｄ.md) 

## ＨＩＤａｓｐ旧回路用での注意
- ＨＩＤａｓｐ旧回路用のファームhidspx-1005-pb01.zipに付きましては、
    - ＵＳＢのＤ＋がＰＢ１，ＰＤ２の両方に接続。
    - ＵＳＢのＤ−がＰＢ０に接続。
- という配線になっていますので、ＰＢ１、ＰＤ２の両方を出力モードにして、各ｐｉｎの出力レベルが異なってしまう場合に大電流が流れてチップを損傷する可能性があります。
- ですので、ＨＩＤｍｏｎの使用はできるだけ避けるようにお願いいたします。
    - ＨＩＤａｓｐのデバッグ用途でピンやポート状態を探るという用途に限定すれば使えますが。

<!-- dummy comment line for breaking list -->




## ＡＴｍｅｇａ８８用ＨＩＤｍｏｎ

- おまけのおまけ：ＨＩＤｍｏｎ８８を作成しました。
- これはＡＴｍｅｇａ８８に移植したＨＩＤｍｏｎです。
- 先にモニタだけ移植して、ＡＶＲライターとしてはまだ機能しません。

<!-- dummy comment line for breaking list -->

- [アーカイブ：hidmon-Mega88-1002.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidmon-Mega88-1002.zip) 

<!-- dummy comment line for breaking list -->

- ＡＴｍｅｇａ８８側のメモリーを読み書きするツールです。
- ＡＴｍｅｇａ８８側のＲＡＭは１Ｋバイトもありますが、それを読み書きしたところでどうなるものでもありません。
- 実際には、汎用ＩＯのようにポートをホストＰＣ側から触って遊ぶツールとなります。
- 不幸にもＵＳＢの接続ポートのDDRレジスタなどを触ってしまうと通信が途絶します。
- ＬＥＤを手動で点灯させたり、ＰＷＭを設定して（繋いだ）スピーカーから音を出して遊ぶとか、アナログポートを読み取ってグラフにするとか、そんな感じになります。

<!-- dummy comment line for breaking list -->

## bootmon for ATmega88
- bootloadHIDとhidmon88の機能を合成しました。
- 何が出来るようになるかというと、
    - bootloaderとしては、bootloadHIDとまったく同じにそのまま使える。
    - hidmonとしても使える。

<!-- dummy comment line for breaking list -->

- 使い道としては、
    - 適当なアプリケーションを書いて、bootloaderで転送して実行させる。
    - 実行前にＰＣからデータを流し込んで、それをアプリケーションが使う。
    - 実行結果をRAMに残して、それを後で読み取って使う。

<!-- dummy comment line for breaking list -->

たとえば、
- ＰＳ２キーボードのデータ取得ルーチンを書いたけど、早速試したい。
    - 実行結果をRAMに残して、あとで読み取って確認する。
    - もちろん、取得結果を直接ＵＳＢで受信するとか、シリアルに流すということを普通は行いますが、
    - 割り込み処理の関係上ＵＳＢと並列動作させにくい場合もあります。
    - シリアル変換を繋いだり、シリアルのルーチンを書くのが面倒くさいという場合もあります。

<!-- dummy comment line for breaking list -->

- 高速なロジアナもどきを作ったが、取得結果をＵＳＢで拾いたい。
    - つまり、一定時間ＵＳＢの割り込みOFFで動きたいという場合です。
    - ＵＳＢドライバーを一定時間停止させる処理とか書くの面倒じゃないですか。
    - だったら、アプリケーションだけ単独で動かして、結果だけhidmonが拾います。

<!-- dummy comment line for breaking list -->

- [アーカイブ：bootmon-1127.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/bootmon-1127.zip) 

<!-- dummy comment line for breaking list -->

- - - -
## キーボードまにあ　Ｐａｒｔ３
ＡＴｍｅｇａ８８にＰＳ/２キーボードを接続して、入力スイッチのかわりに使おうという
プロジェクトです。

- とりあえず、仮作成したソースをＵＰします。

<!-- dummy comment line for breaking list -->

- [アーカイブ：keybrd-mania-1211.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/keybrd-mania-1211.zip) 
- bootmon から書き込んで実行出来ます。

<!-- dummy comment line for breaking list -->


ChangeLog

- 2008/11/27 - 千秋ゼミさんからbootmon-1127.zipをミラーしました。
- 2008/11/13 - test/ディレクトリに応用領域用のfirmを用意しました。
- 2008/11/06 - 未初期化変数により動作不安定バグがあったのを修正しました。メモリー削減を実施しました。
- 2008/11/03 - poll graphコマンドでfirmwareのPOLL機能を使用するようにして高速化しました。
- 2008/11/02 - kuga様のアドバイスによりWORKRAMを480番地に移動、また、bootloader書き込み後のcodeエリア読み込みできない問題を解決しました。

