/*
 * File:   config.c
 * Author: Espiraculo
 *
 * Created on 5 de abril de 2020, 15:36
 */


#include <xc.h>


void Config()
{
    //PortA
    ANSELA = 1;   // Registro que controla entradas analogicas => RA0 = entrada analogica, las demas digitales
    TRISA = 1;    // Registro que define si va a ser entrada o salida, 1 => RA0 es Entrada
    LATA = 0;
    
    //PortB
    ANSELB = 0;  // Todas entradas/salidas digitales
    LATB = 0;    // Escribe un 0 en la salida
    TRISB = 255; // Todas entradas
    WPUB = 0;    // Sin pull up
    
    //PortC
    ANSELC = 0; //Entradas anal�gicas desabilitadas
    TRISCbits.TRISC0 = 0;
    
    //PortD
    ANSELD = 0; //Entradas anal�gicas deshabilitadas
    TRISD = 192;
    
    //PORTE
    TRISE = 0;
    
    //INT0, RB0
    INTCONbits.INT0IE = 0; //no Habilita la interrupci�n 
    
    //INT2, RB2
    INTCON3bits.INT2IP = 1; //Alta prioridad
    INTCON3bits.INT2E = 1; //Habilita la interruci�n
    
    //Timer 0 (Base de tiempo de 10ms)
    T0CONbits.T08BIT = 0; //Timer como 16 bits
    T0CONbits.T0CS = 0; //Clock interno
    T0CONbits.PSA = 0; //Prescaler asignado al timer
    T0CONbits.T0PS = 2; //Prescaler en 8
    TMR0H = 99;
    TMR0L = 191; //Este va ultimo para que se haga la escritura (ver hoja de datos 11.3)
    INTCON2bits.TMR0IP = 0; //Baja prioridad
    INTCONbits.TMR0IE = 1; //Habilita la interrupci�n
    
    //Timer 4 (1: PR2 = 177 - 0: PR2 = 133 - Tbajo: 70) => Para transmitir con laser
    T4CONbits.T4OUTPS = 10; //postscaler en 11
    T4CONbits.T4CKPS = 0; //Prescaler en 1
    PR4 = 203;
    IPR5bits.TMR4IP = 1; //Alta prioridad
    T4CONbits.TMR4ON = 0; //Apaga el timer
    PIE5bits.TMR4IE = 1; //Habilita interrupci�n
    

    // Entradas por panel
//  --------------------------------

    //CCP1
    TRISCbits.TRISC2 = 1;
    CCPTMRS0bits.C1TSEL = 0; //Timer1
    CCP1CONbits.CCP1M = 5; //Configura en captura
    T1CONbits.TMR1ON = 1; //Enciende el timer
    IPR1bits.CCP1IP = 0; //Baja prioridad
    PIE1bits.CCP1IE = 1; //Habilita la interrupci�n
    
    //CCP2
    TRISCbits.TRISC1 = 1;
    CCPTMRS0bits.C2TSEL = 1; //Timer3
    CCP2CONbits.CCP2M = 5; //Configura en captura
    T3CONbits.TMR3ON = 1; //Enciende el timer
    IPR2bits.CCP2IP = 0; //Baja prioridad
    PIE2bits.CCP2IE = 1; //Habilita la interrupci�n
    
    //CCP3
    TRISEbits.TRISE0 = 1;
    CCPTMRS0bits.C3TSEL = 2; //Timer5
    CCP3CONbits.CCP3M = 5; //Configura en captura
    T5CONbits.TMR5ON = 1; //Enciende el timer
    IPR4bits.CCP3IP = 0; //Baja prioridad
    PIE4bits.CCP3IE = 1; //Habilita la interrupci�n
//  --------------------------


    //SPI => Protocolo de comunicacion, para leer por rfid
    TRISCbits.TRISC3 = 0; //CLK
    TRISCbits.TRISC4 = 1; //MISO
    TRISCbits.TRISC5 = 0; //MOSI
    SSP1STATbits.SMP = 0; //Dato se lee en el medio
    SSP1STATbits.CKE = 1; //Lee en flanco ascendente
    SSP1CON1bits.CKP = 0; //Normalmente bajo
    SSP1CON1bits.SSPM = 2; //SPI Master FOSC/64
    SSP1CON1bits.SSPEN = 1; //Habilita el puerto
    IPR1bits.SSP1IP = 0; //Baja prioridad
    PIE1bits.SSP1IE = 0; //Inhabilita la interrupci�n
    
    //EUSART (Debug), puerto serie 2
    BAUDCON2bits.BRG16 = 1;
    TXSTA2bits.BRGH = 0;
    TXSTA2bits.SYNC = 0;
    SPBRG2 = 0xA0;
    SPBRGH2 = 0x01;
    RCSTA2bits.CREN = 0; //Recepc�n continua
    TXSTA2bits.TXEN = 1; //Habilita el transmisor
    PIR3bits.TX2IF = 0; //Resetea el flag
    RCSTA2bits.SPEN = 1; //Habilita el puerto serie
    IPR3bits.TX2IP = 0; //Baja prioridad
    PIE3bits.TX2IE = 1; //Habilita la interrupci�n
    
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}