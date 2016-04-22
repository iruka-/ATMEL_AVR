[PIC18F2550](PIC18F2550.md) 

## UBWを試す。

UBWについては、以下の解説サイトが詳しいです。

~

PIC GAMES：ＵＢＷ解説サイト
- [http://www.picgames.org/modules/tinyd1/index.php?id=31](http://www.picgames.org/modules/tinyd1/index.php?id=31) 

<!-- dummy comment line for breaking list -->


ＵＢＷ公式サイト
- [http://www.schmalzhaus.com/UBW/](http://www.schmalzhaus.com/UBW/) 

<!-- dummy comment line for breaking list -->


千秋ゼミ：ＵＢＷ
- [http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?memo%2FUBW#content_1_6](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?memo%2FUBW#content_1_6) 

<!-- dummy comment line for breaking list -->



~
~
~
インストールメモ
- UBWで扱うファームウェアはfirmware-B（UBW_B_HEX8.zip）とfirmware-Dの２種類がある。
- firmware-BはBootloaderであり、Flash先頭の2kB(0000〜07ff)に常駐する。
- firmware-Dは仮想シリアルポートを提供するファームウェアで、teratermなどから接続して、コマンドによりPICのポートを操作することができる。
- **まず、外部PICライターでfirmware-Bを焼いた後、fsusbを使用してfirmware-Dを導入する**
    - そのときにWindows側にMCHIP-USB.zipに含まれているデバイスドライバーを導入する必要がある。

<!-- dummy comment line for breaking list -->

    - Firmware-Dについて誤解を恐れずに一言で言い表すなら**改良版Gainer**である。

<!-- dummy comment line for breaking list -->

落とし穴
- fsusbを使うときはlibusbフィルタードライバーを導入する必要がある。
    - ~~libusb-win32-filter-bin-0.1.12.1.exe~~
    - しかし、これはVistaには絶対入れてはいけないし、XPでもOSが不安定になることがあるので推奨しない。
    - WindowsでUBWを使う場合はMyBootというGUIの書き込みソフトを使うと良い。

<!-- dummy comment line for breaking list -->

- firmware-Bを起動する場合には、RC2ポートをスイッチでGNDに落としながらリセット操作を行う必要がある。
    - RC2がpull up状態のときは、アプリケーション(ここではfirmware-D)が起動する。

<!-- dummy comment line for breaking list -->

問題点
- 自分のＰＣ環境(WinXP-SP3-HOME,OHCI)では、どちらのfirmwareの動作もかなり不安定で、実用にならない。
    - firmware-Bは何回もリセット操作を行なわないと起動(Windows側が認識)しない。
    - たまたまfsusbが認識するタイミングにうまく合わせてfirmware-Dを書き込めばＯＫだ。
    - firmware-Dは仮想COM:ポートがWindowsのデバイスマネージャーから見えたり見えなかったを繰り返す。
    - teratermからの操作は、まだちゃんと動いていない。

<!-- dummy comment line for breaking list -->

- ちなみに、Gainer-mini(本家サイト配布の最新firm)もあまり安定動作とは言えない状態だったので、もしかしたら、AE-18F2550がハズレなのかもしれない。
    - Writer509で書くときも、「Dev.IDが違います」のエラーすることがたまにあるし、書き込めても、fuseの0006がベリファイエラーする。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 安定化方法
千秋ゼミさんのUBW QAのページを熟読する。
- [http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?memo%2FUBW_QA01](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?memo%2FUBW_QA01) 

<!-- dummy comment line for breaking list -->

- USB1.1HUBを繋いでいたのが悪いらしい。MchipUSBはUSB2.0規格なので、USB1.1HUBを許容しないらしい。
    - ↑そのこと自体、なんか腑に落ちないけれど。2.0は1.1の上位互換だし、HighSpeedがないので1.1相当とほぼ同じ規格になっているはずなので。
- ハブ無し直結に戻したら、firmware-Dがちゃんと動くようになった。
- しかし、firmware-Bが不安定で、'fsusb -l' によるPICデバイス表示が点いたり消えたりする現象は全く改善されていない。
    - firmware-Bはたまにしか認識しないので、bootloaderを使うより、普通にPIC Writerを使うほうが遥かに時間の節約になるような感じだ。

<!-- dummy comment line for breaking list -->

~
- その後、別のマシン(i815,UHCI)で試してみた。
    - ハブ無し直結でfirmware-D用のMCHP-USBドライバーのみを入れて（つまり、libusbのフィルタードライバーは一切入れずに）試してみた。
    - やはり、COMポートが現れたり消えたりを短時間で繰り返すようだ。
    - 自作基板を装着せずにAR-18F2550単体でUSB接続した場合には、デバイスマネージャーにUSB謎の機器が表示されたりされなかったりを短時間で繰り返すようだ。
    - まるで定期的にリセットされているか、watchdogによるリセットが掛かっているような感じ。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## 解決？
- いろいろやっていたら、安定動作させるこつが掴めた。
- ６ＰＩＮのＩＳＰコネクタのうちで5V以外のピンを導電性スポンジでゆるく導電させておくことで

<!-- dummy comment line for breaking list -->
２台のＰＣのどちらもfirmware-Dを安定動作させることが出来るようになった。
- ヒントは、ＬＥＤ(RC0)の点滅にあった。
- ６ＰＩＮコネクタを指で触れると、ＬＥＤが激しく反応するのだ。
- 指で触れないにしても、mini-BコネクタをわずかにこじらすだけでＬＥＤの点滅の変化が起きたりしていたので、最初はmini-Bの接触不良を疑っていた。
    - そこで、miniBケーブルを取り替えてみたり、コネクタの締め付け具合を弄ってみたりしたが全く改善されないので不思議に思っていたわけだ。

<!-- dummy comment line for breaking list -->

- つまり、このAE-18F2550は、ISP端子の静電気に弱いということか。
- ----> LVPでプログラミングされたPICのPGMピンは常にプルダウンしなければ駄目っぽいです。

<!-- dummy comment line for breaking list -->

- 念のためテスターでMCLR(RESET)の電位とRC2(スイッチ)の電位を調べてみたが、こちらはちゃんとプルアップされていることは確認した。

<!-- dummy comment line for breaking list -->

**対策としては**
- RB5,RB6,RB7(PGM,PGC,PGD)を適当な抵抗でプルダウンする。
- プルアップではだめなようだった。
- RB5(PGM)プルダウンだけでもＯＫ。

<!-- dummy comment line for breaking list -->

この対策により、RC2をGNDに落としながらのリセット操作により、RC0,RC1のＬＥＤが安定に交互点滅を繰り返すようになった。
- 今までは、うまくいっても２，３秒だけ点滅を行ってそこで止まっていた。

<!-- dummy comment line for breaking list -->

**根本的な対策としては**
- LVP(５Ｖ書き込み)モードを使わずにHVP(高電圧書き込み)でbootloaderを１回書いてしまえば、RB5も自由に使えるようになり、安定するとのこと。

<!-- dummy comment line for breaking list -->


- ~~あとはlibusbのフィルタードライバーを飼いならすのみ？。~~

<!-- dummy comment line for breaking list -->

    - MyBootを使うことで、不安定なlibusb-filter-driverは使わなくても良いようです。

<!-- dummy comment line for breaking list -->


- - - -
## ＨＶＰ書き込みアタッチメントの制作
- ツェナーDiのかわりは、200個600円(@3円)で買った秋月2SC1815-GRのE-B間が約８Ｖだったので、そのまま実装してみた。

<!-- dummy comment line for breaking list -->

- ところで、LVPとHVPは書き込みソフトから切り替え出来るのだろうか？
- 制御ピンとか異なっているし。自動認識は無理だよね。--->自動認識でした。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## 続々：[PIC入門](PIC18F2550.md)
- 昇圧アタッチメントは完成した。

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/hvp.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/hvp.jpg) 



- でんし研さんのファームは全て自動認識するようだ。
- Writer509のソフトw509.exeにて、正しく操作できた。
- 昇圧アタッチメントを噛まさない場合は、LVPライターとして使える。

<!-- dummy comment line for breaking list -->

素晴らしい -- **とても便利**


- でんし研さん、ありがとうございました。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/writer.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/writer.jpg) 

- 早速AE-18F2550にFirmware-BをHVPで焼いて、MyBootを立ち上げてFirmware-Dをインストールした。
    - 大成功！
    - Firmware-Dはsdccでコンパイルしたものだが、ちゃんと動いた。

<!-- dummy comment line for breaking list -->




~
~
~
~
- - - -
## 参考リンク

FSIJ~
PIC 18F2550 ¶~
秋月電子で18F2550のボードが売られてるのでこれで遊ぶ環境をGNU/Linuxで整備しようという話。 
- https://members.fsij.org/trac/codefestweek2008/wiki/PIC18F2550

<!-- dummy comment line for breaking list -->

アルゴ算法堂:PIC18F概要
- [http://homepage2.nifty.com/sampodo/craft/pic18_01.html](http://homepage2.nifty.com/sampodo/craft/pic18_01.html) 

<!-- dummy comment line for breaking list -->

Porting Microchip MCHPFSUSB v1.3 CDC to use sdcc on Linux: C18のソースをsdccに移植する方法
- [http://www.efn.org/~rick/work/MCHPFSUSB/](http://www.efn.org/~rick/work/MCHPFSUSB/) 

<!-- dummy comment line for breaking list -->

semifluid:かなりのPICマニア？18F2550自作オシロスコープなど。
- [http://www.semifluid.com/](http://www.semifluid.com/) 

