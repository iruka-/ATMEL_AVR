[3] genserial.awk (シリアル番号生成ツール)の使い方

HIDaspx 用のシリアルデータ（HEX ファイル）を生成するツールを用意しました。

　このツールは gawk(GNU awk というインタープリタ） を使って動作します。AVR マイコ
ンの利用者に広く普及している WinAVR には、gawk.exe が付属していますが、gawk を利
用できない方は、バグが少なく単独で動作する、木村さんの移植版 gawk を入手してくだ
さい。

2009年9月4日 gawk 3.1.7 日本語版バイナリバグfix済の最新版 GAWKの改良版の入手先
 ⇒ http://www.kt.rim.or.jp/~kbk/gawk-3.1/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20091124.zip

入手した gawk-mbcs-win32-20080704.zip を展開し、gawk.exe を hidspx.exe と同じディ
レクトリにコピーします。

 2007/10/22  15:50              280,351 ChangeLog
 2007/07/16  23:59               35,147 COPYING
 2008/07/02  16:47              189,434 mbc-diff
 2007/09/25  14:58               83,133 NEWS
 2008/07/03  16:51              729,088 gawk.exe
 2006/12/01  14:26               53,248 igawk.exe
 2008/07/03  17:22                9,588 readme.ja

2008-1028 版以降、「-v adr= 」を指定することで、任意のアドレスに対するシリ
アル情報を生成でき、また、複数のシリアル番号も瞬時に生成可能です。

usage: gawk -f genserial.awk -v t={aspx|mon88} start-number  [end-number]
￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

使用例1：
 > gawk -f genserial.awk 0001

のように実行すると、0001 のシリアル番号を設定する HEX ファイル（_0001.hex）
を生成します。

使用例2：0001〜0100のシリアル番号のファイルを一括して生成します。
         指定できる値は1〜9999まで、これを超えるとエラーになります。

 > gawk -f genserial.awk 1 100


使用例3：16進数や英大文字の指定も可能です。小文字は大文字に変換します。
※　文字列を大文字化している理由は、Windows のファイルシステムが、大文字小
  文字を区別しないことによるファイルの上書きを避けるためです。この方法でシ
  リアル番号が不足する場合は、生成するファイル名を工夫し、小文字でも重複し
  ないファイル名の生成を行う必要があります）

 > gawk -f genserial.awk ABCD

このファイルを main.hex のファームと同時に書き込めば、HIDaspx はそのシリア
ル番号を持ったものになります。

 > hidspx main.hex _0001.hex

※ 先頭文字に'_'を追加しましたので、以下のような操作が可能です。
 > del _*.hex
 で一括してシリアル番号用のファイルを削除できます。

使用例4：
 > gawk -f genserial.awk -v t=mon88 0001

bootmon88用のシリアル番号を生成します。

使用例5：
 > gawk -f genserial.awk -v t=1234 0001

1234(10進数)番地から始まる4桁のシリアル番号を生成します。

