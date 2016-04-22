/* Copyright (c) 2002, Marek Michalkiewicz <marekm@amelek.gda.pl>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE. */

/* $Id: crt.S,v 1.2 2007/08/10 03:17:48 iruka Exp $ */

#if (__GNUC__ < 3) || (__GNUC__ == 3 && __GNUC_MINOR__ < 3)
#error "GCC version >= 3.3 required"
#endif

// *******************************************************
// 		attiny2313専用	int0のみ使用.
// *******************************************************
#if defined (__AVR_ATmega168__)
#define	VECSIZE		12	// RESET, INT0, INT1 の３個のみを有効
#else
// ATmega88, ATtiny2313
#define	VECSIZE		6	// RESET, INT0, INT1 の３個のみを有効
#endif

// *******************************************************
// 		.dataセクションにstaticデータをコピーする.
// *******************************************************
#define	DO_COPY_DATA	1	// .dataサイズが0のときは0に.

// *******************************************************
// 		.bss セクションをゼロクリアする.
// *******************************************************
#define	DO_CLEAR_BSS	1	// .bssサイズが 0のときは0に.


#include "macros.inc"

	.macro	vector name
	.if (. - __vectors < VECSIZE)
	.weak	\name
	.set	\name, __bad_interrupt
	XJMP	\name
	.endif
	.endm

	.section .vectors,"ax",@progbits
	.global	__vectors
	.func	__vectors
__vectors:
	XJMP	__init		; リセット電源ON, WDT, BOD等の各種リセット
#if defined (__AVR_ATtiny2313__)
	vector	__vector_1	; EXT_INT0	 ;外部割り込み要求0
	vector	__vector_2	; EXT_INT1	 ;外部割り込み要求1
	vector	__vector_3	; TIM1_CAPT	 ;タイマ/カウンタ1 捕獲(キャプチャ)発生
	vector	__vector_4	; TIM1_COMPA	 ;タイマ/カウンタ1 比較A一致
	vector	__vector_5	; TIM1_OVF	 ;タイマ/カウンタ1 オーバーフロー
	vector	__vector_6	; TIM0_OVF	 ;タイマ/カウンタ0 オーバーフロー
	vector	__vector_7	; USART_RXC	 ;USART 受信完了
	vector	__vector_8	; USART_UDRE	 ;USART 送信バッファ空き
	vector	__vector_9	; USART_TX	 ;USART 送信完了
	vector	__vector_10	; ANA_COMP	 ;アナログ比較器出力遷移
	vector	__vector_11	; PCINT	 ;ピン変化割り込み要求
	vector	__vector_12	; TIM1_COMPB	 ;タイマ/カウンタ1 比較B一致
	vector	__vector_13	; TIM0_COMPA	 ;タイマ/カウンタ0 比較A一致
	vector	__vector_14	; TIM0_COMPB	 ;タイマ/カウンタ0 比較B一致
	vector	__vector_15	; USI_STRT	 ;USI 開始条件検出
	vector	__vector_16	; USI_OVF	 ;USI カウンタ オーバーフロー
	vector	__vector_17	; EE_RDY	 ;EEPROM操作可
	vector	__vector_18	; WDT_OVF	 ;ウォッチドッグ計時完了
#else
	vector	__vector_1	; INT0 外部割り込み要求0
	vector	__vector_2	; INT1 外部割り込み要求1
	vector	__vector_3	; PCINT0 (PCI0) ピン変化割り込み要求0
	vector	__vector_4	; PCINT1 (PCI1) ピン変化割り込み要求1
	vector	__vector_5	; PCINT2 (PCI2) ピン変化割り込み要求2
	vector	__vector_6	; ウォッチドッグ WDT ウォッチドッグ計時完了
	vector	__vector_7	; タイマ/カウンタ2 COMPA タイマ/カウンタ2比較A一致
	vector	__vector_8	; タイマ/カウンタ2 COMPB タイマ/カウンタ2比較B一致
	vector	__vector_9	; タイマ/カウンタ2 OVF タイマ/カウンタ2溢れ
	vector	__vector_10	; タイマ/カウンタ1 CAPT タイマ/カウンタ1捕獲発生
	vector	__vector_11	; タイマ/カウンタ1 COMPA タイマ/カウンタ1比較A一致
	vector	__vector_12	; タイマ/カウンタ1 COMPB タイマ/カウンタ1比較B一致
	vector	__vector_13	; タイマ/カウンタ1 OVF タイマ/カウンタ1溢れ
	vector	__vector_14	; タイマ/カウンタ0 COMPA タイマ/カウンタ0比較A一致
	vector	__vector_15	; タイマ/カウンタ0 COMPB タイマ/カウンタ0比較B一致
	vector	__vector_16	; タイマ/カウンタ0 OVF タイマ/カウンタ0溢れ
	vector	__vector_17	; SPI STC SPI 転送完了
	vector	__vector_18	; USART RX USART 受信完了
	vector	__vector_19	; USART UDRE USART 送信バッファ空き
	vector	__vector_20	; USART TX USART 送信完了
	vector	__vector_21	; A/D変換器 ADC A/D変換完了
	vector	__vector_22	; EEPROM EE_RDY EEPROM 操作可
	vector	__vector_23	; アナログ比較器 ANA_COMP アナログ比較器出力遷移
	vector	__vector_24	; 2線直列インターフェース TWI 2線直列インターフェース状態変化
	vector	__vector_25	; SPM命令 SPM_RDY SPM命令操作可
