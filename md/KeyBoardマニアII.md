[2009-05ARM](2009-05ARM.md) 

目次



~
~
- - - -
## 昔のAppleADBキーボードをPS/2に変換するアダプタ

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/kbsmall.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/kbsmall.jpg) 

## DOWNLOAD（ＡＤＢ）
ソース,HEXファイル一式: [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/adb-ps2-adapter.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/adb-ps2-adapter.zip) 

- 仕様：
- ATtiny2313 内蔵ＲＣ発振モード（８ＭＨｚ） を使用します。
- 外付け部品は電源のパスコン(0.1uF)１個のみです。
- PS/2側２線とADB側１線はＡＶＲチップ直結と、最小限の配線量で完成します。
- プルアップ抵抗は省略しています。（ＡＶＲの内部プルアップで代用）
- 記述はAVR-gccです。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## キーボ−ドマニア・リベンジ

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pc88-1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pc88-1.jpg) 

元ねた：
- [http://hp.vector.co.jp/authors/VA000177/at88/index.html](http://hp.vector.co.jp/authors/VA000177/at88/index.html) 

<!-- dummy comment line for breaking list -->


## DOWNLOAD（ＰＣ８８０１）

ソース,HEXファイル一式: [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/pc8801.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/pc8801.zip) 
- 仕様：
- ATtiny2313 内蔵ＲＣ発振モード（８ＭＨｚ）です。
- 別途ＴＴＬ ＩＣを使用しています。 74LS158（8 to 4 inverted selector）
- 記述はAVR-gccです。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -


**以下は制作記です。**


- - - -
## ADB to PS/2変換器制作の軌跡
- 昔のAppleADBキーボードをPS/2に変換するアダプタ をAVRで作ってみようと思う。

<!-- dummy comment line for breaking list -->

- 昔PIC 16F84Aで作った奴は、互換機のBIOSモードとか生DOSではちゃんと動いた。
- WindowsやLinuxでは動かない。
- プロトコルに問題があるようだが理由がまだわからない。

<!-- dummy comment line for breaking list -->

調査
- Windowsが起動している状態でPS2キーボード端子を差し替えてAppleADBに差し替えるとちゃんとキー入力できる。
- Windows起動前に差し替えるとキー入力出来ない。
    - これは、リセットシーケンスのようなものが来て、うまく反応出来ずに未接続にされてしまうような感じ。

<!-- dummy comment line for breaking list -->

~
    - AVRで仮組みした奴だと、ここは乗り越えるのだが、
    - 可能性としては、リセット後の待ち時間問題。

<!-- dummy comment line for breaking list -->

~

    - AVRで仮組みした奴でも、少し改造するとうまく行かない。
    - これは原因不明
    - 別のマシンでは動いた。

<!-- dummy comment line for breaking list -->

- 結論的には、RESETコマンド後のリプライを600mS後に遅らせることで、
- キーボード未接続になる問題が回避される公算大。（要確認）

<!-- dummy comment line for breaking list -->

実験結果
- PICで書いたものは、RESETコマンド後の待ち時間をどのように変えても認識しない。

<!-- dummy comment line for breaking list -->

- AVRで書いたほうは、一応大丈夫っぽい気がする。

<!-- dummy comment line for breaking list -->

- 双方にはプロトコル解釈上の見解が大きく異なる。

<!-- dummy comment line for breaking list -->

今のところ決め手無し。

~
~
~
- - - -
## PICで書かれたADBtoPS2変換機のソースの大部分を移植してみた。
- けれど、まだ動かない。
- PS/2インターフェースは生きている。
- ADB側が死んでいる模様。---調査中
- 最初ADB側のGND配線が微妙に抜けていて給電されていなかった。---修正した。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## 一応、全機能を実装。
- してみたが、BIOSリセット後にキーボードが認識されなくなる問題が残った。

<!-- dummy comment line for breaking list -->

- 回避策としては、１秒に１回ダミーキーを送るとか、手動でキーを打てば回避できる。

<!-- dummy comment line for breaking list -->

- たまたま、電源投入直後はADB側の問題でダミーキーが押されっぱなしになる（？）ので回避できている。
- Windowsの起動時のキーボード切り離し問題は結局起きなかった。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## まとめ：とりあえず、ADB to PS/2 変換器は完成した。
- PIC 16F84A に実装していたもの相当の機能に関しては実装済み。
- 一応動作する。
- 少し古めのマザー(i815E)では「BIOSリセット後にキーボードが認識されなくなる問題」は起きない。
- Windowsの起動時のキーボード切り離し問題はマザーの種類に関係なく、起きなかった。
    - PIC版では常に起きていて、結局原因は不明のままだ。

<!-- dummy comment line for breaking list -->


~
~
- - - -

現状の問題点
- 新し目のマザーでは「BIOSリセット後にキーボードが認識されなくなる問題」が起きる。
- このため、対策として
    - RESETコマンドを受け取るまでは、１秒に一回程度の頻度でPC側にACK(0xfa)を送り続けるようにして回避した。
    - 但し、Windows再起動や手動リセットでの起動ではACKを送るモードに戻せないので、
    - BIOS起動後２秒くらいは適当にキーを押していないと「BIOSリセット後にキーボードが認識されなくなる問題」

<!-- dummy comment line for breaking list -->
が起きてしまう。

- とりあえず、古いマザーで使うので、いいんだけど・・・。

<!-- dummy comment line for breaking list -->

~
つまり
- 新し目のマザー（foxconn製G31マザー）では、RESETコマンドが来る以前にキーボード未接続

<!-- dummy comment line for breaking list -->
と見なされるような挙動（？）があって、
- 純正（？）のPS/2キーボードでは、回避されている（？？？）

<!-- dummy comment line for breaking list -->

という謎が浮かび上がっている。

