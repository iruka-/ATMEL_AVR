[2014-02](2014-02.md) 



## Ubuntu LinuxをWindowsXPのかわりに使ってみる。

- 2014年4月からWindowsXPが使えなくなるらしいので、
- かわりのOSとして、Ubuntu Linuxが使えるのだろうか、というテストです。

<!-- dummy comment line for breaking list -->

~

デスクトップOSとして、Linuxを導入するメリットとしては、
- （１）OSのライセンスコストが掛からない。(Windows8を導入する場合は１万3000円くらい掛かるけれどLinuxは無料)
- （２）ウィルス対策ソフトの導入コストも掛からない。（ノートンとかESETとかトレンドマイクロとかだと年間数千円×使用年数ボラレマス）
- （３）Linuxをちょこっと使いたいとき、わざわざVMWarePlayerを入れて、Windows内でLinuxを走らせる手間がいらなくなる。（笑）

<!-- dummy comment line for breaking list -->

~
逆にデメリットは、
- （１）DirectXのゲームが遊べない。(*全く*) スチームとかの新しいプラットフォーム上のゲームが出るのを待つしかない。
- （２）Win32のアプリが使えなくなる。MinGWとかCygwinとか使えない。でも本物のShellやgccやMakeが使える。
- （３）OSに関わる問題解決は全部、自力でやらなければならなくなる。MSなどのサポートが受けられない。

<!-- dummy comment line for breaking list -->

~
ということなので、条件としては
- ある程度Linuxに詳しい。あるいは、新しいものを試してみたい。
- メインマシンはWindows8だったりするけれど、XPが入ってた古いマシン等は、Webブラウジング程度出来ればよいので再利用したい

<!-- dummy comment line for breaking list -->

というダメモト精神で試してみることにします。




~
- - - -
## まず、Ubuntu Linuxを入手します。

- ここ( [http://www.ubuntulinux.jp/](http://www.ubuntulinux.jp/)  )から、ubuntu-ja-13.10-desktop-amd64.isoなどを入手してDVDに焼きます。
- ＰＣにインストールします。

<!-- dummy comment line for breaking list -->

~

- まっさらの状態でインストールしたばかりのUbuntu 13.10-ja

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/unity1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/unity1.jpg) 

## なんじゃこれー

- たいていのひとは、**このデスクトップを見て途方に暮れるか、使用を諦める**と思います。

<!-- dummy comment line for breaking list -->

~
~
~
#### さて、これをWindowsXPっぽく出来ないだろうか？

ということで、

- お手本サイト( [http://kei-efu.blogspot.jp/2014/01/ubuntuwindowsxp.html](http://kei-efu.blogspot.jp/2014/01/ubuntuwindowsxp.html)  )の完全受け売りなんですけど
- これから、MATEというデスクトップ環境を導入して、LunaXPテーマを入れます。
- 結果は、こんな感じです。

<!-- dummy comment line for breaking list -->


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/mate1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/mate1.jpg) 

## やりかたは、上記のサイトのとおりに行うだけですが、要点だけを書きます。

まず、以下のようなバッチを走らせて、MATEデスクトップに移行します。

	#!/bin/sh
	sudo add-apt-repository "deb http://packages.mate-desktop.org/repo/ubuntu saucy main"
	sudo apt-get update
	sudo apt-get install --allow-unauthenticated mate-archive-keyring
	sudo apt-get update
	sudo apt-get install mate-core
	sudo apt-get install mate-desktop-environment
	#
	sudo apt-get --purge remove gnome-session-bin
	sudo apt-get --purge remove gnome-session
	sudo apt-get --purge remove gnome-session-common
	sudo apt-get --purge remove gnome-session-fallback
	#
	sudo apt-get install lightdm-gtk-greeter 
	cd /tmp
	wget http://www.kkaneko.com/rinkou/linux/img/toja.sh
	sudo bash toja.sh
	sudo reboot

- mate-desktop.orgのサイトが重いので、ちょっと時間がかかるかもしれません。
- 所々で、インストール時の自分のパスワード聞かれたりとか、(Y/n)とか出ますが、yで良いです。
- 成功すると再起動して、UnityからMATEにデスクトップが変わります。

<!-- dummy comment line for breaking list -->

~
~

ちなみに、あの糞ったれUnity Desktopから、どうやってtermを立ち上げるかというと・・・
- 左に並んでいるアイコンの一番上の奴をクリック
- キーボードからtermと打つと、GnomeTerminalっぽいアイコンが表示されるので、それをクリック
- シェルスクリプトの打ち込みは、viでやってもいいけど大変なので、jedを入れてjedでやるのが良いかと。(emacsがよければそれでも可)

		$ sudo apt-get update
		$ sudo apt-get install jed
		$ jed mate.sh
		 ・・・編集・・・
		$ bash mate.sh

<!-- dummy comment line for breaking list -->
- - - -
#### 次に、

- 下のツールバーを右クリックして、消去します。
- 上のツールバーを右クリックして、プロパティを選んで、「下」に配置させます。
- 上記お手本サイトにある、LunaXPのテーマをダウンロードしてきて、tarで展開し、install.shを走らせます。
- LunaXPのテーマが導入出来たら、
    - タスクバー ->システム ->設定 ->外観の設定 で、LunaXPのテーマに切り替えます。

<!-- dummy comment line for breaking list -->

以上で終わりですが、漢字変換キーを ALT+~(grave)に替えておきます。やり方は
- タスクバー ->システム ->設定 ->「キーボード・インプットメソッド」 で
- 「次のインプットメソッド」の右にある「・・・」ボタンを選んで、
- キーボードショートカット -> 「キーコード」の右にある「・・・」ボタンを選んで、
- モディファイアーをALTにして、「~(grave)」キーを押して、「＋追加」で追加します。（ちょっとだけコツが要りますが）

		&lt;Alt&gt;grave;&lt;Super&gt;space;
- ちなみに、<Super>キーは、Windowsキー（田んぼマークのキー）のことです。

<!-- dummy comment line for breaking list -->

上記の操作をせずともUbuntuでは、田んぼマークのキー+SPACEキーで漢字/asciiが切り替わるはずなんですけど
- ubuntu13.10のUnityのバグで、<Super>space;キーは捨てられるので、ubuntuデフォルト状態では漢字が打てません。
- （マウス右クリックとか、IMEのボタンを探してマウスで切り替えることはもともと可能ではありますが）

<!-- dummy comment line for breaking list -->

~
~
~
ついしん
#### ウィンドウを最小化したときに、タスクバーに一覧を出すようにするには
- タスクバーの何もないところを右クリック->「＋パネルへ追加」->「ウィンドウの一覧」->追加

<!-- dummy comment line for breaking list -->

~
~
~
~
~
~
- - - -
## LUNA XP以外の方法

- 軽量のLubuntuを使う、という方法があります。
- [LubuntuLinux](http://lubuntu.net/) からisoを落としてきてインストールする方法と、
- 普通にUbuntu14.04LTSをインストールしたあとで、

		# apt-get update
		# apt-get install lubuntu-desktop
- というコマンドを打ち込んでLubuntuデスクトップに切り替える方法のどちらでもOKです。

