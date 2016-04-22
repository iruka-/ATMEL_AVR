*Android-x86 [#db9d191a]
- [http://www.android-x86.org/](http://www.android-x86.org/) 

<!-- dummy comment line for breaking list -->

- ２０１０年８月にandroid v2.2 froyoのisoが公開されている。
- このisoは遅くて使えないので、自分でビルドしたものを動かしてみるテストです。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun.jpg) 

- - - -

- isoファイルから[vmware-player](http://www.vmware.com/jp/products/player/) にインストールするのが面倒な場合は、下記vmxファイルを使用できます。

<!-- dummy comment line for breaking list -->

DOWNLOAD

- [http://cid-e79d5ff1a8049089.office.live.com/browse.aspx/Android22-vmplayer](http://cid-e79d5ff1a8049089.office.live.com/browse.aspx/Android22-vmplayer) 

<!-- dummy comment line for breaking list -->

    - 約３５ＭＢ×２ファイルに分割しています。
    - [7zip](http://sevenzip.sourceforge.jp/) で圧縮されています。
    - 展開すると、以下のファイルが出来上がります。
    
    		Directory: Android22
    		           8,684 Android22.nvram
    		     190,775,296 Android22.vmdk
    		               0 Android22.vmsd
    		           2,209 Android22.vmx
    		             264 Android22.vmxf
    		             248 ReadMe.html
    - 中身はvmxファイルですので、VirtualBoxでは使用できません。
    - インストールはsda1=2G,sda2=6Gでパーティションを切っています。
- デフォルトインストール直後の状態から、
    - 言語を日本語にしています。
    - 谷歌IMEをOFF設定にしています。
    - 「提供元不明のアプリ」の許可ビットをチェック済みです。
    - AndAppStoreのクライアントのみインストール済みです。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 使い方
- （１）まず、[VMWarePlayer](http://www.vmware.com/jp/products/player/) (無償)3.0以上を入手、インストールします。
- （２）**Android22.7z.001**と**Android22.7z.002**を上記downloadサイトから入手します。
- （３）[7zip](http://sevenzip.sourceforge.jp/) を使用して展開すると、以下のファイルが出来上がります。

		Directory: Android22
		           8,684 Android22.nvram
		     190,775,296 Android22.vmdk
		               0 Android22.vmsd
		           2,209 Android22.vmx
		             264 Android22.vmxf
		             248 ReadMe.html
- （４）展開されたファイルのうちの、Android22.vmxを開くとvmware-playerが起動します。
- （５）仮想マシンはコピーされました。を選択して起動します。
- （６）Grubメニューが出てきたら、一番上のメニューを選んで起動するか、そのまま１０秒待つだけでも起動します。
- （７）起動したら、端末ロックのような画面になりますので、下の棒（鍵アイコン）を上方向にマウスドラッグして解除します。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun2.jpg) 

- LANがうまく接続されていないときは、VMWare-Playerのネットワーク設定を疑うか、Windowsのファイアウォール設定の「例外」を許可し、「例外プログラム」にVMWare-Playerを追加します。

<!-- dummy comment line for breaking list -->

## Android-x86特有のキー操作

|Linuxのコマンドライン(#)CUIに切り替える|[ALT]+[F1]|
|コマンドライン(#)からGUIモードに復帰する|[ALT]+[F7]|
|電源OFFのメニューを出す|[End]|
|通話履歴|[ALT]+[F3]|
|戻る|[ESC]|
|HOME|[HOME]|
|MENU|[MENU]または[マウス中ボタン]|
|カーソル移動|カーソルキー|

~
その他
|アイコンをデスクトップに置く|アイコン画面の中のアイコンをマウス左ボタンで長押ししてデスクトップへドラッグ|
|アイコンをデスクトップから移動|デスクトップ画面の中のアイコンをマウス左ボタンで長押しして移動、ゴミ箱で消去|
|壁紙|デスクトップ画面のやや下のほうで、マウス左長押しか、マウス中長押し|

~
~
~
- - - -

