#ifndef Menu_H
#define Menu_H
#include "Jugador.h"
#include "Pokemon.h"

// UI Utilities
void BorrarPantallaCompleta();
void ImprimirCentrado(int y, const char* texto, int color);
void DibujarBotonCentrado(int y, const char* texto, int color);
int GetCentroX();

// Screens
void MostrarMenuPrincipal(Jugador* jugador);
void MostrarTienda(Jugador* jugador);
void MostrarConfiguracion(Jugador* jugador);
void MostrarPokedex();

void VerPokemons(Jugador* jugador);
Pokemon ElegirStarterMouse();

#endif
