#include "serial.h"

/********************************************************************************
  serial_init: aktiverar seriell överföring för transmission av data 
               med angiven baud rate (överföringshastighet) i kbps.
			   Vi ställer in asynkron överföring med åtta bitar i taget 
			   med en stoppbit. om seriell överföring redan har 
			   aktiverats så sker ingen ny initiering.

  1. Biten TXEN0 ettställs för aktivering av USART
  
  2. Åtta bitar skickas i taget vi ettställning av
     bitar UCSR0C
	 
  3. Baud rate
     Baudrate sätt via formel:
     UBRR0 = F_CPU/(16 * baud_rate) - 1
    
  4. Vagnreturstecken '\r'	 	 
*********************************************************************************/
void serial_init(const uint32_t baud_rate_kbps)
{
	static bool serial_initialized = false;
	if (serial_initialized) return;
	
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = (uint16_t)(F_CPU / (16.0 * baud_rate_kbps) - 1 + 0.5);
	UDR0 = '\r';
	
	serial_initialized = true;
	return;
}

/************************************************************************
   s:   skriver ut angivet textstycke till seriell terminal 
        pekare till textstycke som ska skrivas ut 
   
        for (const char* i = s; *i != '\0'; ++i)
   
        sålänge det tecknet som i pekar 
        på ej är lika med ett nolltecken
        inkrimenterar vi med ett 
   
              >>>>   *i != '\0'    <<<<
	                ekvivalent med
                  >>>>   *i    <<<<
 *************************************************************************/ 
void serial_print_string(const char* s)
{
	for (const char* i = s; *i; ++i)
	{
		serial_print_char(*i);
		
		if (*i == '\n')
		{
			serial_print_char('\r');
		}
	}
	return;
}

/************************************************************************* 
serial_print_integer: Skriver ut ett signerat heltal till en seriell terminal
                      
					  1. Deklarerar och initierar an ny sträng som rymmer 
					     20 tecken. Strängen är tom vid start, vi tilldelar
						 därmed ett nolltecken vid start.
						 
					  2. Vi omvandlar heltalet till text och lagrar i
					     strängen via anrop av funktionen sprintf från 
						 stdio.h. Vi använder formatspecifierare %ld, då
						 talet utgör ett 32-bitars signerat heltal.
						 
					  3. Vi skriver ut talet som text via anrop av funktionen
					     serial_print_string.
						 
					  - num: Heltalet som ska skrivas ut. 	 	 	    

*************************************************************************/
void serial_print_integer(const uint32_t num)
{
	char s[20] = { '\0' };
	sprintf(s, "%ld", num);
	serial_print_string(s);
	return;
}

/* osignerad int */
void serial_print_unsigned(const uint32_t num)
{
	char s[20] = { '\0' };
	sprintf(s, "%lu", num);
	serial_print_string(s);
	return;
}


/********************************************************************************  
 serial_print_double: Skriver ut ett flyttal med upp till två decimaler till en seriell
                      terminal.
					   
					  1. Vi deklarerar och initierar en tom sträng som rymmer 
					     40 tecken.
						 
					  2. Heltalsdelen tas ut och lagras i en konstant genom
					     att flyttalsdelen av flyttalet trunkeras.
						 
					  3. Decimaldelen extraheras och avrundas till två decimaler.
					     Vi tar i åtanke om heltaldelen är postitiv eller negativ. 
						  	  
					     	 
					  - num: Flyttalet som ska skrivas ut
					  
if else satsen kan även skrivas:

>>>>> int32_t decimal = integer >= 0 ? (int32_t)((num -integer) * 100 + 0.5) 
>>>>> : (int32_t)((integer - num) * 100 + 0.5)					  
					  	    
 *******************************************************************************/
void serial_print_double(const double num)
{
	char s[40] = { '\0' };
	const int32_t integer = (int32_t)num;
	int32_t decimal = 0;
	
	if (integer >= 0)
	{
		decimal = (int32_t)((num - integer) * 100 + 0.5);
	}
	else
	{
		decimal = (int32_t)((integer - num) * 100 + 0.5);
	}
	
	sprintf(s, "%ld.%ld", integer, decimal);
	serial_print_string(s);
	return;	
}

/********************************************************************************  
 serial_print_char: Skriver ut tecken till en seriell terminal. Innan 
                    angivet tecken skrivs ut sker väntan på att eventuellt
					föregående tecken skickas.
 
                    1. Vi läser av biten UDRE0 (USART Data Register Empty 0)
					   i kontroll- och statusregistret USCR0A (USART control and
					   Status register A) för att kontrollera ifall buffern UDR0
					   (USART Data Register 0) är tom och därmed redo att ta emot
					   nästa tecken. Så länge UDRE0 är lika med 0 har föregående
					   tecken inte skickats och då väntar vi via en while sats.
					   
					 2. Så fort buffern blir tom (föregående tecken har skickats)  
					    så ettställs biten UDRE0 och då avslutas föregående while
						sats. Vi lägger då det nya tecknet i buffern och avslutar
						sedan funktionen.
						
					 - c: tecknet som ska skrivas ut. 	 
 *******************************************************************************/
void serial_print_char(const char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
	return;
}

/********************************************************************************  
 det gamla hederliga vagnreturstecken \r = längst till vänster på nästa rad, 
 nyradstecknet \n 
 denna funktion är så liten att inline fungerar bra och man kan köra funktionen i 
 header.h direkt 
 *******************************************************************************/
void serial_print_new_line(void)
{
	serial_print_string("\r"); 
	return;
}