*スタートメニューを出す [#c58eb64b]

- [ClassicShell](http://classicshell.sourceforge.net/) をインストールする。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## 不要な Metro アプリを消す。
- Shiftを押しながら、スタートメニューを開くことで、Metro画面に行く。
- アプリのパネルを右クリックする。
- 下に出てくるメニューでアンインストールを選択する。
- アンインストールできないものは、ピン止めを外す、を選べばとりあえず表示されなくなる。

<!-- dummy comment line for breaking list -->


~
~
- - - -
## 枠を細くする　＆　デスクトップアイコンを詰める


- PaddedBorderWidth0.reg

		Windows Registry Editor Version 5.00
		#
		[HKEY_CURRENT_USER\Control Panel\Desktop\WindowMetrics]
		"PaddedBorderWidth"="0"
		"IconVerticalSpacing"="-990"
		"IconSpacing"="-1020"
- 以上の内容のテキストファイルをPaddedBorderWidth0.regというファイル名

<!-- dummy comment line for breaking list -->
にリネームし、ダブルクリックする。
- サインアウトして再度サインインする。
- アイコンを整列し直すと間延びしなくなる。詰め具合は-990とか-1020の数値をいじって調整する。

<!-- dummy comment line for breaking list -->

	

~
~
- - - -
## コンソールフォントの追加

- AddConsoleTrueTypeFont.reg

		Windows Registry Editor Version 5.00
		#
		[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Console\TrueTypeFont]
		"932."="*MigMix 1M"
		"932.."="*Ricty"
- さらに増やしたいなら"932..." のようなレジストリキーにフォント名の文字列を設定する。
- 先頭の * は、なくてもよいらしい。フォント名はコントロールパネルのフォントから得ることができる。
- MigMixやRictyなどのTTFフォントは別途入手してください。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## DOS窓やTeratermなどをいきなり使うときに全角漢字モードにならないようにしたい。


- カテゴリ選択の場合は[コントロールパネル]→[時計、言語、および地域]→[入力方法の変更]を開く。
- アイコン選択の場合は[コントロールパネル]→[言語]を開く。
- [言語の設定の変更]→[詳細設定]を選択。
- [詳細設定]→[入力方式の切り替え]に、「アプリ ウィンドウごとに異なる入力方式を設定する」のチェックボックスをON。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## Cygwinがエラーを吐く

	D:\>svn checkout svn+ssh://server/repo/svn
	      0 [main] svn 3096 child_info_fork::abort: e:\cygwin\bin\cygcrypto-1.0.0.dll: Loaded to differe
	nt address: parent(0x570000) != child(0x540000)
	svn: E000011: Unable to connect to a repository at URL 'svn+ssh://server/repo/svn'
	
	svn: E000011: Can't create tunnel: Resource temporarily unavailable
	
	D:\>

- 解決策求む・・・・

<!-- dummy comment line for breaking list -->

- たぶんこれはDLLのアドレスを不定にしてバッファオーバーラン攻撃をかわすための小細工だと思うんだ。
- 簡単に言うと、Cygwinは諦めて、[slik-svn](http://www.sliksvn.com/)  を入れた。

<!-- dummy comment line for breaking list -->



~
~
- - - -
## DOS窓でシステムファイルなどを書き換えしたい。

- DOS窓を右クリック->管理者として実行。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## 今まで使ってたアプリが動かない。

インストールはうまくいっても、起動させると、何も言わずに起動したふりをして起動しない。


- アプリのアイコンを右クリック
- [互換性のトラブルシューティング]->[問題のトラブルシューティング]->[該当する問題が一覧にない]

<!-- dummy comment line for breaking list -->

- [このプログラムが動いていた以前のバージョン]でWin7とかWinXPを選択。

<!-- dummy comment line for breaking list -->


~
~
- - - -
## ウィンドウを画面の上に合わせると最大化するの、やめてほしい。
- コントロールパネル→（カテゴリー別なら「システムとセキュリティ」）→「コンピューターの簡単操作」→「マウス動作の変更」
- ［マウスを使いやすくします］→ 「ウィンドウの管理を簡単にします」→「ウィンドウが画面の端に移動されたときに自動的に整列されないようにします」をチェック。

<!-- dummy comment line for breaking list -->



~
~
- - - -
## 未解決の問題：解決策求む。

- DOS窓のTrueTypeフォントが汚い（WindowsXPではこうはならない。）
- (DOS窓に限った話でもなさそうだ。アプリによっては、TrueTypeフォントが汚く表示されてしまう。
- ウィンドウ枠の**のっぺり感**は、なんとかならんのか？
- ウィンドウ枠が時々太くなる。
- ハイコントラストのテーマを使わずに、ウィンドウのパーツや背景の色をカスタマイズできないか？

<!-- dummy comment line for breaking list -->
~
~
~

~

