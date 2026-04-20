#ifndef Menu_H
#define Menu_H
#include <stdbool.h>
#include "Jugador.h"
#include "Pokemon.h"

// UI Utilities
void BorrarPantallaCompleta();
void ImprimirCentrado(int y, const char* texto, int color);
void DibujarBotonCentrado(int y, const char* texto, int color);
int GetCentroX();

// Screens
bool MostrarMenuPrincipal(Jugador* jugador);
void MostrarTienda(Jugador* jugador);
void MostrarConfiguracion(Jugador* jugador);
void MostrarPokedex();
void MostrarMisiones();

void VerPokemons(Jugador* jugador);
Pokemon ElegirStarterMouse();

#endif
