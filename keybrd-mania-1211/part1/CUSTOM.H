       List
; PIC 16C61, 71, 84   Custom definitions
       NoList

;**************************	PIC16CXX Header *************************
;
;		 NoEXPAND
		List n=0

;
VReset	equ	0	       ; Reset Vector
VIrq	equ	4	       ; Interrupt Vector
;
;
Enable	equ	1
Disable equ	0
NC	equ	254
NZ	equ	255

PC	equ	PCL	       ; PCL is defined in .inc files
;
;*******************************************************************************
; Predefined instructions
;
;	    Name	       Mnemonic       Equivalent       Status
;					     Operation(s)
; Clear Carry		     CLRC	   BCF	    3,0 	  -
; Set Carry		     SETC
; Clear Digit Carry	     CLRDC	   BCF	    3,1 	  -
; Set Digit Carry	     SETDC	   BSF	    3,1 	  -
; Clear Zero		     CLRZ	   BCF	    3,2 	  -
; Set Zero		     SETZ	   BSF	    3,2 	  -
; Skip on Carry 	     SKPC	   BTFSS    3,0 	  -
; Skip on No Carry	     SKPNC	   BTFSC    3,0 	  -
; Skip on Digit Carry	     SKPDC	   BTFSS    3,1 	  -
; Skip on No Digit Carry     SKPNDC	   BTFSC    3,1 	  -
; Skip on Zero		     SKPZ	   BTFSS    3,2 	  -
; Skip on Non Zero	     SKPNZ	   BTFSC    3,2 	  -
; Test File		     TSTF f	   MOVF     f,1 	  Z
; Move File to W	     MOVFW f	   MOVF     f,0 	  Z
; Negate File		     NEGF f,d	   COMF     f,1
;					   INCF     f,d 	  Z
; Add Carry to File	     ADDCF f,d	   BTFSC    3,0
;					   INCF     f,d 	  Z
; Subtract Carry from File   SUBCF f,d	   BTFSC    3,0
;					   DECF     f,d 	  Z
; Add Digit Carry to File    ADDDCF f,d    BTFSC    3,1
;					   INCF     f,d 	  Z
; Subtract Digit	     SUBDCF f,d    BTFSC    3,1
; Carry from File			   DECF     f,d 	  Z
; Branch		     B k	   GOTO     k		  -
; Branch on Carry	     BC k	   BTFSC    3,0
;					   GOTO     k		  -
; Branch on No Carry	     BNC k	   BTFSS    3,0
;					   GOTO     k		  -
; Branch on Digit Carry      BDC k	   BTFSC    3,1
;					   GOTO     k		  -
; Branch on No Digit Carry   BNDC k	   BTFSS    3,1
;					   GOTO     k		  -
; Branch on Zero	     BZ k	   BTFSC    3,2
;					   GOTO     k		  -
; Branch on Non Zero	     BNZ k	   BTFSS    3,2
;					   GOTO     k		  -
; Call across page boundary  LCALL k	   BCF 3,5 or BSF 3,5
;					   BCF 3,6 or BSF 3,6
;					   CALL     k
;
;*******************************************************************************
;
; Usefull Macros
;
__DAvail  set	H'0E'                      ; first RAM location
__RAMTop  set	H'4F'                      ; last RAM location

_TempInt  equ	H'0C'                      ; temporary reg in interrupts
_Temp	  equ	H'0D'                      ; general temp reg



clrps	  macro 			   ; clear prescaler
	    movf   TMR0, F
	  endm

FCom	  macro Flag			   ; complement flag
	    xorwf  STATUS, Flag
	  endm

iDisable  macro 			   ; disable interrupts
	    bcf   INTMASK, GIE
	  endm

iEnable   macro 			   ; enable interrupts
	    bsf   INTMASK, GIE
	  endm

ret	  macro 			   ; return
	    return
	  endm

jmp	  macro Adr			   ; goto
	    goto   Adr
	  endm

addfw	  macro Fx			   ; (W)+(Fx)->(W)
	    addwf  Fx, W
	  endm

andfw	  macro Fx			   ; (W) & (Fx)->(W)
	    andwf  Fx, W
	  endm

comfw	  macro Fx			   ; ~(Fx)->(W)
	    comf   Fx, W
	  endm

comw	  macro 			   ; ~(W)->(W)
	    xorlw  H'FF'
	  endm

decfw	  macro Fx			   ; (--Fx)->(W)
	    decf   Fx, W
	  endm

incfw	  macro Fx			   ; (++Fx)->(W)
	    incf   Fx, W
	  endm

