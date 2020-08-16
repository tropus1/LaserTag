
# 1 "LReception.c"

# 33 "config.h"
typedef enum bits
{
off = 0,
on = 1
}Bit;

typedef enum gameState
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
}EstadoJuego;

typedef struct gameConfig
{
unsigned char SumaAsesinato;
unsigned char RestaMuerte;
unsigned char SumaAsistencia;
unsigned char RestaKillEquipo;
Bit FriendlyFire;
EstadoJuego Estado;
}GameConfig;


void Config(void);

# 30 "player.h"
typedef enum playerState
{
Vivo,
Muerto,
Revivir,
Morir
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
unsigned char damage;
unsigned char tDisparos;
unsigned char contDisparo;
Bit disparando;
Bit habilDisparo;
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
EstadoJugador estado;
Equipo equipo;
unsigned char ID;
unsigned char Vida;
unsigned char Balas, BalasMax;
unsigned char Cargadores, CargadoresMax;
unsigned char contAssits;
unsigned char IDKiller;


union
{
unsigned char IDHitters[10];
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

# 36 "LReception.h"
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
Int16_8 capturado;
unsigned char esperandoInicioRL;
unsigned char DatoLaser;
unsigned char DatosRecibidosL[15];
signed char contDatosRecL;
unsigned char cantDatosRecL;
unsigned char nBitR;
enum bits DatosLRecibidoFlag;
}Reception;

void Capture(Reception *reception);

void ReadShot(GameConfig *gameConfig, Player *player, Reception *reception);

# 9 "LReception.c"
void Capture(Reception *reception)
{
if(reception->esperandoInicioRL)
{
if((reception->capturado._16 >= 2650) && (reception->capturado._16 <= 2750))
{
reception->esperandoInicioRL = 0;
}
}
else
{
if((reception->capturado._16 >= 2650) && (reception->capturado._16 <= 2750))
{
reception->DatoLaser = reception->DatoLaser >> 1;
reception->nBitR++;
reception->DatoLaser = reception->DatoLaser | 128;
}
else if((reception->capturado._16 >= 2220) && (reception->capturado._16 <= 2320))
{
reception->DatoLaser = reception->DatoLaser >> 1;
reception->nBitR++;
}
else
{
reception->esperandoInicioRL = 1;
reception->nBitR = 0;
}

if(reception->nBitR == 8)
{
reception->nBitR = 0;
if(reception->contDatosRecL == 0)
{
reception->cantDatosRecL = reception->DatoLaser;
}
else
{
reception->DatosRecibidosL[reception->contDatosRecL - 1] = reception->DatoLaser;

if(reception->cantDatosRecL == reception->contDatosRecL)
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

void ReadShot(GameConfig *gameConfig, Player *player, Reception *reception)
{
if(player->estado == Vivo)
{
if(reception->DatosRecibidosL[1] != player->ID)
{
if(gameConfig->FriendlyFire || reception->DatosRecibidosL[2] != player->equipo)
{
if(player->Vida < reception->DatosRecibidosL[3])
{
player->Vida = 0;
if(player->Estadisticas.puntos < gameConfig->RestaMuerte )
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
