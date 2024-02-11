#ifndef __UART_H_
#define __UART_H_

#include <stdint.h>

/*!
 * \brief The settings required to configure the UART driver.
 *
 * This structure will contain all the information required to
 * configure the UART device. This structure is used for opening
 * (initializing) the UART device as well as for re-configuring
 * the UART device.
 */
typedef struct
{
   char uartMode ;				//!< The mode in which uart operated (INTERRUPT or POLL).
   uint24_t baudRate ;				//!< The baudrate to be used.
   unsigned char dataBits ;				//!< The number of databits per character to be used.
   unsigned char stopBits ;				//!< The number of stopbits to be used.
   unsigned char parity ;				//!< The parity bit option to be used.
   unsigned char fifoTriggerLevel ;		//!< The receive FIFO interrupt trigger level to be used.
   unsigned char hwFlowControl ;		//!< The hardware flow control to be used.
   unsigned char swFlowControl ;		//!< The software flow control to be used (no plan to implement this in ZSL 1.0).

} xUART ;

void init_UART1();
extern void uart1_putch(char c);
extern char uart1_getch();
uint8_t open_UART1(xUART *pUART); 
uint8_t get_port(uint8_t port);

void uart1_puts(char *str);
#endif
