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
#ifndef CONFIG_H
#define	CONFIG_H

#define _XTAL_FREQ 16000000

typedef enum bits
{
    off = 0,
    on = 1
}Bit;

typedef enum sysState
{
    Inicializacion,
    EsperaJuego,
    ConfigurarRF,
    EsperaConfigJuego,
    ConfigurarJuego,
    EsperaInicioJuego,
    EnJuego,
    Finalizar,
    ErrorRF,
    ErrorConfig,
    ErrorInit,
    ErrorLaser
} EstadoSistema;

typedef struct gameConfig
{
    unsigned char SumaAsesinato; //Cuantos puntas suma un kill
    unsigned char RestaMuerte; //Cuantos puntas resta una muerte
    unsigned char SumaAsistencia; //Cuantos puntas suma un asist
    unsigned char RestaKillEquipo; //Cuantos puntos resta matar a uno de tu mismo equipo
    Bit FriendlyFire; //Indica si hay fuego amigo
    EstadoJuego Estado; //Estado actual del juego
}GameConfig;


void Config(void);

#endif