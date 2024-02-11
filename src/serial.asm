PORT		:=	0xD0			; UART1
				
REG_RBR		:=	PORT+0		; Receive buffer
REG_THR		:=	PORT+0		; Transmitter holding
REG_DLL		EQU	PORT+0		; Divisor latch low
REG_IER		EQU	PORT+1		; Interrupt enable
REG_DLH		EQU	PORT+1		; Divisor latch high
REG_IIR		EQU	PORT+2		; Interrupt identification
REG_FCT		EQU	PORT+2;		; Flow control
REG_LCR		EQU	PORT+3		; Line control
REG_MCR		EQU	PORT+4		; Modem control
REG_LSR		EQU	PORT+5		; Line status
REG_MSR		EQU	PORT+6		; Modem status

REG_SCR		EQU PORT+7		; Scratch

TX_WAIT		EQU	16384 		; Count before a TX times out

UART_LSR_ERR	:=  0x80			; Error
UART_LSR_ETX	:=  0x40			; Transmit empty
UART_LSR_ETH	:=	0x20			; Transmit holding register empty
UART_LSR_RDY	:=	0x01			; Data ready


			ASSUME	ADL = 1
			SECTION .TEXT
			
; Write a byte to UART1
; A: Data byte to write
; Returns:
; A = 0 if timed out
; A = 1 if written
			PUBLIC _uart1_putch
_uart1_putch:
			PUSH	BC			; Stack BC
			PUSH	AF 			; Stack AF
			LD		BC,TX_WAIT	; Set CB to the transmit timeout
loop:		IN0		A,(REG_LSR)	; Get the line status register
			AND 	UART_LSR_ETX; Check for TX empty
			JR		NZ, tx		; If set, then TX is empty, goto transmit
			DEC		BC
			LD		A, B
			OR		C
			JR		NZ, loop
			POP		AF			; We've timed out at this point so
			POP		BC			; Restore the stack
			XOR		A,A			; Return timeout
			RET	
tx:			POP		AF			; Good to send at this point, so
			OUT0	(REG_THR),A	; Write the character to the UART transmit buffer
			POP		BC			; Restore BC
			LD		A,1			; Return succes, also works when A was 0 originally
			RET 

; Blocking read a byte from UART1
; Returns:
; A = byte read
			PUBLIC _uart1_getch
_uart1_getch:
			LD		A,(uart1_received)
			OR		A
			JR		Z, _uart1_getch
			DI		; atomic acknowledge byte read
			XOR		A,A
			LD		(uart1_received),A
			LD		A,(uart1_buffer)
			EI
			RET
	
			
uart1_buffer:		
DB	1	; 64 byte receive buffer
uart1_received:		
DB	1	; boolean
