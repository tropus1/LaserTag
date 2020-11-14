/*
 * File:   pechera3.c
 * Author: Espiraculo
 *
 * Created on 26 de octubre de 2019, 13:49
 */

// PIC18F46K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = HSMP      // Oscillator Selection bits (HS oscillator (medium power 4-16 MHz))
#pragma config PLLCFG = ON      // 4X PLL Enable (Oscillator multiplied by 4)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config CCP3MX = PORTE0  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is mulitplexed with RE0)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
#pragma config DEBUG = ON
// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.

#include <xc.h>
#include <pic18f46k22.h>
#include "LReception.h"
#include "EUSART.h"
#include "player.h"
#include "gameManager.c"

//Tiempos de l�ser
#define TiempoAlto1 177 //137 //177
#define TiempoAlto0 137 //93 //137
#define TiempoBajo 70 //110 //70

//Salidas
#define Laser LA2

//Entradas
#define PulDisparo PORTBbits.RB0


//Variables
GameConfig configJuego; //Guarda la configuraci�n del juego

Player jugador; //Guarda las variables del jugador


EUSART debug;
char debugCont = 0;

//Disparo laser
const unsigned char preambulo = 127; //Cabecera para enviar los datos
char DatosEL[6]; //Datos a enviar por el laser
unsigned char primerosBitsEL = 0; //Contador de los primeros bits
Bit flancoEL = 1; //Indica si hay que crear un flanco ascendente o descendente
unsigned char contByteEL = 0, contBitEL = 1; //Cuenta el byte que se est� transmitiendo, cuenta el bit que se est� transmitiendo
unsigned char cantBytesEL = 0; //Indica la cantidad de bytes que se van a transmitir

Reception receptores[3];

void __interrupt(high_priority) IRS_High() {
     
    //Disparo
    if(INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        //Poner flag de leido del puslo de disparo
        
        INTCONbits.INT0IF = 0; 
    }
    
    if(PIE5bits.TMR4IE && PIR5bits.TMR4IF) //Transmisi�n por l�ser
    {
        if(primerosBitsEL < 15) //Genera los primeros bits (Todos unos y un cero)
        {
            if(flancoEL)
            {
                Laser = 1;

                PR4 = TiempoAlto0;
                            
                flancoEL = 0;
            }
            else
            {
                Laser = 0;
                PR4 = TiempoBajo;
                flancoEL = 1;
                primerosBitsEL++;
            }
            
        }
        else if(primerosBitsEL == 15) //Genera el bit de inicio
        {
            if(flancoEL)
            {
                Laser = 1;
                
                PR4 = TiempoAlto1;
                            
                flancoEL = 0;
            }
            else
            {
                Laser = 0;
                PR4 = TiempoBajo;
                flancoEL = 1;
                primerosBitsEL++;
            }
        }
        else if(primerosBitsEL == 16)
        {
            if(flancoEL) //Se fija si hay que generar un flanco ascendente o uno descendente
            {
                Laser = 1;

                if(DatosEL[contByteEL] & contBitEL) //1
                    PR4 = TiempoAlto1;
                else //0
                    PR4 = TiempoAlto0;
                
                flancoEL = 0;
            }
            else 
            {
                Laser = 0;
                PR4 = TiempoBajo;
                flancoEL = 1;
                contBitEL = contBitEL << 1; 

                if(contBitEL == 0) //Se transmiti� un byte
                {
                    contBitEL = 1;
                    contByteEL++;
                    
                    if(contByteEL == cantBytesEL) //Se transmitieron todos los bytes
                    {
                        primerosBitsEL++;
                    }
                }
            }
        }
        else
        {
            if(flancoEL)
            {
                Laser = 1;
                flancoEL = 0;
                PR4 = 50;
            }
            else
            {
                primerosBitsEL = 0;
                flancoEL = 1;
                contBitEL = 1;
                contByteEL = 0;
                T4CONbits.TMR4ON = 0; //Apaga el timer
                PIE5bits.TMR4IE = 0; // Deshabilita la interrupci�n
                Laser = 0;
            }
        }

        PIR5bits.TMR4IF = 0;
    }
}

