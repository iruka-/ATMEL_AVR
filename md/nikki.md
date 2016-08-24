# Android快適生活

- Android スマホやタブレットでGitに登録してあるソースコードを閲覧したい。
- 編集は一応可能かもしれないが、出来ないものと思って諦める。


- 主に電車の中や待合室等での時間つぶし用。
- とは言っても、そういう場所でこそ、いろいろ発見や思い付きが生まれる。
- ポケモンなんかで時間つぶすよりも、ソースコード見てるほうが落ち着くよ。
- 

## 用意するもの
- Gitリポジトリ　（ソースコードを登録してあるものと仮定）
- Android用gitクライアント
- Android用ソースコードビューワー

## Gitリポジトリ
- なんでもよい。GitHubでもよいし、お家サーバーとかRaspberryPiとかに置いてもよい。
- WiFiから快適にアクセス出来ればOk。

## Android用gitクライアント
- sgit がいい感じ。

https://play.google.com/store/apps/details?id=me.sheimi.sgit&hl=ja

- パスワード入力が面倒なので、サーバーで公開鍵作っておいてsgitに登録する。
- URLは ssh://ユーザー名@サーバーURL/ディレクトリ/リポジトリ名.git
- 公開鍵をsgitに入れておけばパスワードは空欄で良い
- sgitの保存ディレクトリはAndroid端末内のわかりやすいところに置いておくのが吉
- （後述のソースコードビューワーやエディタで読み書きするため） 
- なんだったら、 /sdcard/DCIM/ 直下でもいいぐらい。(USB接続時にPCから上書きできる)

## Android用ソースコードビューワー
- ソースコードビューワー がいい感じ。

https://play.google.com/store/apps/details?id=com.lugalabs.sourcecodeviewerfree&hl=ja

- 最初はコマンドラインでやろうとしたり、Webブラウザー＋Webサーバーでやろうとしたけど全部諦めた

## Happy Hacking!
