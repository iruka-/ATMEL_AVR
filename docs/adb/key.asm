;======================================================================
;
;		�`�c�a to �o�r�^�Q �L�[�{�[�h�A�_�v�^
;
;======================================================================
;
;	�EMacintosh�̃L�[�{�[�h���`�s�݊��@�ɐڑ�����.
;
;�P�U�e�W�S�̔z��:(���������ꂾ��)
;----------------------------------------------------------------------
;  ADB��
;	PORTA	0	in/out	ADB data
;	PORTA	1	in/out	ADB reset(���g�p�Ȃ̂Ŕz���K�v�Ȃ�)
;  PS/2��
;	PORTB	0	in/out	KBD data
;	PORTB	1	in/out	KBD clock
;
;	��L�s����3.3k���ɂ�+5V�փv���A�b�v.
;
;	���g����10MHz(�������U)
;
;	���Z�b�g�̓v���A�b�v���邱��.
;
;
;��v���[�`���ƃ��[�N.
;----------------------------------------------------------------------
;	KeyScanMain()
;		keybuf		�`�c�a�����M����Apple���L�[�R�[�h
;				MSB=1�̏ꍇ�͂��̃L�[�������ꂽ���ƂɂȂ�.
;		KeyPos		0x00�`0x7e  keybuf�l��MSB���}�X�N��������.
;		KeyRow		KeyPos / 8  �̒l(bit table�����p)
;		KeyMask		0x01 .. 0x80 ... bitmask�l
;
;		KeyBits[16]	127���̊e�L�[��������Ă���(1)������Ă���(0)
;				����bit�ŋL������X�R�A�{�[�h
;
;
;The connector will be one of the following:
;5pin 180 deg DIN               6pin MINIDIN
;----------------               ------------
;                                
;                                    _
;       (2)      1 CLOCK          (5) (6)        1 DATA
;    (5)   (4)   2 DATA              |           2 
;   (3)     (1)  3              (3)     (4)      3 GND
;                4 GND                           4 +5V
;                5 5V             (1) (2)        5 CLOCK
;                                                6
;
;,-------,---,---,---,---,,---,---,---,---,,---,---,---,---,
;|ESC    |F1 |F2 |F3 |F4 ||F5 |F6 |F7 |F8 ||F9 |F10|F11|F12|
;|76     |05 |06 |04 |0c ||03 |0b |83 |0a ||01 |09 |78 |07 |
;'-------'---'---'---'---''---'---'---'---''---'---'---'---'
;,---,---,---,---,---,---,---,---,---,---,---,---,---,-----,
;|~  |1  |2  |3  |4  |5  |6  |7  |8  |9  |0  |-  |=  |BS   |
;|0e |16 |1e |26 |25 |2e |36 |3d |3e |46 |45 |4e |55 |66   |
;|---',--',--',--',--',--',--',--',--',--',--',--',--',----|
;|TAB |Q  |W  |E  |R  |T  |Y  |U  |I  |O  |P  |[  |]  |\   |
;|0d  |15 |1d |24 |2d |2c |35 |3c |43 |44 |4d |54 |5b |5d  |
;|----',--',--',--',--',--',--',--',--',--',--',--',--'----|
;|CAPS |A  |S  |D  |F  |G  |H  |J  |K  |L  |;  |,  |ENTER  |
;|58   |1c |1b |23 |2b |34 |33 |3b |42 |4b |4c |52 |5a     |
;|-----',--',--',--',--',--',--',--',--',--',--',----------|
;|SHIFT |Z  |X  |C  |V  |B  |N  |M  |,  |.  |/  |SHIFT     |
;|12    |1a |22 |21 |2a |32 |31 |3a |41 |49 |4a |59        |
;|----,-',--'--,'---'---'---'---'---'---'--,'--,'-----,----|
;|CTRL|  |ALT  |          SPACE            |ALT|      |CTRL|
;|14  |  |11   |          29               |e011      |e014|
;'----'--'-----'---------------------------'---'------'----'
; 
;,-----,-----,-----,
;|PSCRN|SLOCK|BREAK|
;|*2   |7e   |*4   |
;'-----'-----'-----'
;*2 is E012E07C
;*4 is E11477E1F014F077
;,-----,-----,-----,  ,-----,-----,-----,-----,
;|INS  |HOME |PGUP |  |NLOCK|/    |*    |-    |
;|e070 |e06c |e07d |  |77   |e04a |7c   |7b   |
;|-----|-----|-----|  |-----|-----|-----|-----|
;|DEL  |END  |PGDN |  |7    |8    |9    |+    |
;|e071 |e069 |e07a |  |6c   |75   |7d   |79   |
;'-----'-----'-----'  |-----|-----|-----|-----|
;                     |4    |5    |6    |     |
;                     |6b   |73   |74   |     |
;      ,-----,        |-----|-----|-----|-----|
;      |UP   |        |1    |2    |3    |ENTER|
;      |e075 |        |69   |72   |7a   |e05a |
;,-----|-----|-----,  |-----------|-----|     |
;|LEFT |DOWN |RIGHT|  |0          |.    |     |
;|e06b |e072 |e074 |  |70         |71   |     |
;'-----'-----'-----'  '-----------'-----'-----'
; 
;There are at least three options for the mapping of key numberss to
;keycodes.  The common usage seems to be as follows:
;
;F1 F2    `  1  2  3  4  5  6  7  8  9  0  -  =  \ BS   ESC NUML SCRL SYSR 
;-----   ---------------------------------------------  ------------------
;05 06   0E 16 1E 26 25 2E 36 3D 3E 46 45 4E 55 5D 66    76  77   7E   84
;
;F3 F4   TAB   Q  W  E  R  T  Y  U  I  O  P  [  ]       Home Up  PgUp PrtSc
;-----   -----------------------------------------      -------------------
;04 0C    0D  15 1D 24 2D 2C 35 3C 43 44 4D 54 5B        6C  75   7D   7C
;
;F5 F6   CNTL   A  S  D  F  G  H  J  K  L  ;  ' ENTER   Left  5  Right  -
;-----   --------------------------------------------   ------------------
;03 0B    14   1C 1B 23 28 34 33 38 42 4B 4C 52   5A     6B  73   74   7B
;
;F7 F8   LSHFT    Z  X  C  V  B  N  M  ,  .  /  RSHFT   End  Dn  PgDn   +
;-----   --------------------------------------------   ------------------
;83 0A     12    1A 22 21 2A 32 31 3A 41 49 4A   59      69  72   7A   79
;
;F9 F10  ALT                  SPC             CAPLOCK   Ins      Del
;------  --------------------------------------------   -------------
;01 09    11                   29               58       70       71


#include "p16f84.inc"
#include "custom.h"

	radix	DEC

;	.osc		hs		; �I�V���[�^�g�r
;	.wdt		off		; �E�H�b�`�h�b�O�n�e�e
;	.pwrt		on		; �p���[�A�b�v�^�C�}�n�m
;	.protect	off		; �v���e�N�g�n�e�e


; KeyBoard �o�� �͐��_��
PinLO  macro P
	call	P
 endm


PinHI  macro P
	call	P
 endm


rz	  macro			   ; return if Zero
	    btfsc  STATUS, Z
	    return
	  endm

rnz	  macro			   ; return if Not Zero
	    btfss  STATUS, Z
	    return
	  endm

rc	  macro			   ; return if Carry
	    btfsc  STATUS, C
	    return
	  endm

rnc	  macro			   ; return if Not Carry
	    btfss  STATUS, C
	    return
	  endm

;--------------------------------------------------------------
;	�X�^�[�g.
;--------------------------------------------------------------
	jmp	_main	;;0000
	jmp	_main	;;0001
	jmp	_main	;;0002
	jmp	_main	;;0003
	jmp	_intr	;;0004

;--------------------------------------------------------------
;	�L�[�X�L�����E���[�N
;--------------------------------------------------------------
 New	KeyPos		;;0..88      key number
 New	KeyRow		;;0..11 ---  74159N select signal
;;; New	KeyCol		;;0..7  .... bitpos
 New	KeyMask		;;0x01 .. 0x80 ... bitmask
 New	KeyVal		;;KeyRow�ɑΉ�����r�b�g�p�^��.
 New	KeyWk		;;�ėp���[�N.

 New	KeyRepCnt	;;�L�[���s�[�g�J�E���^.
 New	KeyRepFlag	;;�L�[���s�[�g�t���O.
 New	KeyRepCode	;;�L�[���s�[�g�������L�[.

 New	ScanMode	;;

 New	PORTBdir	;; 0:�o�� 1:����
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
;	�`�c�a�p
;--------------------------------------------------------------
 New	Breg		;; 1�o�C�g���M����f�[�^.
 New	Creg		;; 8bit���𐔂���J�E���^.
 New	Ereg		;; 1�o�C�g��M�����f�[�^.
 New	keybuf		;; getkey�󂯓n���p.
;--------------------------------------------------------------
;	�r�b�g�̒�`
;--------------------------------------------------------------
#define	 KeyREPCLR	KeyRepFlag,7	;; �L�[��Ԃ��ω�����.
#define	 KeyREPRESS	KeyRepFlag,6	;; �L�[�͂P�ȏエ����Ă���.
					;; ADB�ɂ����ẮA���s�[�g�\�ȃL�[�̃��[�N������������1
					;;    �ɂ��A���s�[�g�\�ȃL�[�̃u���[�N������0�ɖ߂�.

#define	 BrkOFF		ScanMode,0	;;�u���[�N�R�[�h�͑���Ȃ�.
#define	 RepOFF		ScanMode,1	;;�L�[���s�[�g�R�[�h�͑���Ȃ�.

;--------------------------------------------------------------
;	�|�[�g���蓖�Ă̒�`
;--------------------------------------------------------------
#define  kbCLKi   	PORTB, 1	; clock line input
#define  kbCLKo   	PORTB, 1	; clock line output(inverted OpenCollector)
#define  kbDATAi  	PORTB, 0	; data line input
#define  kbDATAo  	PORTB, 0	; data line output(inverted OpenCollector)

#define	 ADBi		PORTA, 0

#define  ADBTIME	25		; 50uS ���X���b�V�����h�^�C��.

#define	 ADBshift	KeyBits7,0	; ADB�� [SHIFT] �L�[�ԍ��� 0x38 , bit�ʒu�͂���.
#define	 ADBctrl	KeyBits6,6	; ADB�� [CTRL ] �L�[�ԍ��� 0x36 , bit�ʒu�͂���.

#define	 MARK		0x00
#define	 SPACE		0x01
;--------------------------------------------------------------
;	�L�[�X�L�����E���ʃe�[�u��
;--------------------------------------------------------------
;	�W�r�b�g�~�P�U�q������128�L�[���B������Ă�����P������.

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
 New	KeyBits12
 New	KeyBits13
 New	KeyBits14
 New	KeyBits15


;--------------------------------------------------------------
;  �`�c�a�̃X�L�����R�[�h���P�O�U�z��̃X�L�����R�[�h�ɕϊ�
;--------------------------------------------------------------
#include "adb.inc"

;--------------------------------------------------------------
;  �`�c�a�̃L�[�Ń��s�[�g�������r�b�g�Ɉ��t����.
;--------------------------------------------------------------
KeyRepMask:
	addwf	PC,F
	retlw	B'00000000'	; 0x00
	retlw	B'00000000'
	retlw	B'00000000'	; 0x10
	retlw	B'00000000'
	retlw	B'00000000'	; 0x20
	retlw	B'00000000'
	retlw	B'11000000'	; 0x30
	retlw	B'00000111'
	retlw	B'00000000'	; 0x40
	retlw	B'00000000'
	retlw	B'00000000'	; 0x50
	retlw	B'00000000'
	retlw	B'00000000'	; 0x60
	retlw	B'00000000'
	retlw	B'00000000'	; 0x70
	retlw	B'00000000'

;--------------------------------------------------------------
;	�J�����ԍ�����A����PORT�̃}�X�N�p�^�[���ւ̕ϊ�.
;--------------------------------------------------------------
Col2Mask:
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
;	���荞�ݏ����J�n.
;--------------------------------------------------------------
_intr:
	nop
	bcf	INTCON,RBIF	;;���荞�݃t���O�N���A
	retfie			;;���荞�ݕ��A.
;--------------------------------------------------------------
;	���C�������J�n.
;--------------------------------------------------------------
_main:
	nop
	;;�|�[�g�̓��o�͕��������߂�.
	SetRPage 1
	bcf	INTCON,GIE	;;���荞�݋֎~

	movlw	B'00000000'	;;PORTB pull UP
	movwf	OPTION_REG


	movlw	B'11100011'	;; bit 2,3,4�̂ݏo��.���͓���
	movwf	PORTA

	movlw	B'00000011'	;; bit 0 ,1�̂ݓ���.���͏o��
	movwf	PORTBdir
	movwf	PORTB

	SetRPage 0


	;;�o�̓s���̒l������������.
	bsf	kbCLKo
	bsf	kbDATAo

	;;�V�X�e���S�̂����肷��܂ő҂�.
	call	Wait400m

	;;�f�o�b�O�̂��߂�main
	;;jmp	testmain

	;;�`�c�a������.
	movlw	0x00
	call	adb_cmd
	call	Wait400m

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
	movl	KeyPos,0x7f	;;�L�[�Z���X�|�C���^.
	bcf	KeyREPCLR	;;�L�[�ω��t���O��������.

;;�L�[��S���Z���X���郋�[�v�̐擪:
KeyScan_2:
	call	KeyRecv1	;;�Ȃɂ������Ă������M����.
	call	KeyScan1	;;�`�c�a�L�[���Z���X����.

;;�����������ƂŁA��������ςȂ��L�[������΁A���s�[�g����������.
	btfsc	KeyREPCLR
	jmp	KeyScan_0	;;�ω����������̂ŁA���s�[�g�L�����Z��.

	call	RepChk		;;KeyRepCode�����̃L�[�͉�����Ă���H.
	jz	KeyScan_0	;;�L�[�͉���������Ă��Ȃ�.

	btfsc	RepOFF
	jmp	KeyScan_0	;;�^�C�p�}�e�B�b�N���M���Ȃ�.

	decfsz	KeyRepCnt,F	;;���s�[�g�J�n�J�E���g�ɒB����܂Ń��[�v.
	jmp	KeyScan_1

;;���s�[�g�̎��s.
	movfw	KeyRepCode
	movwf	KeyPos
	call	KeySend

	movl	KeyRepCnt,0xc	;;���s�[�g�J�n�J�E���g�_�E���J�E���^�����߂�.
	jmp	KeyScan_1

;--------------------------------------------------------------
;	�L�[�X�L�����E�������[�`��
;--------------------------------------------------------------
;
;	�҂����Ԃ̓N���b�N�P�O�l�g���Ƃ��Đݒ�.
;	1 Cycle = 4 / 10,000,000 = 0.4uS
;
;	KeyW_1() �� ���[�v�P��ɕt���A1.2uS �����.
;
;

Wait12uS:
	movlw	6		;10
	jmp	KeyWait
Wait20uS:
	movlw	14		;18
	jmp	KeyWait
w250us:
	movlw	0xf8		;248
;;;	jmp	KeyWait
KeyWait:
	movwf	SendWk		; 1
KeyW_1:
	decfsz	SendWk,F	; 1
	jmp	KeyW_1		; 2

	return			; 2

w800us:
	call	w250us
	call	w250us
	call	w250us
	movlw	50
	jmp	KeyWait
;--------------------------------------------------------------
;	
;--------------------------------------------------------------
Wait400m:
	movl	KeyWk,0		;1
_loop1:
	call	w800us
	call	w800us
	decfsz	KeyWk,F
	jmp	_loop1
	return


;--------------------------------------------------------------
;	KeyBits[KeyRow] �e�[�u�����C���f�b�N�X�Q�Ƃ���ׂ̏���.
;--------------------------------------------------------------
KeyBitAdr:
	movlw	KeyBits
	addwf	KeyRow,W
	movwf	FSR		;; �Ԑ�Register = KeyBits+KeyRow
	return

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g����������(Z �t���O).
;--------------------------------------------------------------
KeyBitChk:
	call	KeyBitAdr
	movfw	KeyMask
	andwf	INDF,W		;; �������[�ԐڎQ��.
	return

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g�𗧂Ă�.
;--------------------------------------------------------------
KeyBitSet:
	call	KeyBitAdr
	movfw	KeyMask
	iorwf	INDF,F		;; �������[�ԐڎQ��.
	return

;--------------------------------------------------------------
;	KeyPos�ɓ�����r�b�g��|��.
;--------------------------------------------------------------
KeyBitReset:
	call	KeyBitAdr
	movfw	KeyMask
	xorlw	0xff		;�}�X�N�𔽓]���ā������.
	andwf	INDF,F		;; �������[�ԐڎQ��.
	return

;--------------------------------------------------------------
;	�L�[�r�b�g�z���S����������.
;--------------------------------------------------------------
KeyBitsInit:
	clrf	KeyRow
	movl	KeyWk,16

KeyBitsInit1:
	call	KeyBitAdr
	clrf	INDF
	incf	KeyRow,F
	decfsz	KeyWk,F
	jmp	KeyBitsInit1
	return


;--------------------------------------------------------------
;	���݃X�L���������L�[�����s�[�g�s�������� Not Zero �ɂ���
;--------------------------------------------------------------
KeyRepMaskChk:
	movfw	KeyRow
	call	KeyRepMask
	andwf	KeyMask,W
	return

;--------------------------------------------------------------
;	���s�[�^�u���ȃL�[���܂�������Ă��邩�H
;--------------------------------------------------------------
KeyRepScan:
	movl	KeyWk,16
	movl	KeyVal,0
KeyRepScal:
	movlw	KeyBits
	addwf	KeyVal,W
	movwf	FSR		;; �Ԑ�Register = KeyBits+KeyVal

	movfw	KeyVal
	call	KeyRepMask	;; ���s�[�g�������Ȃ��L�[���P�̃e�[�u��.
	xorlw	0xff		;�}�X�N�𔽓]���ā������.
	andwf	INDF,W		;; �������[�ԐڎQ��. 
	rnz		; ===�����s�[�^�u���ȃL�[��������Ă����I

	incf	KeyVal,F
	decfsz	KeyWk,F
	jmp	KeyRepScal

	andlw	0x00
	return	; zero return ���ǉ�����Ă��Ȃ�����.

SetRowMask:
	movwf	KeyRow

;	����p�Ƃ��āAKeyRow,KeyCol,KeyMask ���Z�b�g�����.
	;;; KeyRow = KeyPos / 8;
	rrf	KeyRow,F
	rrf	KeyRow,F
	rrf	KeyRow,W
	andlw	0x0f
	movwf	KeyRow

	;;; KeyCol = KeyPos & 7
	movfw	KeyPos
	andlw	0x07
	;;;movwf KeyCol
	
	;;; KeyMask = masktable[KeyCol]
	call	Col2Mask
	movwf	KeyMask
	return

CapsOnOff:
	call	KeySend		; [CAPS] On
	call	KeySendF0	; [CAPS] Off
	return

;--------------------------------------------------------------
;	KeyPos�Ԗڂɓ�����L�[���P��������
;--------------------------------------------------------------
;	�ω����L������A�R�[�h���݊��@�ɑ��M����.
KeyScan1:
	call	get_key
	incf	keybuf,W	;; keybuf �� 0xff�ȊO�Ȃ���͂���.
	rz			;; ���͂Ȃ��Ȃ烊�^�[��.

	;;; KeyPos = keybuf & 0x7f
	movfw	keybuf
	andlw	0x7f
	movwf	KeyPos		;;�`�c�a�̃L�[�ԍ���KeyPos�ɋL��������.
	call	SetRowMask	;; KeyPos ���� KeyRow��KeyMask���v�Z.

;�r�b�g�̕ω��L��.
	bsf	KeyREPCLR	;;�ω�����������A�L�[�ω��t���O��On

;[CAPS]�̂݁A���ꏈ��
 if	0
	movlw	0x39
	subwf	KeyPos,W
	jz	CapsOnOff
 endif
;���[�N���u���[�N���̔���.
	btfsc	keybuf,7	;;MSB��1�Ȃ�`�c�a����̃L�[�J�����
	jmp	KeyTurnOff

;�L�[�������ꂽ.
KeyTurnOn:
	call	KeyBitSet
	call	KeySend
	call	KeyRepMaskChk
	rnz
;;���s�[�g�\�ȃL�[�̏ꍇ�A���̃R�[�h�����߂�.
	movfw	KeyPos
	movwf	KeyRepCode
	return

;�L�[�������ꂽ.
KeyTurnOff:
	call	KeyBitReset

	btfss	BrkOFF		;;�L�[�u���[�N���ɃR�[�h�𑗂�? bit�������Ă����瑗��Ȃ�.
	call	KeySendF0


	movfw	KeyPos
	subwf	KeyRepCode,W
	rnz			;;�����ꂽ�L�[�́A���s�[�g�����Ă��L�[�Ɠ��ꂾ����.

	movl	KeyRepCode,0x7f	;���s�[�g�L�[���Ƃ肠���������ɂ��Ă���.
	return


;���s�[�g�\�ȃL�[���A�܂�������Ă��邩�ǂ����𔻒f����.
RepChk:
	movfw	KeyRepCode
	movwf	KeyPos
	call	SetRowMask	;; KeyPos ���� KeyRow��KeyMask���v�Z.
	call	KeyBitChk

	return


;--------------------------------------------------------------
;	�L�[�u���[�N�R�[�h���o
;--------------------------------------------------------------
KeySendF0:
	movfw	KeyPos
	call	ADBtoAT
	iorlw	0x00
	jz	kF0_00

	movwf	KeyWk
	
	movlw	0x90		; E0��O�u����R�[�h���ǂ���?
	subwf	KeyWk,W		; KeyWk - 0x90 = (W)
	jnc	KeySendF0_1	; �����Ȃ�����.	�����Ȃ��Ƃ� NC 

	movlw	0xE0		; E0 F0 xx �ɂ���.
	call	KeySendW

KeySendF0_1:
	movlw	0xf0
	call	KeySendW	; 0xf0

	jmp	KeySend_7Wk	; KeyWk�� xx �� MSB �|���đ��o.

; 1�o�C�g�\�Œ�`�s�L�[�̏���.
kF0_00:
	movfw	KeyPos
	sublw	0x69		; PRINT SCREEN
	jz	kF0_69

	movfw	KeyPos
	sublw	0x71		; PAUSE
	jz	kF0_71
	return

; PRINT SCREEN
kF0_69:
	btfsc	ADBshift	; [SHIFT]��������Ă���?
	jmp	kF1_69
	btfsc	ADBctrl		; [CTRL ]��������Ă���?
	jmp	kF1_69

; �P�Ƃ� PRINTSCREEN �̃����[�X�� E0 F0 7c E0 F0 12 �𑗏o.
	movlw	0x7c
	call	kE0_F0_W		; E0 F0 7C
	movlw	0x12
	jmp	kE0_F0_W		; E0 F0 12

; [ SHIFT | CTRL ] + PRINTSCREEN �̃����[�X�� SysRq(E0 F0 7C) �𑗏o.
kF1_69:
	movlw	0x7c
	jmp	kE0_F0_W		; E0 F0 7C

; PAUSE
kF0_71:
; �P�Ƃ� PAUSE �̓����[�X�R�[�h��Ԃ��Ȃ��Ă悢.
	return




; 1�o�C�g�\�Œ�`�s�L�[�̏���.
kM0_00:
	movfw	KeyPos
	sublw	0x69		; PRINT SCREEN
	jz	kM0_69

	movfw	KeyPos
	sublw	0x71		; PAUSE
	jz	kM0_71
	return

; PRINT SCREEN = E0 12 E0 7C
kM0_69:
	btfsc	ADBshift	; [SHIFT]��������Ă���?
	jmp	kM1_69
	btfsc	ADBctrl		; [CTRL ]��������Ă���?
	jmp	kM1_69

; �P�Ƃ� PRINTSCREEN �� E0 12 E0 7C �𑗏o.

	movlw	0x12
	call	kE0_W		; E0 12

; [ SHIFT | CTRL ] + PRINTSCREEN �� SysRq(E07C) �𑗏o.
kM1_69:
	movlw	0x7c
	jmp	kE0_W		; E0 7C

; PAUSE = E1 14 77 E1 F0 14 F0 77
kM0_71:
	btfsc	ADBshift	; [SHIFT]��������Ă���?
	jmp	kM1_71
	btfsc	ADBctrl		; [CTRL ]��������Ă���?
	jmp	kM1_71

; �P�Ƃ� PAUSE �� E1 14 77   E1 F0 14 F0 77 �𑗏o.
	movlw	0x14
	call	kE1_W		; E1 14
	
	movlw	0x77
	call	KeySendW	; 77
	movlw	0xe1
	call	KeySendW	; E1

	movlw	0x14
	call	kF0_W		; F0 14

	movlw	0x77
	call	kF0_W		; F0 77
	return

; [ SHIFT | CTRL ] + PAUSE �� Break(E0 7E E0 F0 7E) �𑗏o.
kM1_71:
	movlw	0x7e
	call	kE0_W		; E0 7E

	movlw	0x7e		; E0 F0 7E

; E0 F0 xx
kE0_F0_W:
	movwf	KeyWk

	movlw	0xE0
	call	KeySendW	; E0

	jmp	kF0_Wx

; F0 xx
kF0_W:
	movwf	KeyWk
kF0_Wx:
	movlw	0xF0
	jmp	kW_Wk		; F0 Wk

; E1 xx
kE1_W:
	movwf	KeyWk

	movlw	0xE1
	jmp	kW_Wk		; E1 Wk

; E0 xx
kE0_W:
	movwf	KeyWk
	movlw	0xE0
kW_Wk:
	call	KeySendW
	
	movfw	KeyWk
	call	KeySendW
	return


;--------------------------------------------------------------
;	�L�[���[�N�R�[�h���o
;--------------------------------------------------------------
KeySend:
	movfw	KeyPos
	call	ADBtoAT
	iorlw	0x00
	jz	kM0_00
	
	movwf	KeyWk
	
	movlw	0x90		; E0��O�u����R�[�h���ǂ���?
	subwf	KeyWk,W		; KeyWk - 0x90 = (W)
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

	jmp	SendBitHi	; Stop Bit

SendBit:
	jnc	SendBitLo	;;skpnc

SendBitHi:
	PinHI	kbDATAhi	    ; assume bit is 1
	jmp	SendBitX	;;skpc			    ; if CF=1, dit already set

SendBitLo:
	PinLO	kbDATAlo	    ; otherwise, set pin to 0
SendBitX:
	call	Wait12uS

	PinLO	kbCLKlo		    ;

	call	Wait20uS

	PinHI	kbCLKhi		    ;

	return


;--------------------------------------------------------------
;	KeyRecv1	;�Ȃɂ��������Ă����牞������.
;--------------------------------------------------------------
KeyRecv1:
 	btfsc	kbCLKi	; CLK��Hi�Ȃ牽��������return.
	return

;; CLK��Lo�ɂȂ��Ă�����...
KeyRecStart:
 	btfss	kbCLKi	; CLK��Lo�̊ԃ��[�v.
	jmp	KeyRecStart	;

	btfsc	kbDATAi	; DATA��Hi�Ȃ�G���[return
	return
	
;--------------------------------------------------------------
;	 �z�X�g�����p�^�[��:
;--------------------------------------------------------------

	call	KeyRecByte
KeyRespond:
	movlw	0xed
	subwf	RecByte,W	; RecByte - 0xed = (W)
	jc	respond_W	; ������. ed�`ff�̃p�^���ɉ���.

;-------------------------------------------------------
;  undefined host commands.
respond_F1:
respond_EF:
	return			;;�������Ȃ�.

;-------------------------------------------------------
;Set/reset status indicators.  This command allows you to control
;the status LEDs on the keyboard.  Keyboard responds with ACK and
;waits for a option byte, bitmapped as follows: b0-Scrollock,
;b1-Numlock, b2-Capslock, b3..7=0.  A '1' bit turns the indicator
;ON.
respond_ED:
	call	respond_FA
	call	KeyRecData
	return
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
	return
respond_F000:
	;...
	return

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
	movfw	SendByteLast
	jmp	KeySendW
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
	return

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
	PinLO	kbDATAlo	; �n���h�V�F�C�Nbit���M
	call	Wait12uS
	PinLO	kbCLKlo		;
	call	Wait12uS
	PinHI	kbCLKhi		;
	PinHI	kbDATAhi	;
	return

KeyRecBit:
	call	Wait20uS
	PinLO	kbCLKlo		    ;
	call	Wait12uS
	PinHI	kbCLKlo		    ;
	clrc			    ; presume data=0
	btfsc	kbDATAi		    ; test data bit
	setc			    ; correct to 1
	return




; =============================================================
; ����ȍ~�͂`�c�a�o�X�̃h���C�u���[�`��

;--------------------------------------------------------------
;	ADB SUPPORT ROUTINES
;--------------------------------------------------------------

;==============================
;	�P�r�b�g���M.
;==============================

;// Cflag=1 bit1�𑗐M.
;// Cflag=0 bit0�𑗐M.
sadbit:
	jnc	sadbit0

	;if(cy) {
	;//     -+   +--------+
	;//      +---+        +-
	;//      35us   65us
	;//
sadbit1:
	movl	PORTA,MARK
	call	w35us
	movl	PORTA,SPACE
;	jmp	w65us
w65us:
	movlw	52
	jmp	KeyWait
	;}else{
	;//     -+        +---+
	;//      +--------+   +-
	;//       65us     35us
	;//
sadbit0:
	movl	PORTA,MARK
	call	w65us
	movl	PORTA,SPACE
;	jmp	w35us
w35us:
	movlw	27
	jmp	KeyWait

;==============================
;	�P�o�C�g���M( Breg )
;==============================

sadbyte:
	movl	Creg,8
sadloop:
	;do {
		;rlc(b);		;// MSB -> cy flag;
		rlf	Breg,F	; Breg�����V�t�g���AMSB�̂��ӂ��Carry�r�b�g�Ɉڂ�.
		call	sadbit;
	;}while(--c);
	decfsz	Creg,F
	jmp	sadloop

	jmp	sadbit0		;// STOP BIT


;// �A�e���V����(800us �̃}�[�N�M���𑗂�A70us�҂�)
adb_atn:
	movl	PORTA,MARK
	call	w800us
	movl	PORTA,SPACE
	jmp	w65us

adb_cmd:
	;;b=a
	movwf	Breg
	call	adb_out
	 call	adb_atn
	 call	sadbyte
	call	adb_inp
	return

;--------------------------------------------------------------
;	�`�c�a��M��
;--------------------------------------------------------------
;//	READ ADB by ONE BIT (Duty TIME=> C reg) 

; ADB�|�[�g���O�ɂȂ�܂ő҂�.
cadbitst:
	clrf	Creg
	clrc
cadbilop:
		btfss	ADBi	;ADBi �r�b�g�� 0(low)�Ȃ烊�^�[��
		return;

	decfsz	Creg,F
	jmp	cadbilop	; -- Lreg != 0 �Ȃ烋�[�v.
	setc
	return

;
;	�^�C���A�E�g�P�\�̒���1�r�b�g��M.
;
radbit0:
	movl	Breg,2
radbit0_l:
		call	cadbitst
		jnc	cadbited

	decfsz	Breg,F
	jmp	radbit0_l	; -- Breg != 0 �Ȃ烋�[�v.
	setc
	return


;
;	�P�r�b�g��M ==> Creg ��MARK�̃^�C�����L��.
;		�^�C���I�[�o�[�� CarryFLAG=1�ɂ���.
;
radbit:
	call	cadbitst	;// '0'�ɂȂ�̂�҂�.
	rc			;;if(cy) return;
;;;	jmp	cadbited	;// '1'�ɂȂ�̂�҂�.

;
;	���[�v�P��5 clock = 2uS (10MHz)
;
cadbited:
	clrf	Creg
	clrc
cadbitel:
		btfsc	ADBi	;ADBi �r�b�g�� 1(high)�Ȃ烊�^�[��
		return;

	incfsz	Creg,F
	jmp	cadbitel	; -- Lreg != 0 �Ȃ烋�[�v.
	setc
	return



;
;	�P�o�C�g��M ==> Ereg
;
radbyte:
	clrf	Ereg
	movl	Breg,8
radbytel:
;	e=0;
;	b=8;do {
		call	radbit
		rc		;;if(cy) return;

		movfw	Creg
		sublw	ADBTIME ;; ADBTIME - Creg => W , Carry
		rlf	Ereg,F	; ��M����bit(Carry)�����l�ߋL��.
;		a=c;a-=0x0e;
;		a=e;adc(a,a);e=a;
;		b--;
;	}while(nz);
	decfsz	Breg,F
	jmp	radbytel

	clrc
	return
;
;
get_key:
	incf	Ereg,W
	jz	getkey1
	;; Ereg �� ��M�f�[�^���c���Ă���̂ŁA���̒l��Ԃ�.
		movfw	Ereg
		movwf	keybuf
		movl	Ereg,0xff
		return

;	=======================================================
;	�`�c�a�ɃZ���X�R�}���h�𔭍s���ăf�[�^����M����.
;
;	���ʂ́Akeybuf �� Ereg �ɓ���. (�Q�o�C�g����M���邽��)
;
;	���ʂ̒l�͊e�L�[�̌ŗL�l(0x00�`0x7e)
;	 ����MSB�������Ă���ꍇ�́A���̃L�[�������ꂽ�C�x���g.
;	 0xff ���Ԃ��Ă����ꍇ�́A�L�[�C�x���g�����Ȃ�.
;	 �����ɂQ�o�C�g�̏�񂪂���ꍇ������i�C�x���g�����܂��Ă���ꍇ�j
;	 ���A���̏ꍇ�Akeybuf�݂̂ɓ���AEreg��0xff�ł��邱�Ƃ������B
;	=======================================================
getkey1:
	movlw	0xff		; �L�[�o�b�t�@����ɂ��Ă���.
	movwf	keybuf
	movwf	Ereg		; Ereg�����ł�.

	movlw	0x2c		; �`�c�a�Z���X�R�}���h���s.
	call	adb_cmd

	call	radbit0		; �X�^�[�g�r�b�g�擾.
	rc			;;if(cy) {return;}
;	a=c;if(a>=0x0e) {return;}

	call	radbyte		; �W�r�b�g��M
	jc	getkey_err
	movfw	Ereg
	movwf	keybuf		;	       =>keybuf

	call	radbyte		; �W�r�b�g��M =>Ereg
	jc	getkey_err
	call	radbit		; �X�g�b�v�r�b�g
	return

getkey_err:
	movlw	0xff		; �L�[�o�b�t�@����ɂ��Ă���.
	movwf	keybuf
	movwf	Ereg		; Ereg�����ł�.
	return

;===========================================
adb_inp:
	;;�|�[�g�̓��o�͕��������߂�.
	SetRPage 1

	movlw	B'11100011'	;; bit 2,3,4�̂ݏo��.���͓���
	movwf	PORTA

	SetRPage 0
	return

;===========================================
adb_out:
	;;�|�[�g�̓��o�͕��������߂�.

	SetRPage 1

	movlw	B'11100010'	;; bit 2,3,4�̂ݏo��.���͓���
	movwf	PORTA

	SetRPage 0

	return

;===========================================
kbCLKlo:
	bsf	STATUS, RP0	;;SetRPage 1
	bcf	PORTBdir,1
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTB
	return
;===========================================
kbCLKhi:
	bsf	STATUS, RP0	;;SetRPage 1
	bsf	PORTBdir,1
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	return
;===========================================
kbDATAlo:
	bsf	STATUS, RP0	;;SetRPage 1
	bcf	PORTBdir,0
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTB
	return
;===========================================
kbDATAhi:
	bsf	STATUS, RP0	;;SetRPage 1
	bsf	PORTBdir,0
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	return
;===========================================
ADBlo:
	movlw	MARK
	bsf	STATUS, RP0	;;SetRPage 1
	movwf	PORTA
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTA
	return
;===========================================
ADBhi:
	movlw	SPACE
	bsf	STATUS, RP0	;;SetRPage 1
	movwf	PORTA
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTA
	return
;===========================================

 if	0

;	�r�[�v�T�E���h�ɂ��f�o�b�O
;	PORTA bit4 �ɃX�s�[�J�[(�A���v��)���Ȃ�

;	�P�������̋�`�g�o��.
beep1cyc:
	movl	PORTA,0x11
	call	w800us
	movl	PORTA,0x01
	jmp	w800us

;	�Z�����U��.
beep1:
	movl	Creg,20
	jmp	bloop

;	�������U��.
beep:
	movl	Creg,0
bloop:
	call	beep1cyc
	decfsz	Creg,F
	jmp	bloop
	return

;	�������U���Ɠ��������̒���.
beep0:
	movl	Creg,0
bloop0:
	call	w800us
	call	w800us
	decfsz	Creg,F
	jmp	bloop0
	return



;--------------------------------------------------------------
;	
;--------------------------------------------------------------
testmain:
	call	beep
	movlw	0x00
	call	adb_cmd
	call	Wait400m
testloop:
	movlw	0x2c
	call	adb_cmd
	call	adb_data
	jmp	testloop

 endif

	end
;
