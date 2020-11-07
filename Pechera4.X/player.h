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
#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"

typedef enum playerState
{
    Vivo, 
    Muerto
}EstadoJugador;

typedef enum equipo 
{
    Rojo = 0, 
    Azul = 1, 
    Verde = 2,
    Celeste = 3, 
    Amarillo = 4, 
    violeta = 5
}Equipo;


typedef struct gun
{
    unsigned char damage; //Da�o del arma del jugador
    unsigned char tDisparos; //Tiempo de cadencia (tDisparo * 0.02)s
    unsigned char contDisparo; //Contador del tiempo de disparo
    Bit disparando; //Indica si se dispar�
    Bit habilDisparo; //Habilita la reactivaci�n del disparo
}Gun;

typedef struct statistics
{
    unsigned char Kills; 
    unsigned char Deaths;
    unsigned char Assists;
    unsigned char Punteria;
    union
    {
        int puntos;
        struct
        {
            unsigned char puntosL;
            unsigned char puntosH;
        };
    };
}Statistics;

typedef struct player
{
    EstadoJugador Estado;
    Equipo Equipo;
    unsigned char ID; //ID del jugador
    unsigned char Vida; //Cantidad de vida del jugador
    unsigned char Balas, BalasMax; //Balas y Balas por cargador
    unsigned char Cargadores, CargadoresMax; //Cantidad de cargadores y Cantidad de cargadores m�xima
    unsigned char contAssits; //Cuenta la cantidad que le pegaron
    unsigned char IDKiller; //Guarda el ID del que lo mata
    

    union
    {
        unsigned char IDHitters[10]; //Guarda los IDs de los que le dispararon
        struct
        {
            unsigned char Hitter0;
            unsigned char Hitter1;
            unsigned char Hitter2;
            unsigned char Hitter3;
            unsigned char Hitter4;
            unsigned char Hitter5;
            unsigned char Hitter6;
            unsigned char Hitter7;
            unsigned char Hitter8;
            unsigned char Hitter9;
        }Hitters;
    };
    
    Gun Arma;
    Statistics Estadisticas; 
}Player;

#endif