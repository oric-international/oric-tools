        org $100

; 22050 baud tape loading routine
; (~12600 bps)
; F.Frances 1999-2012

shifter equ $2F
ptr     equ $33
crc	equ $83

IRB	equ $0300
T2C	equ $0308
T2L	equ $0308
PCR	equ $030C
IFR	equ $030D
IER	equ $030E

	jsr $E76A       ; init VIA (disable T1 interrupt) and set flag I
        lda #LOW(irq)   ; install IRQ handler
	sta $0245
        lda #HIGH(irq)
	sta $0246
        lda #$90
        sta IER         ; allow interrupt on CB1 active edge detection
        jsr $E59B       ; display 'loading <name>'
next_page
	jsr $E6C9	; load MSB of page address
	beq end_load
        sta ptr+1
        jsr $E6C9	; load LSB of page address
        sta ptr
        jsr $E6C9	; load starting offset in page
        tay
        jsr load_page
	jmp next_page
end_load        	
	lda #$22	; restore standard IRQ handler
	sta $0245
	lda #$EE
	sta $0246
	jsr $E93D	; restore VIA settings and clear flag I
	jsr $E651	; displays ERROR FOUND message if parity error detected
	lda $02B1	; is there a parity error in the mini-headers ?
	bne return	; then end loader without starting loaded program
        jmp $E8D6       ; handle auto-exec
        
load_page
        bit IRB         ; clear CB1 flag
	lda #$10
wait    bit IFR         ; wait next positive edge
	beq wait
        ldx #0          ; switch to negative edge detection
	stx PCR		
        bit IRB         ; clear CB1 flag
        dex
        stx T2L		; init T2L
        stx shifter	; shifter will roll without storing bytes until a 0 comes out        
	stx T2C+1	; restart T2 (26 to 33 µs after the edge)
	cli		; allow interrupts
        bne *           ; unconditional, wait for an interrupt (CB1 active edge)

irq			; irq handler modifies registers
        lda PCR         ; reverse active edge detection
	eor #$10
	sta PCR
        bit IRB         ; clear CB1 flag
        ldx T2C         ; load timer : 26 to 28 µs have passed since the active edge
        stx T2L+1       ; and restart a countdown (30 to 32 µs after the edge)
	and #$10
	bne high_level
low_level
	lda shifter
	cpx #110
	bcs short_low_level
long_low_level
	cpx #68		; C=0 if very long level
	bne shift_bits	; unconditional
short_low_level
	cpx #154	; C=1 if very short level
	bne rol_bits	; unconditional
high_level
	lda shifter
	cpx #99
	bcs short_high_level
long_high_level
	cpx #55		; C=0 if very long level
shift_bits
	rol a		; shift 00 if length=10
	asl a		;    or 10 if length=8
	bcc byte_complete
	bcs end_irq
short_high_level
	cpx #141	; C=1 if very short level
rol_bits
	rol a		; shift 01 if length=6
	rol a		;    or 11 if length=4
	bcs end_irq
byte_complete
	sta (ptr),y
	iny
	beq end_page
	lda #$FE	; prepare shifter so that a 0 comes out after 4 interrupts
end_irq
	sta shifter
	rti		; flag I is reset on exit
end_page
			; here flag I remains set on exit
			; end of page : switch back to positive edge detection
	lda #$10
	sta PCR
	pla		; pull flag register
	pla		; pull interrupt address to exit loadpage routine
	pla
return
	rts		


