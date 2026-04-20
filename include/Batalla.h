#ifndef Batalla_H
#define Batalla_H
#include "Jugador.h"

void IniciarBatalla(Jugador* jugador, Jugador* rival);
int HayPokemonsVivos(Jugador* j);
void ResetearEquipo(Jugador* j);
void MenuBatalla(Jugador* jugador, Jugador* rival);
void ConfigurarRivalMenu(Jugador* jugador);
void PersonalizarRival(Jugador* rival);

#endif
