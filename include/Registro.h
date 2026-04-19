#ifndef Registro_H
#define Registro_H

#include "Pokemon.h"
#include "Movimiento.h"

typedef struct {
    char nombre[255];
    char descripcion[255];
    int precio;
} Objeto;

// REGISTROS
int GetTotalPokemons();
Pokemon GetPokemonPorIndice(int idx);

int GetTotalMovimientos();
Movimiento GetMovimientoPorIndice(int idx);

int GetTotalObjetos();
Objeto GetObjetoPorIndice(int idx);

#endif