void __interrupt(low_priority) IRS_Low(){

    //Recepci�n 1
    if(PIE1bits.CCP1IE && PIR1bits.CCP1IF)
    {
        TMR1L = 0;
        TMR1H = 0;

        receptores[0].capturado._8L = CCPR1L;
        receptores[0].capturado._8H = CCPR1H;
        
        Capture(&receptores[0]);
        
        PIR1bits.CCP1IF = 0;
    }
    
    //Recepci�n 2
    if(PIE2bits.CCP2IE && PIR2bits.CCP2IF)
    {
        TMR3L = 0;
        TMR3H = 0;
        
        
        receptores[1].capturado._8L = CCPR3L;
        receptores[1].capturado._8H = CCPR3H;
        
        Capture(&receptores[1]);
        
        PIR2bits.CCP2IF = 0;
    }
    
    //Recepci�n 3
    if(PIE4bits.CCP3IE && PIR4bits.CCP3IF)
    {
        TMR5L = 0;
        TMR5H = 0;
        
        receptores[2].capturado._8L = CCPR5L;
        receptores[2].capturado._8H = CCPR5H;
        
        Capture(&receptores[2]);
        
        PIR4bits.CCP3IF = 0;
    }
    
    if(INTCONbits.T0IE && INTCONbits.T0IF) //Base de tiempo de 20ms
    {
        TMR0H = 99;
        TMR0L = 191;
        
        LA3 ^= 1;
        if(jugador.estado == Vivo)
        {
            if(jugador.Arma.disparando)
            {
                jugador.Arma.contDisparo++;
                if(jugador.Arma.contDisparo == jugador.Arma.tDisparos)
                {
                    jugador.Arma.contDisparo = 0;
                    jugador.Arma.habilDisparo = 1;
                }
            }
        }
        
        debugCont++;
        if(debugCont == 50)
        {
            debugCont = 0;
           /* for(char i = 0; i < 20; i++)
            {
                debug.Datos[i] = rfid.Datos[i];
            }
            debug.CantDatos = 20;
            EUSARTEnviar2(&TXREG2, &debug);*/
            debug.CantDatos = 3;
            EUSARTEnviar2(&TXREG2, &debug);
        }

        INTCONbits.T0IF = 0;
    }
    
    
    if(PIE3bits.TX2IE && PIR3bits.TX2IF)
    {
        if(debug.CantDatos > debug.cont)
        {
            TXREG2 = debug.Datos[debug.cont];
            debug.cont++;
        }
        else
        {
            debug.cont = 0;
            debug.Transmitiendo = 0;
            PIE3bits.TX2IE = 0;
        }
    }
}

void InitDebug(){

    configJuego.Estado = Inicializacion;
    configJuego.FriendlyFire = 0;
    configJuego.RestaKillEquipo = 3;
    configJuego.RestaMuerte = 6;
    configJuego.SumaAsesinato = 5;
    configJuego.SumaAsistencia = 1;
    
    jugador.Estado = Revivir;
    jugador.Equipo = Verde;
    jugador.ID = 101;
    jugador.Vida = 100;
    jugador.Balas = 20;
    jugador.BalasMax = 20;
    jugador.Cargadores = 1;
    jugador.CargadoresMax = 5;
    for(char i = 0; i < 10; i++)
        jugador.IDHitters[i] = 0;
    jugador.contAssits = 0;
    jugador.IDKiller = 0;
    
    jugador.Arma.damage = 15;
    jugador.Arma.tDisparos = 50;
    jugador.Arma.contDisparo = 0;
    jugador.Arma.disparando = 0;
    jugador.Arma.habilDisparo = 0;
    
    jugador.Estadisticas.puntos = 0;
    jugador.Estadisticas.Kills = 0;
    jugador.Estadisticas.Deaths = 0;
    jugador.Estadisticas.Assists = 0;
    jugador.Estadisticas.Punteria = 0;
    
    for(char i = 0; i < 3; i++)
    {
        receptores[i].esperandoInicioRL = 1;
        receptores[i].DatoLaser = 0;
        receptores[i].contDatosRecL = 0;
        receptores[i].cantDatosRecL = 0;
        receptores[i].nBitR = 0;
        receptores[i].DatosLRecibidoFlag = 0;
    }
    
    debug.Transmitiendo = 0;
    debug.CantDatos = 1;
    debug.cont = 0;
    debug.Datos[0] = 0;
}



