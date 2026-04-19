#ifndef MISIONES_H
#define MISIONES_H

#include "Jugador.h"

typedef enum { FACIL, MEDIO, DIFICIL } Dificultad;

typedef struct {
    char nombre[100];
    char descripcion[255];
    Dificultad dificultad;
    int recompensa;
    int completada;
} Mision;

void InicializarMisiones();
void MostrarMenuMisiones(Jugador* j);

#endif
