
#include <xc.h>
#include "SENSOR.h"

#define _XTAL_FREQ 8000000 

void sensor_init(void) {
    ANSELbits.ANS1 = 0;  
    ANSELbits.ANS2 = 0;  
    TRISA1 = 1;          
    TRISA2 = 1;          
}

// Función para leer el sensor Hall
int leer_sensor_hall(void) {
    __delay_us(30);       // Espera para asegurar la lectura
    return PORTAbits.RA1; // Devuelve el valor del sensor Hall (1 o 0)
}

// Función para leer el sensor de obstáculos
int leer_sensor_obstaculos(void) {
    __delay_us(30);         // Espera para asegurar la lectura
    return PORTAbits.RA2;   // Devuelve el valor del sensor de obstáculos (1 o 0)
}

