[PIC32MX](PIC32MX.md) 

- - - -
## CQ
『はじめて読む MIPS(リローデッド)』 by 中森章 - CQ出版社 
- www.cqpub.co.jp/interface/TechI/Vol39/app/mips_asm.pdf

<!-- dummy comment line for breaking list -->

- - - -
## アーキテクチャー
- MIPS M4KとMIPS16eをサポート。
- MX1xx/2xxには、microMIPSのサポートは無い。
- MX1xx/2xxには、（割り込み応答高速化の）シャドウレジスタのサポートはない。(GP((このページの下に記述)) )のコピーのみ存在)
- 32bitレジスタはr0〜r31の32本
- r0は常に零。書き込んでも値は零のまま。
- r31(ra)はリターンアドレス（サブルーチン呼び出しの戻り番地を記憶）
- そのほかには乗算結果を置くHI,LOレジスタとPCがある。

<!-- dummy comment line for breaking list -->

~
- mips16eモードではレジスタは8本しか使えない(s0,s1,v0,v1,a0,a1,a2,a3)。
- mips16eモードではレジスタt8(分岐条件),sp(スタック),ra(戻り番地)が暗黙的に使用される。

<!-- dummy comment line for breaking list -->

~


- - - -
## MICROCHIP PIC32ファミリ　リファレンスマニュアル
,CPU（必読）,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61113D_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61113D_JP.pdf) 
,割り込み,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61108G_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61108G_JP.pdf) 
,input capture,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532813.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532813.pdf) 
,Config,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532843.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532843.pdf) 
,output compare,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532816.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532816.pdf) 
,comparator,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532822.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532822.pdf) 
,メモリー構成,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61115F_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61115F_JP.pdf) 
,pgm,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532846.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532846.pdf) 
,pmp,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61128G_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61128G_JP.pdf) 
,errata,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp556114.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp556114.pdf) 
,usb,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp534259.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp534259.pdf) 
,I2C,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532834.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532834.pdf) 
,RST,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61118F_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61118F_JP.pdf) 
,RTCC,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532837.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532837.pdf) 
,SPI,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532831.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532831.pdf) 
,UART,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532828.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532828.pdf) 
,CVR,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532825.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532825.pdf) 
,CTMU,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp556027.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp556027.pdf) 
,オシレーター,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61112G_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61112G_JP.pdf) 
,ウォッチドッグとパワーアップタイマ,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61114F_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61114F_JP.pdf) 
,PWR,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61130G_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61130G_JP.pdf) 
,PIO,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61120E_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61120E_JP.pdf) 
,TMR,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61105F_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61105F_JP.pdf) 
,フラッシュプログラミング,[http://ww1.microchip.com/downloads/jp/DeviceDoc/61121E_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61121E_JP.pdf) 
,PIC32MX,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp556030.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp556030.pdf) 
,DMAC,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532840.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532840.pdf) 
,ADC,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532819.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp532819.pdf) 
,ADK,[http://ww1.microchip.com/downloads/jp/DeviceDoc/jp554073.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp554073.pdf) 

- - - -
## MIPS カルトFAQ

### r31はスタックポインタと違うの？
- mips16e命令では、r29(sp)をリターンスタックに使用します。
- MIPS32命令では、**何と**、スタックポインタという機能を持った特殊レジスタはありません。
- ですが、コンパイラはr29(sp)をスタックポインタのように使用して、レジスタ退避などを行います。((push,pop命令のようなものはありません。通常のポインタ操作で行います。オートインクリメント、デクリメントもありません。))
- 質問のr31(ra)はリターンアドレス（サブルーチン呼び出しの戻り番地を記憶）というレジスタで、

		jal &lt;番地&gt;  （サブルーチンコール命令のようなもの）
- を実行したときにjalの次の番地のPC値がraにコピーされます。
- なので、サブルーチンがネストする場合は必ず、サブルーチン側でraを保存し、復帰しなければいけません。（スタックにpush,popします）
- Leaf関数（内部でサブルーチンを呼んでない関数）に限り、raの保存、復帰が不要になるため、高速になります。
- 速度ネックになるのは、一番外側の、たくさん呼ばれる関数ですから、それがLeafになる確率は高いです。

<!-- dummy comment line for breaking list -->

~

### カーネルテンポラリ：k0(r26),k1(r27)レジスタの役割は？

- 割り込みおよび例外処理時に（のみ）使用されるレジスタです。
- 割り込み処理時以外では使ってはいけません。（割り込むたびに破壊され、いつ壊れるかわからないからです）
- コンパイラは通常$k0,$k1を使用しないことが保証されています。

<!-- dummy comment line for breaking list -->

- 割り込み発生時、$k0,$k1レジスタは破壊されますが、多重破壊を防ぐため、$k0,$k1操作はdi状態で実行されます。

<!-- dummy comment line for breaking list -->


~

### GPって、何？
- 28番レジスタに付けられた名前です。

		r28=gp　（グローバルポインタ）
- 共通データへの高速アクセスに使用、とありますが、何のことだか分かりませんが、
- その名の通り、グローバル変数群（64kB以内の一箇所に置く）を常に指しておくポインタです。
- これを設定したり使用するためには、mips-gccのコンパイルオプション**-G もしくは --gpsize**を指定してコンパイルする必要があります。
- グローバル変数が一箇所に置かれ、そのど真ん中あたり（というか先頭+32kB位置)にgpが設定されます。
- グローバル変数はすべて

		offset(gp)
- でアクセスされるので、MIPS固有の２命令による絶対番地ロードが１命令で済むようになってめでたしめでたし（本当？）
- グローバル変数が全部で64kBを越えるような場合はリンカエラーが出ます。
    - その場合は、グローバル変数として扱う変数のサイズを-Gオプションで小さく(たとえば4byte以下)します。
    - それでも64kBを越えるような（グローバル変数の多い）プログラムは書くべきではありませんが、越える場合は-Gオプションの使用を諦めてください。

<!-- dummy comment line for breaking list -->


~
- 実際のところ、コードサイズは少しだけ減りますが、まあ誤差の範囲です。
- 割り込んだときに、gp参照コードはバグるので注意が必要です。（割り込み時にCPUの機能としてgpが別物にすりかわったりしますし、マルチタスク環境では各elf毎にgpの値は異なります）割り込みハンドラーは-Gオプション無しでビルドするほうがより安全です。

