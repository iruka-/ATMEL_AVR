[AVR_term](AVR_term.md) 

#### Ｗ３２＿ｔｅｒｍ

- これは、Windowsで動作する簡易シリアルターミナルソフトです。

<!-- dummy comment line for breaking list -->

特徴
- Ｃソースコードが付属します。mingw-gcc(www.mingw.org)でコンパイルします。
- ＵＳＢで接続されたAVR_termのファームウェアと交信することによって、あたかも仮想ＣＯＭポートの先にＡＶＲ側のＵＳＡＲＴが繋がっているように見せることが出来ます。((別途ComEmulDrvも必要です [http://mixw.net/related.htm#tncemu))](http://mixw.net/related.htm#tncemu%29%29) 
- 単純なエコーバックサーバーとして動作させることも出来ます。

<!-- dummy comment line for breaking list -->

使い方
- 無手順端末として起動させます。

		C:&gt; w32term [Option]
		#
		Option
		  -b&lt;baudRate&gt;
		  -c&lt;COMM Port&gt;
		  -l&lt;Log File&gt;
- エコーバックサーバーとして起動します。

		C:&gt; w32term -e [Option]
- ＡＶＲと接続する仮想ＣＯＭサーバーとして起動します。

		C:&gt; w32term -a -c9
    - この場合は、ComEmulDrvが必要です。
    - 上の例では、このサーバーが使用するCOMポートは COM9を指定しています。（デフォルトはCOM9）
    - ＡＶＲのＵＳＡＲＴに受信された文字はCOM9に送り、COM9から受信した文字はＵＳＡＲＴから送信します。(下図)
    
    		COM9 RxD ----&gt; TxD AVR USART
    		COM9 TxD &lt;---- RxD AVR USART
    		           ↑
    		        w32termが介在
    - 一例として、ComEmulDrvがCOM5とCOM9のペアを仮想クロスケーブルで接続しているものとします。
    
    		COM5 TxD ----&gt; RxD COM9
    		COM5 RxD &lt;---- TxD COM9
    		           ↑
    		     仮想クロスケーブル
- PuTTYなどでCOM5を見ると、それはあたかもAVRチップのUSARTが繋がっているように見えます。

		COM5 TxD ----&gt; TxD AVR USART
		COM5 RxD &lt;---- RxD AVR USART
		           ↑
		COM5がＵＳＢシリアルポートに化ける。
- ちなみにCOM5、COM9ともにComEmulDrvが作り出した仮想ＣＯＭポート。
- ＡＶＲのボーレート設定はCOM5側からCOM9に渡すなどの芸当は出来ない。
    - w32term -a を立ち上げるときに -b<baudRate>オプションを与えることでＡＶＲの初期設定を行う。

<!-- dummy comment line for breaking list -->


- - - -
#### ダウンロード
かなり制作途上です。
- [upload/w32term.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/w32term.zip) 

<!-- dummy comment line for breaking list -->


- - - -
#### 何に使うの？
- 相手先がＣＯＭ：でないと困るようなシリアル接続。（典型例がh8write.c）
- 現状TxD RxD以外の信号線の制御は出来ない。
    - 出来たとしてもその反応速度は伝送ボーレートの１/４〜１/７くらい（というかＬｏｗＳｐｅｅｄコントロール転送の１サイクル分＝４ｍＳに１回程度）なので、あまり利用価値はない。
    - というかそれをやるならＡＶＲ側のＩＯポートも割り当てなきゃ。

