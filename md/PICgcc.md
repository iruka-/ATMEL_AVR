[2009-08](2009-08.md) 

## PICとAVR-gccの１対１対応化による可能性

- AVR-gccの吐き出すコードは基本r16〜r31までしか使わないので、PICのレジスタモデルをこうする。

<!-- dummy comment line for breaking list -->

	+-------------+
	|     r16     |
	+-------------+
	|     r17     |
	+-------------+
	|     r18     |
	+-------------+
	|     r19     |
	+-------------+---
	|     r20     |
	+-------------+
	|     r21     |
	+-------------+
	|     r22     |
	+-------------+
	|     r23     |
	+-------------+---
	|     r24     |
	+-------------+
	|     r25     |
	+-------------+
	|   FSR0L     | X
	+-------------+
	|   FSR0H     |
	+-------------+---
	|   FSR2L     | Y
	+-------------+
	|   FSR2H     |
	+-------------+

	+-------------+
	|   FSR1L     | SP
	+-------------+
	|   FSR1H     |
	+-------------+---

- WREGはr0ということにしよう。
- 常時０を入れておくレジスタはいらないので無視。
- r30,r31は取り除く。
- stackはFSR1を割り当てる。

<!-- dummy comment line for breaking list -->

- r16〜r25は固定番地(access-bank)に置く。(とりあえず0x0000番地から0x0009番地）

<!-- dummy comment line for breaking list -->

- 基本的にどちらも8bitMCUなので、１対１のコード変換が可能なはずだ。

<!-- dummy comment line for breaking list -->


	.global	memcpy8
	.type	memcpy8, @function
	memcpy8:
	mov r27,r25
	mov r26,r24
	mov r31,r23
	mov r30,r22
	.L7:
	subi r20,lo8(-(-1))
	cpi r20,lo8(-1)
	breq .L6
	ld r24,Z+
	st X+,r24
	rjmp .L7
	.L6:

↓
	memcpy8:
	movff r25,FSR0H
	movff r24,FSR0L
	movff r23,FSR2H
	movff r22,FSR2L
	.L7:
	        movlw lo8(-(-1))
	subwf r20,W      ; r20--
	        movwf r20
	
	        movfw r20,W
	        movlw lo8(-1)    ; r20==(-1)?
	subwf r20,W
	bz   .L6
	movff POSTINC0,r24
	movff r24,POSTINC2
	goto .L7
	.L6:

やっぱりすげーだめなコードだ。
~
~
~
- - - -
## PIC24はどうなっている？

- PIC24Fは16bitレジスタが16本ある。（って、どっかで聞いたなぁ MSP430だっけ？）
- PIC16の時代から、このように作っておいてくれれば良かったんだよ。

<!-- dummy comment line for breaking list -->



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic24arc.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic24arc.png) 

- C30はgccベースらしい。
- PIC24のgccのmdがPIC18Fに全く流用できないだろうということだけは分かったつもり。

