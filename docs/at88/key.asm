;======================================================================
;
;		�`�s�W�W�O�P����II-�r�q
;
;======================================================================
;
;	�EPC8801mkIISR�̃L�[�{�[�h���`�s�݊��@�ɐڑ�����.
;	�E�p�ӂ���p�[�c�́APIC 16F84 �P�� 74LS158���.
;	�E��R 2.2k���~4��,10k���~2�A2SC�^�C�v�̏��^Tr�Q��.
;
;�P�U�e�W�S�̔z��:
;----------------------------------------------------------------------
;	PORTA	0 - 3   OUT     74159N  4-16 SELECT
;	PORTA	4	OUT     74LS158 A-B  SELECT
;
;	PORTB	0 - 3	IN      74LS158 0-3 OUT
;	PORTB	4	in	KBD data
;	PORTB	5	out	KBD data
;	PORTB	6	in	KBD clock
;	PORTB	7	out	KBD clock
;
;	Clock in��10k���Ńv���A�b�v.(��7MHz���M)
;	����7�`8MHz�̐����𐄏�.
;	���g�����ς�����ꍇ�́A�E�F�C�g���[�v�̒l��v����.
;
;	���Z�b�g�̓v���A�b�v���邱��.
;
;PORTB 4 - 7 �̐��m�Ȕz���͈ȉ����Q��.
;----------------------------------------------------------------------
;
;			     vcc	vcc
;			      | 	 |
;			      \ 	-+-
;			      / 2K2	/_\  1N4148
;			      \ 	 |
; hoCLKi    ------------------o---o------o---------   kbd CLOCK
;			      |   |	 |
;			 2N2222   |50pF -+-
; hoCLKo    ______/\/\/\____|/	 ===	/_\
;		    2K2     |\>   |	 |
;			      |   |	 |
;			     /// ///	///
;
; kbCLK��kbDATA�́A�����H��OK. �_�C�I�[�h�ƃR���f���T�͏ȗ���.
; 2N2222 ��2SC1845�̂悤��Tr��OK.
;
;��v���[�`���ƃ��[�N.
;----------------------------------------------------------------------
;	KeyScanMain()
;		KeyPos		0..88      key number
;		KeyRow		0..11 ---> 74159N select signal
;		KeyCol		0..7  .... bitpos
;		KeyMask		0x01 .. 0x80 ... bitmask
;		KeyVal		KeyRow�ɑΉ�����r�b�g�p�^��.
;
;	KeyBits[12]	KeyVal�̕���
;
;
;


#include "p16f84.inc"
#include "custom.h"

; KeyBoard �o�� Pin�ɂ�Transister�������Ă���̂ŕ��_���ł�.
PinLO  macro P, B
     bsf  P, B
 endm


PinHI  macro P, B
     bcf  P, B
 endm


;--------------------------------------------------------------
;	�X�^�[�g.
;--------------------------------------------------------------
	jmp	_main

;--------------------------------------------------------------
;	�L�[�X�L�����E���[�N
;--------------------------------------------------------------
 New	KeyPos		;;0..88      key number
 New	KeyRow		;;0..11 ---> 74159N select signal
 New	KeyCol		;;0..7  .... bitpos
 New	KeyMask		;;0x01 .. 0x80 ... bitmask
 New	KeyVal		;;KeyRow�ɑΉ�����r�b�g�p�^��.
 New	KeyWk		;;�ėp���[�N.

 New	KeyRepCnt	;;�L�[���s�[�g�J�E���^.
 New	KeyRepFlag	;;�L�[���s�[�g�t���O.
 New	KeyRepCode	;;�L�[���s�[�g�������L�[.

 New	ScanMode	;;

;--------------------------------------------------------------
;	�V���A�����[�N
;--------------------------------------------------------------
 New	SendWk		;; �V���A���ėp.
 New	SendCnt		;; �W�r�b�g�܂ł̃��[�v�𐔂���.
 New	SendParity	;; �p���e�B�v�Z�p(1bit�ŗǂ�)
 New	SendByte	;; ���M�o�C�g�E�o�b�t�@.
 New	SendByteLast	;; �Ō�ɑ��M�����f�[�^(�đ����Ɏg�p)
 New	RecByte		;; ��M�o�C�g�E�o�b�t�@.

;--------------------------------------------------------------
;	�z�X�g�R�}���h�t���O
;--------------------------------------------------------------
 New	ResScanType	;; 01 02 03
 New	ResRateDelay	;; b7-unused. b6..5-Repeat delay (00=250 mS,11=1000mS)
			;; b4..0-Repeat rate (00000=30x/sec, 11111=2x/sec).


;--------------------------------------------------------------
;	�r�b�g�̒�`
;--------------------------------------------------------------
#define	 KeyREPCLR	KeyRepFlag,7	;; �L�[��Ԃ��ω�����.
#define	 KeyREPRESS	KeyRepFlag,6	;; �L�[�͂P�ȏエ����Ă���.

#define	 BrkOFF		ScanMode,0	;;�u���[�N�R�[�h�͑���Ȃ�.
#define	 RepOFF		ScanMode,1	;;�L�[���s�[�g�R�[�h�͑���Ȃ�.

;--------------------------------------------------------------
;	�|�[�g���蓖�Ă̒�`
;--------------------------------------------------------------
#define  kbCLKi   	PORTB, 6	; clock line input
#define  kbCLKo   	PORTB, 7	; clock line output(inverted OpenCollector)
#define  kbDATAi  	PORTB, 4	; data line input
#define  kbDATAo  	PORTB, 5	; data line output(inverted OpenCollector)


;--------------------------------------------------------------
;	�L�[�X�L�����E���ʃe�[�u��
;--------------------------------------------------------------
;	�W�r�b�g�~�P�Q�q�������X�U�L�[���B������Ă�����P������.

 New	KeyBits
 New	KeyBits1
 New	KeyBits2
 New	KeyBits3
 New	KeyBits4
 New	KeyBits5
 New	KeyBits6
 New	KeyBits7
 New	KeyBits8
 New	KeyBits9
 New	KeyBits10
 New	KeyBits11


;--------------------------------------------------------------
;  �o�b�W�W�̃X�L�����R�[�h���P�O�U�z��̃X�L�����R�[�h�ɕϊ�
;--------------------------------------------------------------
#include "tbl.inc"

;--------------------------------------------------------------
;  �o�b�W�W�̃L�[�Ń��s�[�g�������Ȃ��r�b�g�Ɉ��t����.
;--------------------------------------------------------------
KeyRepMask:
	addwf	PC,F
	retlw	B'00000000'	; 0x00
	retlw	B'00000000'
	retlw	B'00000000'	; 0x10
	retlw	B'00000000'
	retlw	B'00000000'	; 0x20
	retlw	B'00000000'
	retlw	B'00000000'	; 0x30
	retlw	B'00000000'
	retlw	B'11110000'	; 0x40
	retlw	B'00000000'
	retlw	B'10000000'	; 0x50
	retlw	B'00000000'

;--------------------------------------------------------------
;	�J�����ԍ�����A����PORT�̃}�X�N�p�^�[���ւ̕ϊ�.
;--------------------------------------------------------------
Col2Mask:
	andlw	0x07
	addwf	PC,F
	retlw	B'00000001'
	retlw	B'00000010'
	retlw	B'00000100'
	retlw	B'00001000'
	retlw	B'00010000'
	retlw	B'00100000'
	retlw	B'01000000'
	retlw	B'10000000'

;--------------------------------------------------------------
;	�z�X�g�R�}���h ED�`FF�ւ̉������[�`���֕���.
;--------------------------------------------------------------
respond_W:
	addwf	PC,F
	jmp	respond_ED
	jmp	respond_EE
	jmp	respond_EF
	jmp	respond_F0
	jmp	respond_F1
	jmp	respond_F2
	jmp	respond_F3
	jmp	respond_F4
	jmp	respond_F5
	jmp	respond_F6
	jmp	respond_F7
	jmp	respond_F8
	jmp	respond_F9
	jmp	respond_FA
	jmp	respond_FB
	jmp	respond_FC
	jmp	respond_FD
	jmp	respond_FE
	jmp	respond_FF
;--------------------------------------------------------------
;������	�W�����v�e�[�u����(addwf PC,F)�́A�Ȃ�ׂ�0x100�Ԓn
;	�ȉ��ɒu��.�������Ȃ��� PCLATH��ݒ肷��K�v�����o��.
;--------------------------------------------------------------

;--------------------------------------------------------------
;	������0x100�Ԓn���z���Ă�����v����:
;--------------------------------------------------------------
H100fence:

;--------------------------------------------------------------
;	���C�������J�n.
;--------------------------------------------------------------
_main:
	;;�|�[�g�̓��o�͕��������߂�.
	SetRPage 1
	clrw			;;	PORT A�͑S�o��
	movwf	PORTA
	movlw	B'01011111'	;; bit 5 ,7�̂ݏo��.���͓���
	movwf	PORTB

	SetRPage 0

	;;�o�̓s���̒l������������.
	PinHI  kbCLKo
	PinHI  kbDATAo

	;;�V�X�e���S�̂����肷��܂ő҂�.
	call	Wait1000mS

;--------------------------------------------------------------
;	Diagnostic �������݊��@�ɓ`����.
;--------------------------------------------------------------

	movlw	0xaa
	call	KeySendW	;;no problem.

	movlw	0x03
	movwf	ResScanType
	clrf	ScanMode

;--------------------------------------------------------------
;	���C�����[�v
;--------------------------------------------------------------
KeyScanMain:
	call	KeyBitsInit

;;���s�[�g������:
KeyScan_0:
	clrf	KeyRepFlag
	movl	KeyRepCnt,0x60	;;���s�[�g�J�n�J�E���g�_�E���J�E���^.

;;�ʏ�̏�����:
KeyScan_1:
	clrf	KeyPos		;;�L�[�Z���X�|�C���^.
	bcf	KeyREPCLR	;;�L�[�ω��t���O��������.
	bcf	KeyREPRESS	;;�L�[���ǂꂩ������Ă���t���O.


;;�L�[��S���Z���X���郋�[�v�̐擪:
KeyScan_2:
	call	KeyRecv1	;;�Ȃɂ������Ă������M����.
	call	KeyScan1	;;�L�[���P�Z���X����.

	incf	KeyPos,F	;; �|�C���^ �{�P
	movfw	KeyPos
	xorlw	0x5a		;;�ŏI�R�[�h�ɒB����܂�.
	jnz	KeyScan_2
;;�L�[��S���Z���X���郋�[�v�͂����܂�:


;;�����������ƂŁA��������ςȂ��L�[������΁A���s�[�g����������.
	btfsc	KeyREPCLR
	jmp	KeyScan_0	;;�ω����������̂ŁA���s�[�g�L�����Z��.
	btfss	KeyREPRESS
	jmp	KeyScan_0	;;�L�[�͉���������Ă��Ȃ�.
	btfsc	RepOFF
	jmp	KeyScan_0	;;�^�C�p�}�e�B�b�N���M���Ȃ�.

	decfsz	KeyRepCnt,F	;;���s�[�g�J�n�J�E���g�ɒB����܂Ń��[�v.
	jmp	KeyScan_1

;;���s�[�g�̎��s.
	movfw	KeyRepCode
	movwf	KeyPos
	call	KeySend

	movl	KeyRepCnt,0x0c	;;���s�[�g�J�n�J�E���g�_�E���J�E���^�����߂�.
	jmp	KeyScan_1

;--------------------------------------------------------------
;	�L�[�X�L�����E�������[�`��
;--------------------------------------------------------------

;--------------------------------------------------------------
;	��A�h���X���Z�b�g���āA���ʕb(TTL�����������܂�)�҂�.
;--------------------------------------------------------------
KeyRowOut:
	movwf	PORTA	;��A�h���X��PORT A�̉��ʂS�r�b�g(74159N)�ɗ^����.
KeyW:
	movl	SendWk,8
KeyW_1:
	decfsz	SendWk,F	; 1
	jmp	KeyW_1		; 2
	ret
Wait12uS:
	movl	SendWk,6
	jmp	KeyW_1
Wait20uS:
	movl	SendWk,12
	jmp	KeyW_1
Wait400uS:
	movl	SendWk,0
	jmp	KeyW_1
;--------------------------------------------------------------
;	
;--------------------------------------------------------------
Wait1000mS:
	movl	KeyWk,0		;1
_loop1:
	call	Wait400uS
	decfsz	KeyWk,F
	jmp	_loop1
	ret


;--------------------------------------------------------------
;	KeyRow�ɓ�����}�g���N�X�f�[�^���P�o�C�g����Ă���
;--------------------------------------------------------------
KeyGetRow:
	movfw	KeyRow		;; Row�o��.
	call	KeyRowOut

	movfw	PORTB		;; �S�r�b�g��荞��.
	andlw	0x0f
	movwf	KeyVal

	movfw	KeyRow
	iorlw	0x10		;; LS158�؂�ւ�.
	call	KeyRowOut

	movfw	PORTB		;; �S�r�b�g��荞��.
	andlw	0x0f
	movwf	KeyWk
	
	swapf	KeyWk,W		;; W = swap(KeyWk);
	iorwf	KeyVal,F	;; KeyVal |= W		4bit �� 4bit ������

	ret

;--------------------------------------------------------------
;	KeyBits[KeyRow] �e�[�u�����C���f�b�N�X�Q�Ƃ���ׂ̏���.
;--------------------------------------------------------------
KeyBitAdr:
	movlw	KeyBits
	addwf	KeyRow,W
	movwf	FSR		;; �Ԑ�Register = KeyBits+KeyRow
	ret

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g����������(Z �t���O).
;--------------------------------------------------------------
KeyBitChk:
	call	KeyBitAdr
	movfw	KeyMask
	andwf	INDF,W		;; �������[�ԐڎQ��.
	ret

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g�𗧂Ă�.
;--------------------------------------------------------------
KeyBitSet:
	call	KeyBitAdr
	movfw	KeyMask
	iorwf	INDF,F		;; �������[�ԐڎQ��.
	ret

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g��|��.
;--------------------------------------------------------------
KeyBitReset:
	call	KeyBitAdr
	movfw	KeyMask
	xorlw	0xff		;�}�X�N�𔽓]���ā������.
	andwf	INDF,F		;; �������[�ԐڎQ��.
	ret

;--------------------------------------------------------------
;	�L�[�r�b�g�z���S����������.
;--------------------------------------------------------------
KeyBitsInit:
	clrf	KeyRow
	movl	KeyWk,12

KeyBitsInit1:
	call	KeyBitAdr
	clrf	INDF
	incf	KeyRow,F
	decfsz	KeyWk,F
	jmp	KeyBitsInit1
	ret


;--------------------------------------------------------------
;	���݃X�L���������L�[�����s�[�g�s�������� Not Zero �ɂ���
;--------------------------------------------------------------
KeyRepMaskChk:
	movfw	KeyRow
	call	KeyRepMask
	andwf	KeyMask,W
	ret

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g��KeyVal�Ƀt�F�b�`����.
;--------------------------------------------------------------
;	����p�Ƃ��āAKeyRow,KeyCol,KeyMask ���Z�b�g�����.
KeyGetBit:
	;;; KeyRow = KeyPos >> 3;
	ld	KeyRow,KeyPos
	rrf	KeyRow,F
	rrf	KeyRow,F
	rrf	KeyRow,W
	andlw	0x0f
	movwf	KeyRow

	;;; KeyCol = KeyPos & 7
	movfw	KeyPos
	andlw	0x07
	movwf	KeyCol
	
	;;; KeyMask = masktable[KeyCol]
	call	Col2Mask
	movwf	KeyMask

	call	KeyGetRow
	movfw	KeyMask
	andwf	KeyVal,F

	;;; Repeat Key Pressed?
	jnz	KeyGetBit_REPON
	ret

KeyGetBit_REPON:
	call	KeyRepMaskChk
	skpnz
	bsf	KeyREPRESS	;;���s�[�g�\�ȃL�[��������Ă���.
	ret

;--------------------------------------------------------------
;	KeyPos�Ԗڂɓ�����L�[���P��������
;--------------------------------------------------------------
;	�ω����L������A�R�[�h���݊��@�ɑ��M����.
KeyScan1:
	call	KeyGetBit	;;�}�g���N�X��1bit���X�L����.
	call	KeyBitChk	;;���[�N�G���A�Ɣ�r.
	xorwf	KeyVal,W
	btfsc	STATUS,Z
	ret			;;�ω������F��v���Ă�����return

;�r�b�g�̕ω��L��.
	bsf	KeyREPCLR	;;�ω�����������A�L�[�ω��t���O��On
	movfw	KeyVal
	jz	KeyTurnOff

;�L�[�������ꂽ.
KeyTurnOn:
	call	KeyBitSet
	call	KeySend
	call	KeyRepMaskChk
	skpz
	ret
;;���s�[�g�\�ȃL�[�̏ꍇ�A���̃R�[�h�����߂�.
	movfw	KeyPos
	movwf	KeyRepCode
	ret

;�L�[�������ꂽ.
KeyTurnOff:
	call	KeyBitReset
	btfsc	BrkOFF		;;�L�[�u���[�N���ɃR�[�h�𑗂�?
	ret			;;bit�������Ă����瑗��Ȃ�.
	jmp	KeySendF0

;--------------------------------------------------------------
;	�L�[�u���[�N�R�[�h���o
;--------------------------------------------------------------
KeySendF0:
	movfw	KeyPos
	call	pc88toAT
	iorlw	0x00
	btfsc   STATUS, Z
	ret

	movwf	KeyWk
	
	movlw	0x80		; E0��O�u����R�[�h���ǂ���?
	subwf	KeyWk,W		; KeyWk - 0x80 --> (W)
	jnc	KeySendF0_1	; �����Ȃ�����.	�����Ȃ��Ƃ� NC 

	movlw	0xE0		; E0 F0 xx �ɂ���.
	call	KeySendW

KeySendF0_1:
	movlw	0xf0
	call	KeySendW	; 0xf0

	jmp	KeySend_7Wk	; KeyWk�� xx �� MSB �|���đ��o.

;--------------------------------------------------------------
;	�L�[���[�N�R�[�h���o
;--------------------------------------------------------------
KeySend:
	movfw	KeyPos
	call	pc88toAT
	iorlw	0x00
	btfsc   STATUS, Z
	ret
	
	movwf	KeyWk
	
	movlw	0x80		; E0��O�u����R�[�h���ǂ���?
	subwf	KeyWk,W		; KeyWk - 0x80 --> (W)
	jnc	KeySend_1	; �����Ȃ�����.	�����Ȃ��Ƃ� NC 

	movlw	0xE0
	call	KeySendW

KeySend_7Wk:
	movlw	0x7F
	andwf	KeyWk,F
	
KeySend_1:
	movfw	KeyWk

;--------------------------------------------------------------
;	(W)�L�[�R�[�h���o
;--------------------------------------------------------------
KeySendW:
	movwf	SendByteLast
KeySendLast:
	movwf	SendByte

KeySendRxChk:
 	btfss	kbCLKi		; CLK��Lo�Ȃ��M����Ă݂�.
	call	KeyRecStart

	call	SendBitLo	; send start bit
	movl   	SendParity,1	; set ODD parity
	movl	SendCnt,8

KeySendLoop:
	movfw	SendByte
	xorwf	SendParity,F
	rrf	SendByte,F
	call	SendBit
	decfsz	SendCnt,F
	jmp	KeySendLoop

	rrf	SendParity,F	; check Parity
	call	SendBit

	PinHI	kbDATAo		; Stop Bit
	jmp	SendBitX

SendBit:
	skpnc
	PinHI	kbDATAo		    ; assume bit is 1
	skpc			    ; if CF=1, dit already set
SendBitLo:
	PinLO	kbDATAo		    ; otherwise, set pin to 0
SendBitX:
	call	Wait12uS

	PinLO	kbCLKo		    ;

	call	Wait20uS

	PinHI	kbCLKo		    ;

	ret


;--------------------------------------------------------------
;	KeyRecv1	;�Ȃɂ��������Ă����牞������.
;--------------------------------------------------------------
KeyRecv1:
 	btfsc	kbCLKi	; CLK��Hi�Ȃ牽��������return.
	ret

;; CLK��Lo�ɂȂ��Ă�����...
KeyRecStart:
 	btfss	kbCLKi	; CLK��Lo�̊ԃ��[�v.
	jmp	KeyRecStart	;

	btfsc	kbDATAi	; DATA��Hi�Ȃ�G���[return
	ret
	
;--------------------------------------------------------------
;	 �z�X�g�����p�^�[��:
;--------------------------------------------------------------

	call	KeyRecByte
KeyRespond:
	movlw	0xed
	subwf	RecByte,W	; RecByte - 0xed --> (W)
	jc	respond_W	; ������. ed�`ff�̃p�^���ɉ���.

;-------------------------------------------------------
;  undefined host commands.
respond_F1:
respond_EF:
	ret			;;�������Ȃ�.

;-------------------------------------------------------
;Set/reset status indicators.  This command allows you to control
;the status LEDs on the keyboard.  Keyboard responds with ACK and
;waits for a option byte, bitmapped as follows: b0-Scrollock,
;b1-Numlock, b2-Capslock, b3..7=0.  A '1' bit turns the indicator
;ON.
respond_ED:
	call	respond_FA
	call	KeyRecData
	ret
;-------------------------------------------------------
;Echo.  Responds with ECHO code (EE).
respond_EE:
	movlw	0xee
	jmp	KeySendW
;-------------------------------------------------------
; Select scan code set.  Responds with ACK, then waits for host to
; send a byte (01,02 or 03) specifying the scan code set to use.
; If 00 is sent, keyboard responds with ACK followed by the scan code
; set in use.
respond_F0:
	call	respond_FA
	call	KeyRecData
	movfw	RecByte
	jz	respond_F000
	movwf	ResScanType
	ret
respond_F000:
	;...
	ret

;-------------------------------------------------------
;Read ID.  Responds with ACK and two ID bytes (83,AB).  Resumes
;scanning even if previously disabled.
respond_F2:
	call	respond_FA
	movlw	0xab
	call	KeySendW
	movlw	0x83
	jmp	KeySendW

;-------------------------------------------------------
;Set autorepeat rate/delay.  Responds with ACK, then waits for an
;option byte that specifies the autorepeat delay and rate,
;bitmapped as follows: b7-unused. b6..5-Repeat delay (00=250 mS,
;11=1000mS). b4..0-Repeat rate (00000=30x/sec, 11111=2x/sec).
;Keyboard responds with ACK after reception of the option byte.
respond_F3:
	call	respond_FA
	call	KeyRecData
	movfw	RecByte
	movwf	ResRateDelay

;-------------------------------------------------------
;Enable.  Clears output buffer, enabled kbd, returns ACK.
respond_F4:
	jmp	respond_FA

;-------------------------------------------------------
;Default Disable.  Resets keyboard, returns ACK and suspends
;scanning, waiting for another command.  Does not affect the
;indicator LEDs.
respond_F5:
;-------------------------------------------------------
;Set default.  Responds as the 'Default Disable' command, but does
;not inhibit scanning.  Does not affect LED indicators.
respond_F6:
	call	respond_FA
	call	KeyRecData
	jmp	KeyRespond

;-------------------------------------------------------
;'Set all keys typematic'
respond_F7:
	bcf	RepOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set all keys make/break'
respond_F8:
	bcf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set all keys make'
respond_F9:
	bsf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set all keys typematic/make/break'
;I am not certain what these commands do.  I suspect they control the
;way the keyboard transmits scan codes.  All of the above commands
;respond with an ACK code.
respond_FA:
	movlw	0xfa
	jmp	KeySendW

;-------------------------------------------------------
;'Set key type typematic'
respond_FB:
	bcf	RepOFF
	jmp	respond_FA
;-------------------------------------------------------
;'Set key type make/break'
respond_FC:
	bcf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set key type make'
respond_FD:
	bsf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;Resend.  Retransmit last sent scan code.
respond_FE:
	movlw	SendByteLast
	jmp	KeySendLast
;-------------------------------------------------------
;Reset.  Resets keyboard CPU, starts power-on test.  Responds with
;power-on-test byte.
respond_FF:
	call	respond_FA
	movlw	0xaa
	jmp	KeySendW



;--------------------------------------------------------------
;	�z�X�g����̃f�[�^����M����.
;--------------------------------------------------------------
KeyRecData:
	movl	RecByte,0	;�_�~�[�f�[�^���Z�b�g.
KeyRecData0:
	btfsc	kbCLKi	; CLK��Hi�Ȃ�ҋ@.
	jmp	KeyRecData0

	;; CLK��Lo�ɂȂ��Ă�����...
KeyRecData1:
 	btfss	kbCLKi	; CLK��Lo�̊ԃ��[�v.
	jmp	KeyRecData1

	btfsc	kbDATAi	; DATA��Hi�Ȃ�G���[return
	ret

	;; CLK��Hi�ɖ߂��Ă��ADATA��Lo�̂܂܂Ȃ�A���M�v���Ƃ݂Ȃ�.

KeyRecByte:
;; �L�[�{�[�h������N���b�N���������Ď�M���s��.
	bsf    	SendParity,7	; set ODD parity
	movl	SendCnt,8
KeyRecLoop:
	call	KeyRecBit
	rrf	RecByte,F	; ��M����bit(Carry)���E�l�ߋL��.
;; �p���e�B�v�Z.
	movfw	RecByte
	andlw	0x80
	xorwf	SendParity,F

	decfsz	SendCnt,F
	jmp	KeyRecLoop

	call	KeyRecBit	; �p���e�B��M.
	call	KeyRecBit	; �X�g�b�v��M.
	
	call	Wait20uS
	PinLO	kbDATAo		; �n���h�V�F�C�Nbit���M
	call	Wait12uS
	PinLO	kbCLKo		;
	call	Wait12uS
	PinHI	kbCLKo		;
	PinHI	kbDATAo		;
	ret

KeyRecBit:
	call	Wait20uS
	PinLO	kbCLKo		    ;
	call	Wait12uS
	PinHI	kbCLKo		    ;
	clrc			    ; presume data=0
	btfsc	kbDATAi		    ; test data bit
	setc			    ; correct to 1
	ret


;--------------------------------------------------------------
;	
;--------------------------------------------------------------
	end
;
