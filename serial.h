/********************************************************************************   
   serial.h seriell överföring av text, heltal och flyttal till en seriell terminal
   via USART.
   Universal Synchronous Asynchronous Receiver/Transmitter.
 ********************************************************************************/ 

#ifndef SERIAL_H_
#define SERIAL_H_

#include "misc.h"

/* aktiverar seriell överföring baudrate i kilo bits per sekund*/
void serial_init(const uint32_t baud_rate_kbps);

/* skriver ut angivet textstycke till seriell terminal 
   s: pekare till textstycke som ska skrivas ut */ 
void serial_print_string(const char* s);

/* heltal */
void serial_print_integer(const uint32_t num);

/* osignerad int */
void serial_print_unsigned(const uint32_t num);

/* decimal tal */
void serial_print_double(const double num);

/* tecken */
void serial_print_char(const char c);

/********************************************************************************  
 det gamla hederliga vagnreturstecknet \r = längst till vänster på nästa rad, 
 nyradstecknet \n 
 *******************************************************************************/
void serial_print_new_line(void);

#endif /* SERIAL_H_ */