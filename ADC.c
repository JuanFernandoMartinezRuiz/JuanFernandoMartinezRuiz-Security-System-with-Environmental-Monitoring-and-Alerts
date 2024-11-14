#include <xc.h>
#include "ADC.h"
#include <math.h>

#define _XTAL_FREQ 8000000 

void adc_begin(void) {
    ADCON1bits.ADFM = 1; // Justificaci�n a la derecha
    ADCON1bits.VCFG0 = 0; // Referencia de voltaje 0V a 5V
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 0b01; // Tiempo de conversi�n Fosc/8
}

int adc_conversion(int channel) {
    ADCON0bits.CHS = (0x0F & channel); // Seleccionar canal
    ADCON0bits.ADON = 1; // Encender el m�dulo ADC
    __delay_us(30); // Tiempo de adquisici�n
    ADCON0bits.GO = 1; // Iniciar conversi�n
    while (ADCON0bits.GO); // Esperar que termine
    ADCON0bits.ADON = 0; // Apagar el ADC
    return ((ADRESH << 8) | ADRESL); // Retornar valor de 10 bits
}

int leer_temperatura(void) {
    int raw_value = adc_conversion(0); 
    
    
    long beta = 3975;
    
    
    long a = 1023 - raw_value; 
    

    float TempC = (float)(beta / (log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0);
    

    return (int) TempC;
}

int leer_fotoresistor(void) {
    return adc_conversion(3);
}
