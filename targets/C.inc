;**** **** **** **** ****
;
; Bluejay digital ESC firmware for controlling brushless motors in multirotors
;
; Copyright 2020 Mathias Rasmussen
; Copyright 2011, 2012 Steffen Skaug
;
; This file is part of Bluejay.
;
; Bluejay is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Bluejay is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Bluejay.  If not, see <http://www.gnu.org/licenses/>.
;
;**** **** **** **** ****
;
; Hardware definition file "C"
; Ac Ap MC MB MA CC X  RC    X  X  X  X  Cc Cp Bc Bp
;
;**** **** **** **** ****

TEMP_LIMIT			EQU 	49	; Temperature measurement ADC value for which main motor power is limited at 80degC (low byte, assuming high byte is 1)
TEMP_LIMIT_STEP		EQU	9	; Temperature measurement ADC value increment for another 10degC

;**** **** **** **** ****
; Bootloader definitions
;**** **** **** **** ****
RTX_PORT		  		EQU 	P0		; Receive/Transmit port
RTX_MDOUT				EQU	P0MDOUT	; Set to 1 for PUSHPULL
RTX_MDIN				EQU	P0MDIN	; Set to 1 for DIGITAL
RTX_SKIP				EQU	P0SKIP	; Set to 1 for SKIP
RTX_PIN				EQU 	0		; RTX pin

SIGNATURE_001			EQU	0E8h		; Device signature
IF MCU_48MHZ == 0
SIGNATURE_002			EQU	0B1h
ELSE
SIGNATURE_002			EQU	0B2h
ENDIF

;*********************
; PORT 0 definitions *
;*********************
AcomFET		EQU	7	;o
ApwmFET		EQU	6	;o
Mux_C		EQU	5	;i
Mux_B		EQU	4	;i
Mux_A		EQU	3	;i
Comp_Com		EQU	2	;i
;			EQU	1	;i
Rcp_In		EQU	0	;i

P0_DIGITAL	EQU	NOT((1 SHL Mux_A)+(1 SHL Mux_B)+(1 SHL Mux_C)+(1 SHL Comp_Com))
P0_INIT		EQU	NOT((1 SHL ApwmFET)+(1 SHL AcomFET))
P0_PUSHPULL	EQU	(1 SHL ApwmFET)+(1 SHL AcomFET)
P0_SKIP		EQU	0FFh

Set_Pwm_Polarity MACRO
	mov	PCA0POL, #02h				; Damping inverted, pwm noninverted
ENDM

IF FETON_DELAY == 0
	PCA0_POWER_MODULE	EQU	PCA0CPM0
	PCA0_POWER_L		EQU	PCA0CPL0
	PCA0_POWER_H		EQU	PCA0CPH0

	PCA0_DAMP_MODULE	EQU	PCA0CPM1
	PCA0_DAMP_L		EQU	PCA0CPL1
	PCA0_DAMP_H		EQU	PCA0CPH1
ELSE
	PCA0_POWER_MODULE	EQU	PCA0CPM0
	PCA0_POWER_L		EQU	PCA0CPL0
	PCA0_POWER_H		EQU	PCA0CPH0

	PCA0_DAMP_MODULE	EQU	PCA0CPM1
	PCA0_DAMP_L		EQU	PCA0CPL1
	PCA0_DAMP_H		EQU	PCA0CPH1
ENDIF


;*********************
; PORT 1 definitions *
;*********************
;			EQU	7	;i
;			EQU	6	;i
;			EQU	5	;i
;			EQU	4	;i
CcomFET		EQU	3	;o
CpwmFET		EQU	2	;o
BcomFET		EQU	1	;o
BpwmFET		EQU	0	;o

P1_DIGITAL	EQU	(1 SHL BpwmFET)+(1 SHL CpwmFET)+(1 SHL BcomFET)+(1 SHL CcomFET)
P1_INIT		EQU 	00h
P1_PUSHPULL	EQU	(1 SHL BpwmFET)+(1 SHL CpwmFET)+(1 SHL BcomFET)+(1 SHL CcomFET)
P1_SKIP		EQU	0Fh

ApwmFET_on MACRO
	setb	P0.ApwmFET
IF FETON_DELAY == 0
	setb	P0.AcomFET
ENDIF
ENDM
ApwmFET_off MACRO
IF FETON_DELAY != 0
	clr	P0.ApwmFET
ELSE
	clr	P0.AcomFET
ENDIF
ENDM
BpwmFET_on MACRO
	setb	P1.BpwmFET
IF FETON_DELAY == 0
	setb	P1.BcomFET
ENDIF
ENDM
BpwmFET_off MACRO
IF FETON_DELAY != 0
	clr	P1.BpwmFET