orfw	  macro Fx			   ; (W)|(Fx)->(W)
	    iorwf  Fx, W
	  endm

orwf	  macro Fx			   ; (W)|(Fx)->(Fx)
	    iorwf  Fx
	  endm

rlfw	  macro Fx			   ; (F) rol ->(W)
	    rlf    Fx, W
	  endm

rrfw	  macro Fx			   ; (F) ror ->(W)
	    rrf    Fx, W
	  endm

tstfw	  macro Fx			   ; (F) -> (W)
	    movfw  Fx
	  endm

subfw	  macro Fx			   ; (Fx)-(W)->(W)
	    subwf  Fx, W
	  endm

swapfw	  macro Fx			   ; (Fx) swap ->(W)
	    swapf  Fx, W
	  endm

xorfw	  macro Fx			   ; (Fx)^(W)->(W)
	    xorwf  Fx, W
	  endm

orlw	  macro K			   ; (W)|(K)->(W)
	    iorlw  K
	  endm

;;; org macro

;move literal to memory
movl	macro	mem,imm
		movlw	imm
		movwf	mem
	  endm

;move memory to memory
ld		macro	mem,src
		movfw	src
		movwf	mem
	  endm



SetVector macro Vector, Adr	   ; Set Vector <Vector>
	  local  Tmp
	  Tmp	 set $
	    org    Vector
	    goto   Adr
	    org    Tmp
	  endm

SetRPage  macro Num			   ; Set register page <Num>
	    if Num & 1
	      bsf  STATUS, RP0		   ; bank we are
	    else
	      bcf  STATUS, RP0
	    endif
	  endm

jz	  macro Adr			   ; Jump if Zero
	    btfsc  STATUS, Z
	    jmp    Adr
	  endm

jnz	  macro Adr			   ; Jump if Not Zero
	    btfss  STATUS, Z
	    jmp    Adr
	  endm

jc	  macro Adr			   ; Jump if Carry
	    btfsc  STATUS, C
	    jmp    Adr
	  endm

jnc	  macro Adr			   ; Jump if Not Carry
	    btfss  STATUS, C
	    jmp    Adr
	  endm

jbs	  macro Reg, Bit, Adr		   ; jump if bit set
	    btfsc  Reg, Bit
	    jmp    Adr
	  endm

jbc	  macro Reg, Bit, Adr		   ; jump if bit clear
	    btfss  Reg, Bit
	    jmp    Adr
	  endm

sbs	  macro Reg, Bit		   ; skip if bit set
	    btfss  Reg, Bit
	  endm

sbc	  macro Reg, Bit		   ; skip if bit clear
	    btfsc  Reg, Bit
	  endm

lnop	  macro 			   ; long nop (2 cycles)
	    jmp  $+1
	  endm

SwitchW   macro 			   ; jump to offset specified by W
	  local TableStart

	    clrf   PCLATH
	  if ((HIGH TableStart) & 1) == 1  ; check for bits 8, 9
	    bsf    PCLATH, 0		   ; of table address (this gives
	  else
	    bcf    PCLATH, 0
	  endif 			   ; 1K of address space)
	  if ((HIGH TableStart) & 2) == 2  ; and set corresponding bits
	    bsf    PCLATH, 1		   ; in PCLATH
	  else
	    bcf    PCLATH, 1
	  endif
	    addlw  LOW (TableStart)	   ; calculate dest address LO
	    skpnc			   ; adjust PCLATH if necessary
	    incf   PCLATH		   ; if LO+ofs > 255
	    movwf  PC			   ; branch to desired offset
TableStart:				   ; here begins the table
	  endm


New	  macro Name
	  NoExpand
	    if __DAvail > __RAMTop
	      Error "Out of available RAM / File registers"
	    endif
Name  equ  __DAvail
__DAvail = __DAvail + 1
	  Expand
	  endm

Alloc	  macro Size
	  NoExpand
	    __DAvail = __DAvail + Size
	    if __DAvail > __RAMTop
	      Error "Out of available RAM / File registers"
	    endif
	  Expand
	  endm

EERead	  macro EELoc
	    if EELoc != W
	      movlw  EELoc
	    endif
	    movwf  EEADR
	    SetRPage 1
	    bsf    EECON1, EERD
	    SetRPage 0
	  endm

EEWrite   macro EELoc
	    if EELoc != W
	      movlw  EELoc
	    endif
	    movwf  EEADR
	    SetRPage 1
	    movlw  H'55'
	    movwf  EECON2
	    movlw  H'AA'
	    movwf  EECON2
	    bsf    EECON1, EEWR
	    SetRPage 0
	  endm

       List
