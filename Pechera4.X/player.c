#include "player.h"


unsigned char revivir(char* DatosEL, Player* jugador)
{
    
    DatosEL[0] = 3;
    DatosEL[1] = jugador->ID;
    DatosEL[2] = jugador->Equipo;
    DatosEL[3] = jugador->Arma.damage;

    jugador->Estado = Vivo;

    return 4; // Cantidad de Bytes en la trama para el laser
}
