/* 
 * File:   eusart_lib.h
 * Author: Christian Cruz 21759
 *
 * Created on July 18, 2023, 4:06 PM
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef EUSART_LIB_H
#define	EUSART_LIB_H

void EUSART_Init(uint16_t baudrate);
void EUSART_RX_config (uint16_t baudrate);
void EUSART_TX_config (uint16_t baudrate);
void EUSART_write_char (char c);
void EUSART_write_string (char c[]);
char EUSART_read_char ();

#endif	/* ADC_CONFIG_LIB_H */
