#ifndef TMP36_H_
#define TMP36_H_

#include "misc.h"
#include "adc.h"
#include "serial.h"

/* strukt f�r TMP36 */
struct tmp36
{
	struct adc input;
};

/* initierar TMP36 */
static inline void tmp36_init(struct tmp36* self, const uint8_t pin)
{
	adc_init(&self->input, pin);
	serial_init(9600);
	return;
}
/********************************************************************************  
(adc_read / 1023.0) * 5.0V ger oss m�tv�rdet som sedan omvandlas genom att 
(multipliceras med 100) och (subtraheras med 50 som �r startv�rdet), detta ger ut
ett svar i celsius. ( startv�rdet kr�ver viss kalibrerig f�r att f� korrekt 
temperatur)  

for satsen tillsammans med temporary och average ger ett medelv�rde av 100 
m�tv�rden. 
*********************************************************************************/
static inline double tmp36_get_temperature(const struct tmp36* self)
{
	
	double temporary = 0;
	
	for (int i = 0; i < 100; i++)
	{
		const double input_voltage = adc_read(&self->input) / 1023.0 * 5.014;
		temporary += (100 * input_voltage - 55);
	}
	
	double average = temporary / 100.00;
	return average;
	
	/*
	const double input_voltage = adc_read(&self->input) / 1023.0 * 5;
	return 100 * input_voltage - 50;
	*/
	
}

/* skriver ut temperaturen i seriell monitor */
static inline void tmp36_print_temperature(const struct tmp36* self)
{
	serial_print_string("Temperature: ");
	serial_print_double(tmp36_get_temperature(self));
	serial_print_string(" degrees Celsius ");
	
	return;
}

#endif