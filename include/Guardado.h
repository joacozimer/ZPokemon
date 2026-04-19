#ifndef Guardado_H
#define Guardado_H
#include "Jugador.h"

#define SAVE_FILE "PartidaActual.zpk"
#define SAVE_VERSION 3

typedef struct {
    char firma[4]; // "PKMN"
    int version;
} SaveHeader;

void GuardarPartida(Jugador* jugador);
Jugador* CargarPartida();
int ExistePartida();
void BorrarPartida();

#endif