void vivito_y_coleando(){

    /*
        Chequear la vida y transicionar o no a muerto

        if(Revivir)  
    {
        revivir(DatosEL,EstadoJugador); // PROBAR SI FUNCAN LAS DIRECCIONES
    }
    */

    if(PulDisparo && !jugador.Arma.disparando)
        {                    
            TMR4 = 0;
            PR4 = 20;
            T4CONbits.T4OUTPS = 10; //postscaler en 11
            T4CONbits.T4CKPS = 0; //Prescaler en 1
            T4CONbits.TMR4ON = 1; //Enciende el timer
            PIE5bits.TMR4IE = 1; //Habilita la interrupci�n

            jugador.Arma.disparando = 1;
            
            //Debug
            /* debug.Datos[0] = 'D';
            debug.Datos[1] = 'i';
            debug.Datos[2] = 's';
            debug.Datos[3] = 'p';
            debug.CantDatos = 1;
            EUSARTEnviar2(&TXREG2, &debug);*/
        }
        else if(!PulDisparo && jugador.Arma.habilDisparo)
        {
            jugador.Arma.habilDisparo = 0;
            jugador.Arma.disparando = 0;
        }

        

}

void check_enviroment(){

    if(receptores[0].DatosLRecibidoFlag) //Recepci�n de datos por entrada 1
        {
            ReadShot(&configJuego, &jugador, &receptores[0]);
            
            receptores[0].DatosLRecibidoFlag = 0;
        }

        if(receptores[1].DatosLRecibidoFlag) //Recepci�n de datos por entrada 2
        {
            ReadShot(&configJuego, &jugador, &receptores[1]);

            receptores[1].DatosLRecibidoFlag = 0;
        }

        if(receptores[2].DatosLRecibidoFlag) //Recepci�n de datos por entrada 3
        {
            ReadShot(&configJuego, &jugador, &receptores[2]);

            receptores[2].DatosLRecibidoFlag = 0;
        }
}

void  player_state_choice(){

    check_enviroment();      // Sabe lo que paso en el juego
    switch (player.Estado){

        case Vivo:
            vivito_y_coleando();
            break;

        case Muerto:
            /*
            Verificar como revivimos, etc..
            */
            break;
        default:
            break;
        }

}

void make_state_choice(GameConfig configJuego){

    switch (configJuego.Estado){

        case EnJuego:
            player_state_choice();
            break;

        case Inicializacion:

            break;

        case EsperaJuego:
            
            break;
        
        case ConfigurarRF:
            
            break;

        case EsperaConfigJuego:
            
            break;
        
        case ConfigurarJuego:
            
            break;
        
        case EsperaInicioJuego:
            
            break;
        
        case Finalizar:
            
            break;
        
        case ErrorRF:
            
            break;
        
        case ErrorConfig:
            
            break;
        
        case ErrorInit:
            
            break;

        case ErrorLaser:
            
            break;
        
        default:
            break;
        }
}

void main(void){

    Config();
    InitDebug();  
    configJuego.Estado = EnJuego;
    
    while(1){
        
        make_state_choice(configJuego);     
    }
}