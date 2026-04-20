#ifndef Pokemon_H
#define Pokemon_H
#include "Estado.h"
#include "Movimiento.h"

typedef enum{
  VACIO, ACERO, VOLADOR, AGUA, HIELO, PLANTA, BICHO, ELECTRICO, NORMAL, ROCA, TIERRA, FUEGO, LUCHA, HADA, PSIQUICO, VENENO, DRAGON, FANTASMA, SINIESTRO
} tipoPokemon;

typedef enum {
    RAPIDO, MEDIO_RAPIDO, MEDIO_LENTO, LENTO
} CurvaExp;

typedef struct {
  char nombre[255];
  tipoPokemon tipo[2];
  int vida, vidaMax;
  int nivel;
  int atk, def, atkSp, defSp, vel;
  
  // Stats Base para el cálculo de crecimiento
  int bVida, bAtk, bDef, bAtkSp, bDefSp, bVel;
  
  long exp;
  int id;
  float vidaVisual; // Para animación suave de la barra de vida
  CurvaExp curva;
  
  estadoPokemon estado;
  Movimiento movimientos[4];
} Pokemon;

void CalcularStats(Pokemon* p);
void GanarExp(Pokemon* p, long expGano);
long GetExpNecesaria(int nivel, CurvaExp curva);

Pokemon crearBulbasaur();
Pokemon crearCharmander();
Pokemon crearSquirtle();
Pokemon crearPikachu();
Pokemon crearEevee();
Pokemon crearMew();
void InicializarStarters(Pokemon* p);

#endif
