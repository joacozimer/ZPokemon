#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Jugador.h"
#include "Pokemon.h"

Jugador* crearJugador(const char* nombre, int nivel){
    Jugador* nuevoJugador = (Jugador*)malloc(sizeof(Jugador));
    if(nuevoJugador == NULL) return NULL;
    
    strncpy(nuevoJugador->nombre, nombre, 255);
    nuevoJugador->nivel = nivel;
    nuevoJugador->cantidadPokemons = 0;
    nuevoJugador->monedas = 0;
    nuevoJugador->gemas = 0;
    
    // Inicializar Estadisticas
    memset(&nuevoJugador->stats, 0, sizeof(Estadisticas));
    
    return nuevoJugador;
}

void agregarPokemon(Jugador* jugador, Pokemon p){
    if(jugador->cantidadPokemons < 6){
        jugador->pokemons[jugador->cantidadPokemons] = p;
        jugador->cantidadPokemons++;
    }
}

Jugador* crearRival(){
    Jugador* rival = crearJugador("Rival", 1);
    return rival;
}
