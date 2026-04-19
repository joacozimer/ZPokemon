#ifndef Jugador_H
#define Jugador_H
#include "Pokemon.h"

typedef struct {
    int maxNivelVencido;
    int combatesGanados;
    int vitoriaTipoFuego;
    int victoriaAguaSoloAgua;
    int vitoriaRival6v6;
    int combatesSeguidos;
    int maxNivelAlcanzado;
} Estadisticas;

typedef struct{
    char nombre[255];
    int nivel;
    Pokemon pokemons[6];
    int cantidadPokemons;
    long monedas;
    int gemas;
    Pokemon pc[30];
    int cantidadPC;
    Estadisticas stats;
} Jugador;

Jugador* crearJugador(const char* nombre, int nivel);
void agregarPokemon(Jugador* jugador, Pokemon p);
Jugador* crearRival();

#endif