#endif
	.endfunc

	/* Handle unexpected interrupts (enabled and no handler), which
	   usually indicate a bug.  Jump to the __vector_default function
	   if defined by the user, otherwise jump to the reset address.

	   This must be in a different section, otherwise the assembler
	   will resolve "rjmp" offsets and there will be no relocs.  */

	.section .init0,"ax",@progbits
	.weak	__init
;	.func	__init
__init:
__bad_interrupt:

	.weak	exit
exit:

#ifndef __AVR_ASM_ONLY__
	.weak	__stack

	/* By default, malloc() uses the current value of the stack pointer
	   minus __malloc_margin as the highest available address.

	   In some applications with external SRAM, the stack can be below
	   the data section (in the internal SRAM - faster), and __heap_end
	   should be set to the highest address available for malloc().  */
	.weak	__heap_end
	.set	__heap_end, 0

	.section .init2,"ax",@progbits
	clr	__zero_reg__
	out	_SFR_IO_ADDR(SREG), __zero_reg__
	ldi	r28,lo8(__stack)
	ldi	r29,hi8(__stack)

	out	_SFR_IO_ADDR(SPL), r28
	out	_SFR_IO_ADDR(SPH), r29

	.section .init4,"ax",@progbits
	.global __do_copy_data
	.global __do_clear_bss

// *******************************************************
// attiny2313専用:
// .data , .bss の上位アドレスは常に０であると仮定.
// *******************************************************

__do_copy_data:
	ldi	r17, hi8(__data_end)

	// Y r26:27 = 初期化データのコピー先SRAMアドレス.
	ldi	r26, lo8(__data_start)
	ldi	r27, hi8(__data_start)

	// Z r30:31 = staticデータのコピー元ROMアドレス.
	ldi	r30, lo8(__data_load_start)
	ldi	r31, hi8(__data_load_start)


// *******************************************************
// 		.dataセクションにstaticデータをコピーする.
// *******************************************************
#if	DO_COPY_DATA
// *******************************************************
// .data  のサイズは非ゼロを仮定.
//	rjmp	.do_copy_data_start
// *******************************************************

.do_copy_data_loop:
	lpm r0, Z+
	st	X+, r0

.do_copy_data_start:
	cpi	r26, lo8(__data_end)
	cpc	r27, r17

// *******************************************************
// attiny2313専用:
// .data , .bss の上位アドレスは常に０であると仮定.
//	cpc	r27, r17
// *******************************************************

	brne	.do_copy_data_loop

#endif	// DO_COPY_DATA

// *******************************************************
// 		.bss セクションをゼロクリアする.
// *******************************************************
#if	DO_CLEAR_BSS

__do_clear_bss:

// *******************************************************
// .data の直後に.bssが来ると仮定.
// .bss  のサイズは非ゼロを仮定.
// *******************************************************
	ldi	r17, hi8(__bss_end)
//	ldi	r26, lo8(__bss_start)
//	ldi	r27, hi8(__bss_start)
//	rjmp	.do_clear_bss_start


.do_clear_bss_loop:
	st	X+, r1
.do_clear_bss_start:
	cpi	r26, lo8(__bss_end)
	cpc	r27, r17
	brne	.do_clear_bss_loop

#endif	//	DO_CLEAR_BSS

	.set	__stack, RAMEND

#endif /* !__AVR_ASM_ONLY__ */

	.section .init9,"ax",@progbits
	XJMP	main

;	.endfunc

