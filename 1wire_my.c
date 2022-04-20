#asm
_w1_init_my:
		clr  r30
		cbi  __w1_port,__w1_bit
		;cli
		sbi  __w1_port-1,__w1_bit
		__DELAY_USW 0x20F
		cbi  __w1_port-1,__w1_bit
		__DELAY_USB 0x25
		sbis __w1_port-2,__w1_bit
		ret
		;sei
		__DELAY_USB 0xCB
		sbis __w1_port-2,__w1_bit
		;sei
		inc  r30
		__DELAY_USW 0x30C
		ret
	LDI  R30,LOW(0)
	RET
_w1_read_bit_my:
		cli
		sbi  __w1_port-1,__w1_bit
		__DELAY_USB 0x3
		cbi  __w1_port-1,__w1_bit
		__DELAY_USB 0x10
		clc
		sbic __w1_port-2,__w1_bit
		sec
		ror  r30
		__DELAY_USB 0xA0
		sei
		ret
	LDI  R30,LOW(0)
	RET
_w1_read_my:
		ldi  r22,8
		__w1_read0:
		rcall _w1_read_bit_my
		dec  r22
		brne __w1_read0
		ret
	LDI  R30,LOW(0)
	RET
_w1_write_my:
		ldi  r22,8
		ld   r23,y+
		clr  r30
	__w1_write2:
		rcall __w1_write_bit_my
		brtc __w1_write3
		ror  r23
		dec  r22
		brne __w1_write2
		inc  r30
	__w1_write3:
		ret
		
	__w1_write_bit_my:
		clt
		cli
		sbi  __w1_port-1,__w1_bit
		__DELAY_USB 0x3
		sbrc r23,0
		cbi  __w1_port-1,__w1_bit
		__DELAY_USB 0x23
		sbic __w1_port-2,__w1_bit
		rjmp __w1_write_bit2
		sbrs r23,0
		rjmp __w1_write_bit3
		ret
	__w1_write_bit2:
		sbrs r23,0
		ret
	__w1_write_bit3:
		__DELAY_USB 0xB8
		cbi  __w1_port-1,__w1_bit
		__DELAY_USB 0xA
		sei
		set
		ret
	ADIW R28,1
	RET
#endasm