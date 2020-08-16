/*
 * File:   EUSART.c
 * Author: Espiraculo
 *
 * Created on 22 de julio de 2020, 01:11
 */


#include "EUSART.h"

void EUSARTEnviar(char *reg, EUSART *e, char index)
{
    e->Transmitiendo = 1;
    e->cont = 1;
    *reg = e->Datos[0];
    if(index == 0)
    {
        PIE1bits.TX1IE = 1;
    }
    else
    {
        PIE3bits.TX2IE = 1;
    }
}

void EUSARTEnviar1(char *reg, EUSART *e)
{
    e->Transmitiendo = 1;
    e->cont = 1;
    *reg = e->Datos[0];
    PIE1bits.TX1IE = 1;
    
}

void EUSARTEnviar2(char *reg, EUSART *e)
{
    e->Transmitiendo = 1;
    e->cont = 1;
    *reg = e->Datos[0];
    PIE3bits.TX2IE = 1;
}