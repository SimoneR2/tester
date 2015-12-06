#define USE_AND_MASKS
#include <xc.h>
#include "newfile.h"
#include <CANlib.h>
#include <pic18f4480.h>
#define _XTAL_FREQ 16000000

#define start_measure 0b00000000000000000000000000110
#define stop_measure 0b00000000000000000000000000101
#define speed_info 0b00000000000000000000000001000
#define speed_change 0b00000000000000000000000000110
#define emergency 0b00000000000000000000000000001

void configurazione_iniziale(void);
void send_data(void);
void delay_ms(void);
bit remote_frame = 0;
unsigned long id = 0;
CANmessage msg;
BYTE counter_array [8] = 0;
BYTE currentSpeed_array [8] = 0;
BYTE data_array [8] = 0;
//int step = 0;
//int loop = 0;

//*************************************
//ISR Bassa priorità (gestione can bus)
//*************************************

//__interrupt(low_priority) void ISR_bassa(void) {
//    if ((PIR3bits.RXB0IF == 1) || (PIR3bits.RXB1IF == 1)) {
//        if (CANisRxReady()) { //Se il messaggio è arrivato
//            CANreceiveMessage(&msg); //leggilo e salvalo
//            if (msg.identifier == stop_measure) {
//                step = msg.data[0];
//                for (int i = 0; i < step; i++) {
//                    PORTC = 0xFF;
//                    delay_ms(200);
//                    PORTC = 0x00;
//                    delay_ms(200);
//                }
//            }
//
//        }
//    }
//}

int main(void) {
    configurazione_iniziale();
    PORTC = 0xFF;
    delay_ms();
    PORTC = 0x00;
    delay_ms();

    while (1) {
        data_array[0] = 0x00;
        PORTC = 0xFF;
        delay_ms();
        PORTC = 0x00;
        delay_ms();
        PORTC = 0xFF;
        delay_ms();
        PORTC = 0x00;
        send_data();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        data_array[0] = 0x10;
        PORTC = 0xFF;
        delay_ms();
        PORTC = 0x00;
        delay_ms();
        PORTC = 0xFF;
        delay_ms();
        PORTC = 0x00;
        send_data();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
        delay_ms();
    }
}

void send_data(void) {
    if (CANisTxReady()) { //è disponibile almeno un buffer per l'invio dei dati
        CANsendMessage(speed_change, data_array, 8, CAN_NORMAL_TX_FRAME & CAN_CONFIG_STD_MSG & CAN_TX_PRIORITY_3);
        remote_frame = 0; //azzero flag risposta remote frame
        PORTC = 0xFF;
        delay_ms();
        PORTC = 0x00;
        delay_ms();
    }
}

void delay_ms() {
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);


}

void configurazione_iniziale(void) {

    //impostazione periferica can bus e azzeramento flag interrupt
    CANInitialize(4, 6, 5, 1, 3, CAN_CONFIG_LINE_FILTER_OFF & CAN_CONFIG_SAMPLE_ONCE & CAN_CONFIG_ALL_VALID_MSG & CAN_CONFIG_DBL_BUFFER_ON);
    RCONbits.IPEN = 1; //abilita priorità interrupt
    INTCONbits.INT0IF = 0; //azzera flag interrupt RB0
    INTCONbits.INT0IE = 0; //disabilita interrupt RB0
    PIR3bits.RXB1IF = 0; //azzera flag interrupt can bus buffer1
    PIR3bits.RXB0IF = 0; //azzera flag interrupt can bus buffer0
    PIE3bits.RXB1IE = 1; //abilita interrupt ricezione can bus buffer1
    PIE3bits.RXB0IE = 1; //abilita interrupt ricezione can bus buffer0
    IPR3bits.RXB1IP = 0; //interrupt bassa priorità per can
    IPR3bits.RXB0IP = 0; //interrupt bassa priorità per can
    INTCONbits.GIEH = 1; //abilita interrupt alta priorità
    INTCONbits.GIEL = 1; //abilita interrupt bassa priorità periferiche
    INTCON2bits.INTEDG0 = 1; //interrupt su fronte di salita

    //impostazione porte
    LATA = 0x00;
    TRISA = 0b11111110;

    LATB = 0x00;
    TRISB = 0b11111011;

    LATC = 0x00;
    TRISC = 0x00;

    LATD = 0x00;
    TRISD = 0b00001111;

    LATE = 0x00;
    TRISE = 0x00;

}
