/*
 * File:   LReception->c
 * Author: Espiraculo
 *
 * Created on 5 de abril de 2020, 21:22
 */
#include "LReception.h"

void Capture(Reception *reception)
{
    if(reception->esperandoInicioRL)
    {
        if((reception->capturado._16 >= Min1) && (reception->capturado._16 <= Max1))
        {
            reception->esperandoInicioRL = 0;
        }
    }
    else
    {
        if((reception->capturado._16 >= Min1) && (reception->capturado._16 <= Max1)) //1
        {
            reception->DatoLaser = reception->DatoLaser >> 1;
            reception->nBitR++;
            reception->DatoLaser = reception->DatoLaser | 128;
        }
        else if((reception->capturado._16 >= Min0) && (reception->capturado._16 <= Max0)) //0
        {
            reception->DatoLaser = reception->DatoLaser >> 1;
            reception->nBitR++;
        }
        else //Ruido
        {
            reception->esperandoInicioRL = 1;
            reception->nBitR = 0;
        }

        if(reception->nBitR == 8)
        {
            reception->nBitR = 0;
            if(reception->contDatosRecL == 0) //El primer byte que llega
            {
                reception->cantDatosRecL = reception->DatoLaser;
            }
            else //El resto de los bytes
            {
                reception->DatosRecibidosL[reception->contDatosRecL - 1] = reception->DatoLaser;

                if(reception->cantDatosRecL == reception->contDatosRecL) //Fin de la cantidad de datos
                {
                    reception->esperandoInicioRL = 1;
                    reception->contDatosRecL = -1;
                    reception->DatoLaser = 0;
                    reception->DatosLRecibidoFlag = 1;
                }
            }

            reception->contDatosRecL++;
        }
    }
}

void ReadShot(GameConfig *gameConfig, Player *player, Reception *reception) //Lee la información de un disparo
{
    if(player->estado == Vivo) //Si el player está vivo
    {
        if(reception->DatosRecibidosL[1] != player->ID) //Si no es el mismo player
        {
            if(gameConfig->FriendlyFire || reception->DatosRecibidosL[2] != player->equipo) //Resuelve el si es del mismo equipo
            {
                if(player->Vida < reception->DatosRecibidosL[3]) //Si el player tiene que morir
                {
                    player->Vida = 0;
                    if(player->Estadisticas.puntos < gameConfig->RestaMuerte ) //Si los puntos tienen que ser menor que cero
                    {
                        player->Estadisticas.puntos = 0;
                    }
                    else
                    {
                        player->Estadisticas.puntos -= gameConfig->RestaMuerte;
                    }
                }
                else
                    player->Vida -= reception->DatosRecibidosL[3];
            }
        }
    }
}