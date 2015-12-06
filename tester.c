#define USE_AND_MASKS
#include <xc.h>
#include "PIC18F4480_config_tester.h"
#include "CANlib.h"
#include "delay.c"
#include "delay.h"
#include "pwm.h"
#include "timers.h"
#define _XTAL_FREQ 16000000

// RA0 => send 00
// RA1 => send 01
// RA2 => send 10
// RA3 => send 11
// RC0 => STATUS LED

#define brake_signal_id 0b00000000000000000000000000110 //<= ID CENTRALINA FRENO

void board_initialization(void);
void send_data(void);

unsigned long id = 0;
bit Tx_retry = 0;
CANmessage msg;
BYTE data_array [8] = 0;

unsigned long TMR3_counter = 0;
unsigned long TMR3_stored_1 = 0;
unsigned long TMR3_stored_2 = 0;

__interrupt(high_priority) void ISR_Alta(void) {
    TMR3_counter++;
    TMR3H = 0x63;
    TMR3L = 0xC0;
    PIR2bits.TMR3IF = 0;
}

int main(void) {
    board_initialization();
    while (1) {
        if ((TMR3_counter - TMR3_stored_1) > 3) {
            if (PORTAbits.RA0 == 1) {
                data_array[0] = 00;
                send_data();
            }
            if (PORTAbits.RA1 == 1) {
                data_array[0] = 01;
                send_data();
            }
            if (PORTAbits.RA2 == 1) {
                data_array[0] = 10;
                send_data();
            }
            if (PORTAbits.RA3 == 1) {
                data_array[0] = 11;
                send_data();
            }
            TMR3_stored_1 = TMR3_counter;
        }
        if (Tx_retry == 1) {
            send_data();
        }
        if ((TMR3_counter - TMR3_stored_2) > 15) {
            PORTCbits.RC0 = ~PORTCbits.RC0;
            TMR3_stored_2 = TMR3_counter;
        }
    }
}

void send_data(void) {
    if (CANisTxReady()) {
        CANsendMessage(brake_signal_id, data_array, 8, CAN_NORMAL_TX_FRAME & CAN_CONFIG_STD_MSG & CAN_TX_PRIORITY_3);
        if (TXB0CONbits.TXABT || TXB1CONbits.TXABT) {
            Tx_retry = 1;
        } else {
            Tx_retry = 0;
        }
    } else {
        Tx_retry = 1;
    }
}

void board_initialization(void) {
    //Configurazione I/O
    LATA = 0x00;
    TRISA = 0b00001111;
    LATB = 0x00;
    TRISB = 0xFF;
    LATC = 0x00;
    TRISC = 0x00;
    LATD = 0x00;
    TRISD = 0xFF;
    LATE = 0x00;
    TRISE = 0xFF;

    CANInitialize(4, 6, 5, 1, 3, CAN_CONFIG_LINE_FILTER_OFF & CAN_CONFIG_SAMPLE_ONCE & CAN_CONFIG_ALL_VALID_MSG & CAN_CONFIG_DBL_BUFFER_ON);

    RCONbits.IPEN = 0;
    PIR2bits.TMR3IF = 0; //resetta flag interrupt timer 3
    IPR2bits.TMR3IP = 1; //interrupt alta priorità timer 3
    TMR3H = 0x63; //<= VALORI PER AVERE UN
    TMR3L = 0xC0; //<= INTERRUPT OGNI 10ms
    INTCON2bits.INTEDG0 = 1; //interrupt su fronte di salita
    PIE2bits.TMR3IE = 1; //abilita interrupt timer 3
    INTCONbits.GIE = 1; //abilita interrupt alta priorità
    INTCONbits.PEIE = 1; //abilita interrupt alta priorità periferiche

    T3CON = 0x01; //abilita timer
    delay_ms(2);
}
