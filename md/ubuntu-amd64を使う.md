[2009-11](2009-11.md) 

## 使ってみた感想。

- とりあえず使ってみる。

		main() {printf("Hello");}
		$ gcc -O2 main.c
		$ objdump -S a.out
- するとこんな感じ。

		0000000000400460 &lt;_start&gt;:
		 400460:       31 ed                   xor    %ebp,%ebp
		 400462:       49 89 d1                mov    %rdx,%r9
		 400465:       5e                      pop    %rsi
		 400466:       48 89 e2                mov    %rsp,%rdx
		 400469:       48 83 e4 f0             and    $0xfffffffffffffff0,%rsp
		 40046d:       50                      push   %rax
		 40046e:       54                      push   %rsp
		 40046f:       49 c7 c0 70 05 40 00    mov    $0x400570,%r8
		 400476:       48 c7 c1 80 05 40 00    mov    $0x400580,%rcx
		 40047d:       48 c7 c7 50 05 40 00    mov    $0x400550,%rdi
		 400484:       e8 c7 ff ff ff          callq  400450  &lt;__libc_start_main@plt&gt;
		 400489:       f4                      hlt
		 40048a:       90                      nop
		 40048b:       90                      nop
- 64bit長げー。（それが感想か？）

<!-- dummy comment line for breaking list -->

嘘。

## ubuntu-amd64のよい点。
- 大抵の実行ファイルを実行できる。
    - Win32Console EXE / Win32 GUI EXE / 32bit i386 elf / x86_64 elf
- ~~64bit版だから~~レジスタが実質６本(eax,edx,ecx,ebx,esi,edi)から、１４本に増えたから、elfが速いかもしれない。（未計測）
- ConsoleがWin32よりまとも。

<!-- dummy comment line for breaking list -->

## ubuntuのだめな点。

- ~~mp3とかmpegを再生できない。（何それ）~~
    - 適切なコーデックが無かったり、自動で入れてくれたやつがだめだったり。

<!-- dummy comment line for breaking list -->

    - ---> vlc player を入れると一応再生出来るようになります。
    
    		# apt-get install vlc
- 字（フォント）が微妙に汚い。

<!-- dummy comment line for breaking list -->



    - まあたぶん、使い慣れたWindowsと比べて、各種のレンダリングエンジンやらフォントが異なるので、配置がずれて気持ちが悪いとか、OpenOfficeでも以下同様。
    - **慣れの問題**

<!-- dummy comment line for breaking list -->

- 漢字変換がダメダメ。

<!-- dummy comment line for breaking list -->


- DVDが焼けない。（焼くためのフリーソフトはあるんだけど、デバイスがエラーして１枚焼き損じた。）

<!-- dummy comment line for breaking list -->

しかし、昔に比べるとだいぶ良くなっている。
- マルチメディア系に目をつぶれば、何とか使える。
- とりあえず困らない。
    - MPLABのC18のインストーラーが普通に動いて、mcc18.exeのインストールが完了する。
    - [mchip-cdc.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mchip-cdc.zip) をそのままビルド出来る。

<!-- dummy comment line for breaking list -->

    - 全くもって問題なしなので、Windows要らない。

<!-- dummy comment line for breaking list -->

## 64bit Linuxは(32bit版に比べて)速いのか？
- ~~否。~~
    - ---> 64bitにすると微妙に速い。でも微妙（数％程度）
    - 整数レジスタが14本になり、関数引数がほぼレジスタ渡しになるので少しだけ速くなる（？）
    - しかし、ポインタサイズが無駄に長い(64bitになるが実際に使用するアドレスはせいぜい40bit程度)ので命令長やデータアクセスのバンド幅を無駄に消費する。
    - Atomでは64bitモードで逆に遅くなるという話。64bitレジスタはあってもALUが32bitだとか（？）ありがち。未検証。
    - AMD64とintel x86_64は（一応）互換性はあるのだが、最適化方法が異なるのでx86_64のバイナリーをAthlon64で動作させると速度的に不利らしい。（未検証）
- intel様は、Core2アーキテクチャーの設計時に32bitモードでの最適化のほうを選択した。
- 命令デコーダの並列度は32bitモードで最適になるように設計されたが、64bitモードでは命令長が伸びるため、１クロックでデコード可能な命令数が(32bitモードと比べて)少なくなる。
- Core2を高速にしている特長の一つ、microOP fusion は32bitモードでしか有効になっていない。
- レジスタ数が8->16に増えた反面、命令プリフィックス(REX)が必要となってコードサイズや命令デコードの手間が増える。
- アドレスフィールドが64bitに増えるのでプログラムサイズが肥大化し、そのぶん命令キャッシュ、データキャッシュも効きが悪くなる。

<!-- dummy comment line for breaking list -->

但し、64bit整数演算を多用する場合は断然64bit-elfのほうが速くなるのでそこは工夫次第。

~
~
~
~
- - - -
## 番外編：g++(gcc)のコンパイル速度を比較する。

|Celeron E3200|3.20GHz(OC)|64bit Linux|real	0m53.769s|user	0m49.480s|sys	0m5.010s|
|Core2 E6600|2.40GHz|64bit Linux|real    0m56.939s|user    0m52.870s|sys     0m5.190s|
|Core2 E6600|2.40GHz|32bit Linux|real	1m1.912s|user	0m58.388s|sys	0m3.660s|
|Celeron E3200|2.40GHz|64bit Linux|real	1m7.561s|user	1m2.770s|sys	0m5.590s|
|Athlon64 X2 4200+| 2.20GHz |64bit Linux|real	1m26.263s|user	1m19.380s|sys	0m8.220s|
|Athlon64 X2 4200+| 2.20GHz |32bit Linux|real	1m25.029s|user	1m19.977s|sys	0m6.004s|

- 面倒なのでグラフ化はしていません。
- Linuxは全部ubuntu9.10です。
- g++-4.4.1です。
- ビルドに使用したソースは[ＨＴエディタ](http://sourceforge.jp/projects/sfnet_hte/) です。（Ｃ＋＋で記述され、丁度１分程度でビルド完了します）
- 計測方法は、

		$ make clean
		$ time make
- これを２回実施し、２回目の値を貼り付けています。（ディスクアクセスを極力キャッシュさせて影響を減らすため）
- ~
- Celeron DualCore(E3200)は4500円程度で売られています。同クロックのCore2Duo比で２割弱遅いです。
- Athlon64ではどうかというと、同クロックのCore2Duo比（換算して比較）で４割程遅いです。

<!-- dummy comment line for breaking list -->

- E3200のOC性能が奮わないのは、メモリーが追いつかないのでメモリークロックを落としているためです。
- また、E6600はDDR2をデュアルチャネルで動作させていますが、E3200のほうはシングルチャネルしか挿していません。

