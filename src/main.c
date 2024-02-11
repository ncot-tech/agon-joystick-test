/* vim: set tabstop=4:softtabstop=0:shiftSC_WIDTH=4:noexpandtab */
/*
 * Title:		Agon Light Joystick Test
 * Author:		James Grimwood
 * Created:		4/2/2024
 *
 */

#include <string.h>
#include "uart.h"
#include <agon/vdp_vdu.h>
#include <stdio.h>
#include <mos_api.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 
#define CHECK_BIT(val, pos) ((val) & (1 << pos))

#define PCDR 0x9E
#define PDDR 0xA2

// Change these to suit the bits that get toggled in the two ports above
// They're all active low, so test for them being equal to zero, not 1
// This is port D
#define JOYA_U 7
// These are port C
#define JOYA_D 3
#define JOYA_L 5
#define JOYA_R 7
#define JOYA_F 6

int main()
{

	static volatile SYSVAR *sv;
	sv = vdp_vdu_init();
	
	xUART uart = {
		.baudRate = 115200,
		.dataBits = 8,                                      //!< The number of databits per character to be used.
		.stopBits = 1,                                      //!< The number of stopbits to be used.
		.parity = 0,                                        //!< The parity bit option to be used.
		.hwFlowControl = 0,
	};

	init_UART1();

	uint8_t ret = open_UART1(&uart);
	printf ("mos_uopen returned %d\n", ret);
	uart1_puts("Hello World!\n");

	uint8_t portc, portd;
	char buf[80];

	uint8_t current_joya_state[5] = {0,0,0,0,0}, prev_joya_state[5] = {0,0,0,0,0};

	while (1) {

		for (int i = 0; i < 5; i++) prev_joya_state[i] = current_joya_state[i];

		portc = get_port(PCDR);
		portd = get_port(PDDR);
		
		current_joya_state[0] = CHECK_BIT(portd,JOYA_U);
		current_joya_state[1] = CHECK_BIT(portc,JOYA_D);
		current_joya_state[2] = CHECK_BIT(portc,JOYA_L);
		current_joya_state[3] = CHECK_BIT(portc,JOYA_R);
		current_joya_state[4] = CHECK_BIT(portc,JOYA_F);

		putch(31); putch(0); putch(0);
		//vdp_clear_screen();
		printf ("Agon Light Joystick Test Program\r\n");
		printf ("James Grimwood - https://ncot.uk 2024\r\n");
		printf ("Wiggle your joysticks!\r\n");
		printf ("Avoid connecting them to GPIO PC0 and PC1 as the UART is also active\r\n");
		printf ("Plug in a serial terminal to GPIO PC0, PC1 set at 115200, 8N1\r\n");
		snprintf(buf, 80, "PORT C "BYTE_TO_BINARY_PATTERN" - PORT D "BYTE_TO_BINARY_PATTERN"\r\n", 
		   BYTE_TO_BINARY(portc), BYTE_TO_BINARY(portd));
		uart1_puts(buf);
		printf("%s", buf);
		snprintf(buf,80, "Joystick A Status [%c,%c,%c,%c,%c]\n\r",
		   ((current_joya_state[0]) == 0 ? ('U') : ('u')),
		   ((current_joya_state[1]) == 0 ? ('D') : ('d')),
		   ((current_joya_state[2]) == 0 ? ('L') : ('l')),
		   ((current_joya_state[3]) == 0 ? ('R') : ('r')),
		   ((current_joya_state[4]) == 0 ? ('F') : ('f')));
		uart1_puts(buf);
		printf("%s", buf);
		for (int i = 0; i < 5; i++)
			printf ("%d ", current_joya_state[i]);
		printf ("\n");

		waitvblank();
	}

	return 0;
}