ELSE
	clr	P1.BcomFET
ENDIF
ENDM
CpwmFET_on MACRO
	setb	P1.CpwmFET
IF FETON_DELAY == 0
	setb	P1.CcomFET
ENDIF
ENDM
CpwmFET_off MACRO
IF FETON_DELAY != 0
	clr	P1.CpwmFET
ELSE
	clr	P1.CcomFET
ENDIF
ENDM
All_pwmFETs_Off MACRO
IF FETON_DELAY != 0
	clr	P0.ApwmFET
	clr	P1.BpwmFET
	clr	P1.CpwmFET
ELSE
	clr	P0.AcomFET
	clr	P1.BcomFET
	clr	P1.CcomFET
ENDIF
ENDM

AcomFET_on MACRO
IF FETON_DELAY == 0
	clr	P0.ApwmFET
ENDIF
	setb	P0.AcomFET
ENDM
AcomFET_off MACRO
	clr	P0.AcomFET
ENDM
BcomFET_on MACRO
IF FETON_DELAY == 0
	clr	P1.BpwmFET
ENDIF
	setb	P1.BcomFET
ENDM
BcomFET_off MACRO
	clr	P1.BcomFET
ENDM
CcomFET_on MACRO
IF FETON_DELAY == 0
	clr	P1.CpwmFET
ENDIF
	setb	P1.CcomFET
ENDM
CcomFET_off MACRO
	clr	P1.CcomFET
ENDM
All_comFETs_Off MACRO
	clr	P0.AcomFET
	clr	P1.BcomFET
	clr	P1.CcomFET
ENDM

Set_Pwm_A MACRO
IF FETON_DELAY == 0
	setb	P0.AcomFET
	mov	P0SKIP, #0BFh
	mov	P1SKIP, #0Fh
ELSE
	mov	P0SKIP, #03Fh
	mov	P1SKIP, #0Fh
ENDIF
ENDM
Set_Pwm_B MACRO
IF FETON_DELAY == 0
	setb	P1.BcomFET
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #0Eh
ELSE
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #0Ch
ENDIF
ENDM
Set_Pwm_C MACRO
IF FETON_DELAY == 0
	setb	P1.CcomFET
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #0Bh
ELSE
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #03h
ENDIF
ENDM
Set_Pwms_Off MACRO
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #0Fh
ENDM

Set_Comp_Phase_A MACRO
	mov	CMP0MX, #32h	; Set comparator multiplexer to phase A
ENDM
Set_Comp_Phase_B MACRO
	mov	CMP0MX, #42h	; Set comparator multiplexer to phase B
ENDM
Set_Comp_Phase_C MACRO
	mov	CMP0MX, #52h	; Set comparator multiplexer to phase C
ENDM
Read_Comp_Out MACRO
	mov	A, CMP0CN0	; Read comparator output
ENDM


;*********************
; PORT 2 definitions *
;*********************
DebugPin		EQU	0	;o

P2_PUSHPULL	EQU	(1 SHL DebugPin)


;**********************
; MCU specific macros *
;**********************
Initialize_Xbar MACRO
	mov	XBR2, #40h	; Xbar enabled
	mov	XBR1, #02h	; CEX0 and CEX1 routed to pins
ENDM
Initialize_Comparator MACRO
	mov	CMP0CN0, #80h	; Comparator enabled, no hysteresis
	mov	CMP0MD, #00h	; Comparator response time 100ns
ENDM
Initialize_Adc MACRO
	mov	REF0CN, #0Ch	; Set vdd (3.3V) as reference. Enable temp sensor and bias
IF MCU_48MHZ == 0
	mov	ADC0CF, #59h	; ADC clock 2MHz, PGA gain 1
ELSE
	mov	ADC0CF, #0B9h	; ADC clock 2MHz, PGA gain 1
ENDIF
	mov	ADC0MX, #10h	; Select temp sensor input
	mov	ADC0CN0, #80h	; ADC enabled
	mov	ADC0CN1, #01h	; Common mode buffer enabled
ENDM
Start_Adc MACRO
	mov	ADC0CN0, #90h	; ADC start
ENDM
Read_Adc_Result MACRO
	mov	Temp1, ADC0L
	mov	Temp2, ADC0H
ENDM
Stop_Adc MACRO
ENDM
Set_RPM_Out MACRO
ENDM
Clear_RPM_Out MACRO
ENDM
Set_LED_0 MACRO
ENDM
Clear_LED_0 MACRO
ENDM
Set_LED_1 MACRO
ENDM
Clear_LED_1 MACRO
ENDM
Set_LED_2 MACRO
ENDM
Clear_LED_2 MACRO
ENDM
Set_LED_3 MACRO
ENDM
Clear_LED_3 MACRO
ENDM
