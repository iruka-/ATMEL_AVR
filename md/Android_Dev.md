[Android](Android.md) 

Old Topic: See [Android_x86](Android_x86.md) 


- - - -

やっと今頃になってAndroidの開発環境を用意してみる。

- 基本的は、Oracle JDK7u55(Java)、Google ADT(SDK本体)、AndroidStudio(統合環境)、Genymotion(高速エミュレータ)を入れる。
    - Eclipseでの開発はもう古い。
    - AVDは遅くて使い物にならないので、実機端末あるいはGenymotionを使う。

<!-- dummy comment line for breaking list -->

~
- 開発環境はWindows8(64it)もしくはLinux(64bit)を使用する。

<!-- dummy comment line for breaking list -->
~
~
~
**目次**




~
~
- - - -
## Android SDKをWindows8に入れる(2014-6月の環境)

1. Android本家からSDK(adt-bundle-windows-x86_64-20140321.zip) をダウンロードしてきて展開する。
    - [http://developer.android.com/index.html](http://developer.android.com/index.html) 
    - 他のOS用はOTHER PLATFORMというところをクリックすればよい

<!-- dummy comment line for breaking list -->
1. Oracle本家から Java SDKを入れる
    - [http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html) 
    - jdk-7u60-windows-x64.exe
    - 安定板はバージョン末尾が5の倍数になっていて、前回は55、今回は60、らしい。

<!-- dummy comment line for breaking list -->


~
~
## SDK Managerの起動
- <インストール先>/sdk/tools/android.bat を起動する。
- 起動したら、
    - tools -> □Android SDK Platform-tools にチェックを入れる。
    - Android端末のサポートすべき主要なバージョン、例えば □Android 2.3.3と□Android 4.0.3 にチェックを入れる（任意）
    - 右下のinstall ** package ボタンを押す。
    - License Agreeラジオボタンを入れる。
    - インストール

<!-- dummy comment line for breaking list -->

~
~
## 端末エミュレータ(Android Virtual Device)の設定
- SDK Managerから Manage AVDs を選ぶ
- 適当にスペックを選んで作成。
- 普通に作るとARMエミュレーションになるので遅い。
- intel x86バイナリーで作成すると、（エミュレータの起動が）少しだけ速い。
- 純正品のAVDは起動やら何やら遅くていらいらするので、後述のGenyMotionを使ったほうが良い。

<!-- dummy comment line for breaking list -->

~
~
## 実機Android端末を繋ぐ。
- 適当な端末を用意する。
- 端末側のUSBデバッグを有効□にチェック。
- PCにつなぐ。
- adb(デバッガ)用のUSBドライバーを導入する。（これが面倒）
    - まず、端末メーカーごとにUSBのVID:PIDが違うので、各メーカーのサイトから、adb用のUSBドライバーをダウンロードしなければならない。
    - USBドライバーと言っても、実体はWinUSBなので、infがあれば十分(googleのinfを使いまわし)な気もするけれど、最近のWindowsはinfの署名catが正しくないと撥ねられる。
    - USBドライバーが正しく導入(デバイスマネージャーで!マークが付かないこと)出来ても、adbがデバイスを見つけてくれない場合は、
    - 手動で、C:/Users/ユーザー名/.android/adb_usb.ini に、該当デバイスのVIDを１行追記する必要があるらしい。
    
    		0x2207
    - とかそんな感じ

<!-- dummy comment line for breaking list -->


参考URL:

よく使うadbのコマンド 
- [http://qiita.com/t2low/items/cb37cec5f864c4748e14](http://qiita.com/t2low/items/cb37cec5f864c4748e14) 

<!-- dummy comment line for breaking list -->


~
~
adbはデバッガではないけれど、OSやアプリのログをプリントする機能がある.
	D:> adb logcat -c   ＝過去ログ消去
	D:> adb logcat　　　＝ログをコンソールに吐く


~
adbは本当はシェルではないけれど、Android端末（LinuxのBusyBoxシェル）に接続してコンソールとして使える
	D:> adb shell
- 見かけ上は組み込みLinuxのBusyBox的な、使いにくいシェルが使える。

<!-- dummy comment line for breaking list -->

~



## PC上のAndroidエミュレータ GenyMotion について。

PC上でのAndroid開発やテスト動作に使用できる。

GenyMotion
- [http://www.genymotion.com/](http://www.genymotion.com/) 

<!-- dummy comment line for breaking list -->

Android-x86 (intelアーキテクチャー用にビルドされたAndroid OS)をさらにカスタマイズして
Oracle Virtual-Boxと統合してしまったようなAndroid VM環境動作ソフト。

Android 標準のSDKについてくるエミュレータでもx86ビルドのバイナリーは提供されているけど、
ここまで使いやすくはないし、（ARMバイナリーにくらべるとかなり速いんだけれどそれでも）遅い。

~
~
~

~
- 現時点(2013-11)ではGoogle Play (gapps) がインストールされたVM環境はダウンロード出来ない。
- （ちょっと前までは with googleappsのVMもＤＬ可能だった）

<!-- dummy comment line for breaking list -->

- ARM-Translation(x86ビルドでもARMバイナリーなアプリを動かす)とgapps(GooglePlay)の入手は、以下のＵＲＬの下のほうから。

<!-- dummy comment line for breaking list -->

- [http://stackoverflow.com/questions/17831990/how-do-you-install-google-frameworks-play-accounts-etc-on-a-genymotion-virtu/17856453](http://stackoverflow.com/questions/17831990/how-do-you-install-google-frameworks-play-accounts-etc-on-a-genymotion-virtu/17856453) 

<!-- dummy comment line for breaking list -->

- ただし、Android側の動作がかなり不安定になるので、開発用のVirtualインスタンスには入れないほうが良い。(インスタンスはいくつでも作れる)

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
## Eclipseの日本語化
- [Pleiadesプラグイン](http://mergedoc.sourceforge.jp/) を導入する

<!-- dummy comment line for breaking list -->

- Pleiades.zipを展開して出来たものを、ディレクトリ構造そのままでeclipseインストール先にコピーする。
- Pleiades.zipのReadMeに従って、eclipse.ini に１行追記する。
- Eclipse再起動。(-cleanオプションしたほうが良い)

<!-- dummy comment line for breaking list -->

~
## EclipseにGenyMotionプラグインを入れる.
- Eclipseを起動
- ヘルプ(H)->新規ソフトウェアのインストール を選択
- 作業対象(W): サイトを入力または選択 のところに以下のURLを入れる

		http://plugins.genymotion.com/eclipse
- □Genymobile にチェック
- ライセンスに同意してインストール

<!-- dummy comment line for breaking list -->

~
## Eclipseで、サンプルソースをビルドしてみる.

・・・という、たったこれだけのことが出来ないのだよ。（実は、ずーーーっと出来なかったのだよ）
- SDKマネージャーを起動して、Android 4.4.2(API 19)の□ Samples for SDK にチェックを入れる。
- Eclipseを起動して、
- ファイル->新規->その他   を選ぶ（って、一体どんなとこにあるんだよ）
- 「ウィザードを選択」ウィンドウが出る筈
- その中のAndroid +-- Android サンプル・プロジェクトを選ぶ
- 試したいサンプルを選んで、あとはビルド＆ラン
    - たいがいはエラーしますなぁ・・・脳味噌バグってるんじゃないのか？

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
## apkの解析方法
- apkファイルは、拡張子をzipに変えて展開するだけで、中身のファイルが取り出せる。

<!-- dummy comment line for breaking list -->

~
Androidアプリの配布パッケージapkの解析について :CodeZine
- [http://codezine.jp/article/detail/6992](http://codezine.jp/article/detail/6992) 

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
## Android Studio について

- 従来のEclipseに代わる、Android専用の統合開発環境のこと。Javaで書かれている。

<!-- dummy comment line for breaking list -->

グーグル、Androidのための統合開発ツール「Android Studio」発表。オープンソースで無償提供。Google I/O 2013

- [http://www.publickey1.jp/blog/13/androidandroid_studiogoogle_io_2013.html](http://www.publickey1.jp/blog/13/androidandroid_studiogoogle_io_2013.html) 
- GenyMotionを使用する場合は、専用のPluginが必要。

<!-- dummy comment line for breaking list -->

- メニューのFileから --> Settings --> Plugins --> 下の「Browse repositories...」ボタン を押す。
- ネット経由で Genymotion を検索して 「download and install」

<!-- dummy comment line for breaking list -->




~
- 公式からダウンロードしたものはやや古いので、Studio メニューの Help-> Check for update でアップデートを入れる。
- Javaなので、JDKさえ入れていれば当然Linuxでも使える。
- Linux(64bit)上で使う場合は、32bitのELFが混じっているので32bitのlibを入れておかないといろいろツール呼び出しでエラーする。

		# apt-get zlib1g:i386
		# apt-get g++-multilib
- 32bir ELF問題はEclipseでも起こる。

<!-- dummy comment line for breaking list -->






~
~
~
~
- - - -
## AndroidStudioの日本語化
- AndroidStudioはIntelli-JというJava統合環境で動いている。
- なので、Intelli-Jの日本語化と同様の方法で日本語化出来る、らしい。

<!-- dummy comment line for breaking list -->

以下のURLを参考にする
- [http://tech.furyu.jp/blog/?p=1917](http://tech.furyu.jp/blog/?p=1917) 

<!-- dummy comment line for breaking list -->

~
やり方は
- Eclipseを日本語化したのと同様に、[Pleiadesプラグイン](http://mergedoc.sourceforge.jp/) を導入する。
- Pleiadesプラグインのzipファイルを展開して、pluginsディレクトリに生成されたもの

		/jp.sourceforge.mergedoc.pleiades/
- をディレクトリごと、AndroidStudioのplugins/ にコピーする。
- AndroidStudioのbin/studio64.exe.vmoptionsの最終行に一行追記する。

		-Didea.platform.prefix=AndroidStudio
		-Didea.paths.selector=AndroidStudioPreview
		・・・
		-javaagent:../plugins/jp.sourceforge.mergedoc.pleiades/pleiades.jar
- AndroidStudioを起動する。

<!-- dummy comment line for breaking list -->




~
~
~
~
- - - -
## アプリケーションの基礎

用語集
- [http://www.techdoctranslator.com/android/basics/fundamentals](http://www.techdoctranslator.com/android/basics/fundamentals) 

<!-- dummy comment line for breaking list -->


サンプルコードの入手
- [http://www.techdoctranslator.com/resources/samples/get](http://www.techdoctranslator.com/resources/samples/get) 

<!-- dummy comment line for breaking list -->
~
~
~
~

~
~
~
~

