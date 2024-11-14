#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "CONFIG.h"
#include "LCD.h"
#include "KEYPAD.h"
#include "ADC.h"
#include <math.h>
#include "SENSOR.h"

// Definimos los estados posibles
enum State {
    Inicio,
    Seguridad,
    Bloqueado,
    Monitoreo_Ambiental,
    Monitor_Eventos,
    Alarma,
    Alerta
};

// Definimos las variables de estado
enum State Estado = Inicio;

const char password[5] = {'1','2','3','4',0};
char pass_user[5];
unsigned char idx = 0;
unsigned char intentos = 3;
volatile unsigned int ms_counter = 0;   // Contador en milisegundos
unsigned int timeout_ms = 0;            // Tiempo de espera en milisegundos

// Variables de sensores
int tempCelsius;
int light_intensity;
int sensor_hall;
int value_obstaculo;

// Definimos los pines del LED RGB en el puerto E
#define LED_ROJO   PORTEbits.RE0
#define LED_VERDE  PORTEbits.RE1
#define LED_AZUL   PORTEbits.RE2

// Prototipos de funciones
void timer_init(void);
void __interrupt() ISR(void);
void function_inicio(void);
void function_seguridad(void);
void function_bloqueado(void);
void function_monitoreo_ambiental(void);
void function_monitor_eventos(void);
void function_alarma(void);
void function_alerta(void);

void main() {
    OSCCON = 0x71;  
    TRISD = 0x00;   
    TRISA = 0xFF;   
    ANSEL = 0x0F;   
    TRISE = 0x00;

    LCD_Init();
    keypad_init();
    adc_begin();
    sensor_init();
    timer_init();

    while (1) {
        switch (Estado) {
            case Inicio:
                function_inicio();
                break;
            case Seguridad:
                function_seguridad();
                break;
            case Bloqueado:
                function_bloqueado();
                break;
            case Monitoreo_Ambiental:
                function_monitoreo_ambiental();
                break;
            case Monitor_Eventos:
                function_monitor_eventos();
                break;
            case Alarma:
                function_alarma();
                break;
            case Alerta:
                function_alerta();
                break;
        }
    }
}

/** 
 * Configura el Timer0 para contar en intervalos de milisegundos.
 */
void timer_init(void) {
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b111;
    TMR0 = 256 - (8000000 / 4 / 1000 / 256);
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.GIE = 1;
}

/**
 * Interrupción del Timer0 para incrementar el contador de milisegundos.
 */
void __interrupt() ISR(void) {
    if (INTCONbits.TMR0IF) {
        TMR0 = 256 - (8000000 / 4 / 1000 / 256);
        ms_counter++;
        INTCONbits.TMR0IF = 0;
    }
}

/**
 * Estado de Inicio: inicializa el sistema y configura el estado a Seguridad.
 */
void function_inicio(void) {
    LCD_Clear();
    LCD_String("Seguridad");
    LED_VERDE = 0;
    LED_ROJO = 0;
    LED_AZUL = 0;

    Estado = Seguridad;
    ms_counter = 0;
}

/**
 * Estado de Seguridad: permite al usuario ingresar la contraseña y verifica su validez.
 */
void function_seguridad(void) {
    char key = '0';
    idx = 0;

    if (intentos != 0) {
        LCD_String_xy(0, 0, "ENTER PASSWORD");
        LCD_Command(0xC0);

        timeout_ms = 100;
        do {
            key = keypad_getkey();
            if (key != 0) {
                LCD_Char('*');
                pass_user[idx++] = key;
            }

            while (ms_counter < timeout_ms);
            ms_counter = 0;
        } while (idx < 4);

        if (strncmp(pass_user, password, 4) == 0) {
            LCD_Clear();
            LCD_String_xy(0, 0, "Clave Correcta");

            for (int i = 0; i < 3; i++) {
                LED_VERDE = 1;
                timeout_ms = 500;
                while (ms_counter < timeout_ms);
                ms_counter = 0;

                LED_VERDE = 0;
                timeout_ms = 500;
                while (ms_counter < timeout_ms);
                ms_counter = 0;
            }

            intentos = 3;
            Estado = Monitoreo_Ambiental;

        } else {
            LCD_Clear();
            LCD_String_xy(0, 0, "Clave Incorrecta");

            for (int i = 0; i < 2; i++) {
                LED_ROJO = 1;
                timeout_ms = 300;
                while (ms_counter < timeout_ms);
                ms_counter = 0;

                LED_ROJO = 0;
                timeout_ms = 700;
                while (ms_counter < timeout_ms);
                ms_counter = 0;
            }

            intentos--;
        }
    } else {
        Estado = Bloqueado;
    }
    timeout_ms = 2000;
    while (ms_counter < timeout_ms);
    ms_counter = 0;
    LCD_Clear();
}

/**
 * Estado Bloqueado: indica que el sistema está bloqueado tras fallar los intentos de contraseña.
 */
void function_bloqueado(void) {
    LCD_Clear();
    LCD_String("Bloqueado");
    LED_ROJO = 1;

    timeout_ms = 10000;
    while (ms_counter < timeout_ms);
    ms_counter = 0;

    Estado = Seguridad;
    intentos = 3;
    LED_ROJO = 0;
}

/**
 * Estado Monitoreo Ambiental: muestra la temperatura y la luz en el LCD y verifica condiciones de alarma.
 */
void function_monitoreo_ambiental(void) {
    tempCelsius = leer_temperatura();
    light_intensity = leer_fotoresistor();

    LCD_Clear();
    char buffer[17];
    sprintf(buffer, "T: %dC L: %dLx", tempCelsius, light_intensity);
    LCD_String_xy(0, 0, buffer);

    LED_VERDE = 1;
    timeout_ms = 5000;

    if (tempCelsius > 30 || light_intensity > 800) {
        Estado = Alarma;
        ms_counter = 0;
        return;
    }

    if (ms_counter >= timeout_ms) {
        Estado = Monitor_Eventos;
        ms_counter = 0;
    }
}

/**
 * Estado Monitor de Eventos: muestra el sensor Hall y el de obstáculos y verifica el estado de alerta.
 */
void function_monitor_eventos(void) {
    sensor_hall = leer_sensor_hall();
    value_obstaculo = leer_sensor_obstaculos();

    LCD_Clear();
    char buffer[17];
    sprintf(buffer, "H: %d O: %d", sensor_hall, value_obstaculo);
    LCD_String_xy(0, 0, buffer);

    timeout_ms = 5000;

    if (sensor_hall > 0) {
        Estado = Alerta;
        ms_counter = 0;
    } else if (ms_counter >= timeout_ms) {
        Estado = Monitoreo_Ambiental;
        ms_counter = 0;
    }
}

/**
 * Estado Alarma: activa el LED azul y muestra "Alarma" en el LCD.
 */
void function_alarma(void) {
    LED_AZUL = 1;
    LCD_Clear();
    LCD_String_xy(0, 0, "Alarma");
    timeout_ms = 5000;

    if (ms_counter >= timeout_ms) {
        Estado = Monitoreo_Ambiental;
        ms_counter = 0;
        LED_AZUL = 0;
    }
}

/**
 * Estado Alerta: activa el LED rojo y muestra "Alerta" en el LCD.
 */
void function_alerta(void) {
    LED_ROJO = 1;
    LCD_Clear();
    LCD_String_xy(0, 0, "Alerta");
    timeout_ms = 5000;

    if (ms_counter >= timeout_ms) {
        Estado = Monitoreo_Ambiental;
        ms_counter = 0;
        LED_ROJO = 0;
    }
}
