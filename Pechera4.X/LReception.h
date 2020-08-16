/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#include "player.h"

#define Min0 2220
#define Max0 2320
#define Min1 2650
#define Max1 2750

typedef union int16_8
{
    unsigned int _16;
    struct
    {
        unsigned char _8L;
        unsigned char _8H;
    };
}Int16_8;

typedef struct reception
{
    Int16_8 capturado; //Valor que se recibe del CCP
    unsigned char esperandoInicioRL; //Espera el inicion de una trama
    unsigned char DatoLaser; //En donde se forma el dato
    unsigned char DatosRecibidosL[15]; //Guarda los datos recibidos
    signed char contDatosRecL; //contador de datos recibidos
    unsigned char cantDatosRecL; //Cantidad de datos que se esperan recibir
    unsigned char nBitR; //Contador de bits
    enum bits DatosLRecibidoFlag; //Indica cuando una trama se ricibio
}Reception;

void Capture(Reception *reception);

void ReadShot(GameConfig *gameConfig, Player *player, Reception *reception);