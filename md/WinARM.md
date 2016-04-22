[ARMCortexM3](ARMCortexM3.md) 

## WinARMのダウンロード

以下からＤＬします。
- [WinARM](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm) 

<!-- dummy comment line for breaking list -->

## Installメモ
上記[WinARM](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm) から、WinARM_20080331_testing.zip をダウンロード。
	C:\> unzip WinARM_20080331_testing.zip
	C:\> path %path%;C:\WinARM\bin;C:\WinARM\utils\bin
- C:\に展開すると、C:\WinARM\ 以下にコンパイラがインストールされます。
- 実行パスを C:\WinARM\bin と C:\WinARM\utils\bin の両方に通します。
- ここまではWinAVRとほとんど同じ使い勝手です。

<!-- dummy comment line for breaking list -->

- コンパイラはarm-elf-gcc ではなく arm-eabi-gcc になります。

<!-- dummy comment line for breaking list -->

参考：ARM gcc バッドノウハウ集
- [http://jr0bak.homelinux.net/~imai/linux/arm_gcc_badknowhow/arm_gcc_badknowhow.html#toc8](http://jr0bak.homelinux.net/~imai/linux/arm_gcc_badknowhow/arm_gcc_badknowhow.html#toc8) 

<!-- dummy comment line for breaking list -->

~
~
~

- - - -
### WinARM(arm-eabi-gcc)でfloatが使えない件

- libgcc.aに

		__aeabi_dmul
- とかもろもろのsoftfloat関数が抜けているようです。
- [ARMクロスコンパイラ構築Linux](2010-04#b817f226.md) で作ったlibgcc.aをコピーすればリンクは通ります。

<!-- dummy comment line for breaking list -->


~
- WinARMは、あえてlibgcc.aを差し替えないで、floatを使ったら地雷警告が出ると思って使うのも悪くないと思いますが、
- やせ我慢せず素直にfloatを使ったほうが楽な場合は、以下のビルド済みlibgcc.aをご利用ください。

<!-- dummy comment line for breaking list -->

~

**libgcc.a ダウンロード:**
- [libgcc.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/libgcc.zip) 
- Linuxでビルドしたものは若干ディレクトリ構成が異なりますが、
- WinARM以下のlibgcc.aと差し替えると、__aeabi_dmulなどが使えるようになります。

		C:/WinARM/lib/gcc/arm-eabi/4.3.0/libgcc.a
		C:/WinARM/lib/gcc/arm-eabi/4.3.0/thumb/libgcc.a
		C:/WinARM/lib/gcc/arm-eabi/4.3.0/thumb2/libgcc.a

