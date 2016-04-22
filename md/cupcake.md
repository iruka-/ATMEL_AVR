[2009-06](2009-06.md) 


- ---> [Android](Android.md) の項目を参照すること。
- cupcake は放置プレイ中

<!-- dummy comment line for breaking list -->

~
~
~
これは、古い情報


## Live Android v0.3

- [http://code.google.com/p/live-android/](http://code.google.com/p/live-android/) 

<!-- dummy comment line for breaking list -->

- [http://twitter.com/liveandroid](http://twitter.com/liveandroid) 

<!-- dummy comment line for breaking list -->

VMWarePlayerで試しているが、そろそろ見切りをつけようかな。


- 1024x768はマウスが出ない。そしてさらに不安定。
- 左のゲイツキー(ミ田)＋'Ｆ'でタスク切り替えが出来る。（画面効果があるので遅い）
- いろいろさらに不安定になった。ほっておくと死んだりする。
- サクサク感がなくなった。反応が鈍くなった。
- スーパー丸尾みたいなエミュレータゲームが入っているがつまんない。

<!-- dummy comment line for breaking list -->

- ５１２Ｍのメモリーカードが使えるらしいのだが、うまく使えない。（たぶんなんか設定がいるんだろうね）

<!-- dummy comment line for breaking list -->

結論：
- 残念だが、これは実用には到底使えない。ビルドごみだと思ってもらっていい。
- 実用の定義＝Androidの開発評価用。Androidアプリのお試し用。NetBookなどで簡易的にWeb見たりメール書いたりとか。

<!-- dummy comment line for breaking list -->






~
~
~


## Live Android v0.2USB


Ver0.2 USBが出ています。

- [http://code.google.com/p/live-android/](http://code.google.com/p/live-android/) 

<!-- dummy comment line for breaking list -->

- [http://twitter.com/liveandroid](http://twitter.com/liveandroid) 

<!-- dummy comment line for breaking list -->

- 落とし中・・・

<!-- dummy comment line for breaking list -->

- MD5SUMは合っている。

<!-- dummy comment line for breaking list -->

- VMWarePlayer上で動作せず。 Splash Screenのまま。かれこれ３分経過。

<!-- dummy comment line for breaking list -->

- vmxはbrowser-applianceのやつと、twitter.comにあったやつの両方で試した。
- もちろん USBデバイスはVMWareに与えている。

<!-- dummy comment line for breaking list -->

    - つーかUSBは普通のFATフォーマットで空き容量があればいいんだろうな？
    - >それともisoをddしなければいけないのかそれすら不明。

<!-- dummy comment line for breaking list -->

- ALT+F1効かない。

<!-- dummy comment line for breaking list -->

- **どっかにログ吐けよって感じ。**

<!-- dummy comment line for breaking list -->

そろそろ見切りをつけるべきか？？？


## Livedroid alpha-3
livedroid: yet another android livecd
- [http://sourceforge.jp/projects/livedroid/](http://sourceforge.jp/projects/livedroid/) 
- VMWareに突っ込んでみたけれど、netcfgが無いと言われてinitが起動しない。
- E,J,Sの３択のところで、Sを選ぶと起動するけれど、これってstartxとか出来ないよね。＜できるわけねーじゃんバカっ!!

<!-- dummy comment line for breaking list -->

kinnekoさん、がんばれっ！ ＜エールを送るよっ。

~
で、Androidをブートするたびに思うんだけれど、こんな作りかけの組み込みLinuxなんか使うよりも
もっと気の利いたLinuxのディス鳥使ったほうがよくね。
- あと、ChromeブラウザとDalvikの動作環境があれば、べつにGoogleOSなんかいらない。

<!-- dummy comment line for breaking list -->





## Live Android


Ver0.2が出ています。

- [http://code.google.com/p/live-android/](http://code.google.com/p/live-android/) 

<!-- dummy comment line for breaking list -->

- VMWare内で動作するらしいです。--動作しました。
- 実機(Pen4とか)で起動しますが、たいていLAN(eth0)が使えません。(たまにちゃんと使えます)
- 実機で、起動時にマウスが使えない場合はBattery警告から抜けることも出来ず何も出来なくなります。
    - 確率は1/2くらい？実機がロートルだから？

<!-- dummy comment line for breaking list -->

- google検索で最初の数文字を入れたところで検索候補の表示が出ますが、（不思議なくらい）めっちゃ速いです。
    - 内蔵のDBをダウンロードしているのかと思うくらい。

<!-- dummy comment line for breaking list -->

- SDカードが認識できないのでかなり不便です。

<!-- dummy comment line for breaking list -->




Updated Jul 12 (4 days ago) by liveandroid  

todo   
Introduction
please follow us: [http://twitter.com/liveandroid](http://twitter.com/liveandroid)  

## TODO
[http://code.google.com/p/live-android/wiki/todo](http://code.google.com/p/live-android/wiki/todo) 
	 v0.3
	  ・liveUSB 
	  ・SD card 
	  ・Audio 
	  ・support more net cards 
	  ・higher graphics resolutions 
	  ・easy netconfig 
	  ・wifi (akl5k, tl9k) 
	  ・change HOME/MENU key on Mac 
	  ・a way to add files to the system 
	  ・support marketplace (?) 
	  ・integrate debian with android (?) 
	  ・a simple hd-install version (?)  

- なんか夢みたいな寝言が列挙って感じ。

<!-- dummy comment line for breaking list -->

自分的な寝言としては、
- Linuxエミュレータ(busybox,ssh,microGnuEmacs or jed,mutt,lunx or w3m)って感じ。
    - これじゃあ、ＣＵＩだけで完結するよ。
- あと、ARMなマシンは持ってないのでいらない。~

<!-- dummy comment line for breaking list -->
（それなりに組み込みLinuxは難易度が高そうなので、型落ちＰＣで遊べればいいや）
- そのうちカーネルがLinuxからbsdになるんでないの？

<!-- dummy comment line for breaking list -->

- ＧＰＬを嫌っているようなので、GNU/Linuxじゃなくて Apache/Linuxって感じか。

<!-- dummy comment line for breaking list -->


~
~
~

- - - -

以下は古い情報


## Android:cupcakeをVMWare上で動かす方法がすこし分かりました

IT Pro:第1回　話題の携帯向けOS「Android」をx86パソコンで動かしてみよう 
- [http://itpro.nikkeibp.co.jp/article/COLUMN/20090219/325052/](http://itpro.nikkeibp.co.jp/article/COLUMN/20090219/325052/) 

<!-- dummy comment line for breaking list -->

- cupcakeのビルド方法は、上記リンクの通りに行えば、たぶんうまく行きます。

<!-- dummy comment line for breaking list -->

- でも、わざわざビルドしなくても上記ページの最後にあるUSBイメージをＤＬ

<!-- dummy comment line for breaking list -->
すれば、同じもの(android-usb.img)が得られるはずです。

- ただ、この(android-usb.img)をVMWareを使ってＰＣ上で動かすのにとても難儀しました。

<!-- dummy comment line for breaking list -->

これはそのメモです。

## android-usb.imgをＰＣ実機上で動かす方法
以下のページからＤＬできるDD for Windowsを使うか、~
稼動しているLinux上で本物のddを使って、android-usb.imgをUSBメモリーに書き込みます。
- [http://www.si-linux.co.jp/wiki/silinux/index.php?DDforWindows](http://www.si-linux.co.jp/wiki/silinux/index.php?DDforWindows) 
- ddを使ってAndroidを書き込んだら、元のUSBメモリーの中身は完全に消滅しますのでご注意ください。

<!-- dummy comment line for breaking list -->

- このUSBメモリーを実機ＰＣに差し込んで、ＰＣのBIOS設定をいじり、USBメモリーからブートするようにします。

<!-- dummy comment line for breaking list -->

- リセット後、運がよければブートします。
- 運が良い場合には

		/dev/sda ---&gt; 内蔵SATA HDD （但し１台のみ実在）
		/dev/sdb ---&gt; USBメモリー
- という順にデバイスが割り当てられているはずです。
- 実際こうはうまく行かないので、（ＰＣによってはSATAかUSBは２択になっていて）

		/dev/sda ---&gt; USBメモリー
- の場合は以下のようにします。

<!-- dummy comment line for breaking list -->

USBメモリーをLinuxにマウントします。LinuxはVMWare上でもリアルマシン上でもどちらでも構いません。

- マウントできたら、

		/media/sdb1/boot/grub/menu.list  を編集します。
		~~~~~~~~~~~~ここは自分がマウントした位置.
		title Android
		root (hd0,0)
		kernel /boot/vmlinuz root=/dev/sdb1 init=/init rw vga=788 rootwait
		                               ~~~~
- 下線部をsda1 に書き換えます。

		# sync
- USBメモリーを unmount したのち、ＰＣに挿して起動させます。

<!-- dummy comment line for breaking list -->


## android-usb.imgをVMWare Player上で動かす方法

はっきりいって、激ムズでした。
- VM Imageを作るのが面倒だったので、VMWare Playerの公式サイトで配布している、Browser-Applianceという名前のubuntuのVM Imageを使います。

<!-- dummy comment line for breaking list -->

- Browser-Appliance.vmdk を android.vmdk にリネームします。
- これをいつも使っているubuntu-8とか9のvmxのディレクトリにコピーします。
- ubuntuのvmxを編集して、/dev/sdb1 にandroid.vmdkを割り当てます。

<!-- dummy comment line for breaking list -->

	scsi1.present = "TRUE"
	scsi1.virtualDev = "lsilogic"
	scsi1:0.present = "TRUE"
	scsi1:0.fileName = "android.vmdk"
	scsi1:0.redo = ""

- ただし、中身はまだ、古いubuntuのままです。

<!-- dummy comment line for breaking list -->

- VMWare Playerを起動し、普通にubuntu-8を起動します。
- USBメモリーを挿します。

		/dev/sdb1 --&gt; android.vmdk
		/dev/sdc1 --&gt; USBメモリー
- となっていることを確認したのち、 コンソールを開き、rootになります。

		# dd if=/dev/sdc of=/dev/sdb
		# sync
- とやって、USBメモリーのイメージを丸ごとandroid.vmdkにコピーします。
- VMWare Playerを一旦終了します。

<!-- dummy comment line for breaking list -->


- こんどは、android.vmdkを、元のBrowser-applianceの位置に戻します。
- そして、/dev/sda1/ に割り当てなおします。

<!-- dummy comment line for breaking list -->

	scsi0.present = "TRUE"
	scsi0.virtualDev = "lsilogic"
	scsi0:0.present = "TRUE"
	scsi0:0.fileName = "android.vmdk"
	scsi0:0.redo = ""

- こんどは、VMWare Playerを Browser-appliance側で起動します。

		/dev/sda1 --&gt; android.vmdk
		/dev/sdb1 --&gt; USBメモリー
- となることを期待します。（が、実際にはUSBメモリーが/dev/sda1になるようです。）
- VMWare PlayerのメニューでRESETを実行します。
- grubが起動したら、速攻でウィンドウフォーカスしてESCを押します。

<!-- dummy comment line for breaking list -->

- grubメニューを 'e' コマンドで編集します。

		root (hd0,0)
		kernel /boot/vmlinuz root=/dev/sdb1 init=/init rw vga=788 rootwait
		                               ~~~~
- 下線部をsda1に書き直して、'b'コマンドで起動します。
- 画面は真っ黒のままなので、３０秒ぐらいしてから、USBメモリーを一度抜いて挿しなおします。（ずっとVMWare側に割り当てたままにしてください）

<!-- dummy comment line for breaking list -->

- すると/dev/sda1をUSBにマウントしてくれて、Androidのsplashスクリーンが出ます。

<!-- dummy comment line for breaking list -->

- うまく行かないときは、grubメニューの'e'コマンドで、vga=788を一旦削除して、root=/dev/sda1 に書き換えて、練習します。（この場合はrootのマウントタイミングの練習にはなりますが、VGAのモードが違うのでkernel panicで止まります）

<!-- dummy comment line for breaking list -->


~
~
~
どうでしたか？
- どう考えても、**もっとスマートな方法が存在する**と思います。
    - 分かったら教えてください。

<!-- dummy comment line for breaking list -->

困っているのは、
- VMWareはUSBメモリーから直ブートしてくれないらしい。
    - なので、しかたなくイメージコピーのandroid.vmdkのほうからブートしている。
- Androidは、rootマウントを（なぜか）USBメモリー側でしか行ってくれない
    - 変ですねぇ・・・しかたなく実USBメモリーの挿し直しにより認識させてます。

<!-- dummy comment line for breaking list -->

- ＰＣ実機で起動した場合はＬＡＮに接続出来ない。
    - これはドライバーのconfigをきちんとやってLinux Kernelを作り直せばなおるはずです。
    - ただ、ＰＣ実機にはいろいろなNICがありますので、決め撃ちというやりかたは通用しにくいです。

<!-- dummy comment line for breaking list -->

そうそう、LiveAndroidのISOイメージのほうは、直接VMWareで動くのですが、
- [http://code.google.com/p/live-android/](http://code.google.com/p/live-android/) 
- こいつもLANに繋がらない・・・なんで？

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## それでもLANに繋がらない。
- [http://groups.google.co.jp/group/android-embedded-japan/browse_thread/thread/63eb93cc33dd0723](http://groups.google.co.jp/group/android-embedded-japan/browse_thread/thread/63eb93cc33dd0723) 

<!-- dummy comment line for breaking list -->

- android-usb.imgのほうはALT+F1でコンソールに落ちてこない・・・何故？
- androidv0.1.isoのほうはALT+F1でコンソールに落ちる。
    - eth0はupしている。IPアドレスは取得済み。
    - ローカルIP(192.168.0.*)からLAN内のマシンに対してはpingが通る。
    - しかしALT+F7してブラウザを起動し、[http://192.168.0.](http://192.168.0.) */ を見ようとしても見れない。
    - LAN外のマシンには一切pingが通らない。
    - route add default gw 192.168.0.1 みたいなことをやってもやっぱり通らない。
    - 名前は全然引けない。ってことは
    
    		# netcfg eth0 dhcp
    - してないってことか。（isoのほうはしていないっぽい。）
- 手動で

		# netcfg eth0 down
		# netcfg eth0 dhcp
- やってみたけど、upしそこねてコンソールもGUIもハングしてしまう。

<!-- dummy comment line for breaking list -->

**組み込みLinuxは難易度高いっすね。**

~
~
~
~
- - - -
## 続編
- [http://groups.google.com/group/android-porting/browse_thread/thread/0312b96ba82df314](http://groups.google.com/group/android-porting/browse_thread/thread/0312b96ba82df314) 
- ここのTipsに書いてあるようにやってみた。

		Tips
		 use alt+F1 or alt+F7 to switch GUI and console.
		 busybox added, you can do everything in console.
		 config networking in LiveAndroid via console
		Commands
		 IP: ifconfig eth0 yourip netmask yourip's mask
		 Gateway:  route add default gw yourgateway dev eth0
		 DNS:
		setprop net.eth0.dns1 yourDNS
- IPはifconfigによると、最初からOK.
- Gatewayは２回くらいトライしたけれど駄目っぽい。(VMWareかWindowsがwallしてるのかな？)
- DNSも当然駄目.(Gatewayが駄目なので)

<!-- dummy comment line for breaking list -->

だけど、[ついたーの中の人](http://twitter.com/liveandroid) によると、60MBくらいのisoとVM imageをリリースするのでもうちょっと待ってねということらしい。

	# ping 192.168.0.1
とかやると、^Cも何も効かないで、無限にpingするのはやめてほしかった。(というかsignal受け取らないのか？)


~
~
~
~
- - - -
## Android:VM Imageリリース
[ついたーの中の人](http://twitter.com/liveandroid) 参照
- android.rar 60MBくらい。
- 起動したものの、VMが別のマシンで作られていて、起動後スナップショットだったのでいろいろ不調。
- other 3.vmx を見ると、androidv0.1.isoのパスが思いっきり作者マシン限定で書かれていた。
- androidv0.1.isoをカレントディレクトリに持ってきてRESETを掛ける。
- ちゃんと起動する。
- ALT+F1でコンソールに戻す。

		# ifconfig
- でipが192.168.0.3のままだったらdhcpになっていない。

		# netcfg eth0 dhcp
- とやると、dhcpを取得してくれる。
- ALT+F7でGUIに戻る。

<!-- dummy comment line for breaking list -->

これで、**やっとＷｅｂが見れるようになりました。**


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/android.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/android.png) 

- 敗因：たぶんandroid.vmx のでっち上げ方に問題があったようだ。
- それ以外は全くandroidv0.1.isoのままなので。

