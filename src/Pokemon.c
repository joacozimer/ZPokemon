#include <string.h>
#include "Pokemon.h"

#include <math.h>
#include "Pokemon.h"

long GetExpNecesaria(int nivel, CurvaExp curva) {
    double n = (double)nivel;
    switch(curva) {
        case RAPIDO: return (long)(0.8 * pow(n, 3));
        case MEDIO_RAPIDO: return (long)(pow(n, 3));
        case MEDIO_LENTO: return (long)(1.2 * pow(n, 3) - 15 * pow(n, 2) + 100 * n - 140);
        case LENTO: return (long)(1.25 * pow(n, 3));
        default: return (long)pow(n, 3);
    }
}

void CalcularStats(Pokemon* p) {
    // PS = floor((2 * Base) * Nivel / 100) + Nivel + 10
    int viejaVidaMax = p->vidaMax;
    p->vidaMax = (int)((2.0 * p->bVida * p->nivel) / 100.0) + p->nivel + 10;
    
    // Ajustar vida actual proporcionalmente o solo sumar la diferencia
    if (viejaVidaMax > 0) p->vida += (p->vidaMax - viejaVidaMax);
    else p->vida = p->vidaMax;

    // Stat = floor(((2 * Base) * Nivel / 100) + 5)
    p->atk = (int)((2.0 * p->bAtk * p->nivel) / 100.0) + 5;
    p->def = (int)((2.0 * p->bDef * p->nivel) / 100.0) + 5;
    p->atkSp = (int)((2.0 * p->bAtkSp * p->nivel) / 100.0) + 5;
    p->defSp = (int)((2.0 * p->bDefSp * p->nivel) / 100.0) + 5;
    p->vel = (int)((2.0 * p->bVel * p->nivel) / 100.0) + 5;
}

void GanarExp(Pokemon* p, long expGano) {
    p->exp += expGano;
    while (p->nivel < 100 && p->exp >= GetExpNecesaria(p->nivel + 1, p->curva) && GetExpNecesaria(p->nivel + 1, p->curva) > 0) {
        p->nivel++;
        CalcularStats(p);
    }
}

void InicializarStarters(Pokemon* p) {
    p->nivel = 5;
    p->exp = GetExpNecesaria(p->nivel, p->curva);
    for(int i=0; i<4; i++) p->movimientos[i].nombre[0] = '\0';
    p->movimientos[0] = crearPlacaje();
    CalcularStats(p);
    p->vidaVisual = (float)p->vida;
}

Pokemon crearBulbasaur(){
  Pokemon p;
  strncpy(p.nombre, "Bulbasaur", 255);
  p.tipo[0] = PLANTA; p.tipo[1] = VENENO;
  p.bVida = 45; p.bAtk = 49; p.bDef = 49; p.bAtkSp = 65; p.bDefSp = 65; p.bVel = 45;
  p.curva = MEDIO_LENTO;
  p.id = 0; // Bulbasaur
  p.vidaMax = 0; // Para que CalcularStats inicialice vida
  InicializarStarters(&p);
  p.movimientos[1] = crearLatigoCepa();
  return p;
}

Pokemon crearCharmander(){
  Pokemon p;
  strncpy(p.nombre, "Charmander", 255);
  p.tipo[0] = FUEGO; p.tipo[1] = VACIO;
  p.bVida = 39; p.bAtk = 52; p.bDef = 43; p.bAtkSp = 60; p.bDefSp = 50; p.bVel = 65;
  p.curva = MEDIO_LENTO;
  p.id = 1; // Charmander
  p.vidaMax = 0;
  InicializarStarters(&p);
  p.movimientos[1] = crearAscuas();
  return p;
}

Pokemon crearSquirtle(){
  Pokemon p;
  strncpy(p.nombre, "Squirtle", 255);
  p.tipo[0] = AGUA; p.tipo[1] = VACIO;
  p.bVida = 44; p.bAtk = 48; p.bDef = 65; p.bAtkSp = 50; p.bDefSp = 64; p.bVel = 43;
  p.curva = MEDIO_LENTO;
  p.id = 2; // Squirtle
  p.vidaMax = 0;
  InicializarStarters(&p);
  p.movimientos[1] = crearBurbuja();
  return p;
}

Pokemon crearPikachu() {
  Pokemon p;
  strncpy(p.nombre, "Pikachu", 255);
  p.tipo[0] = ELECTRICO; p.tipo[1] = VACIO;
  p.bVida = 35; p.bAtk = 55; p.bDef = 40; p.bAtkSp = 50; p.bDefSp = 50; p.bVel = 90;
  p.curva = MEDIO_RAPIDO;
  p.id = 3; // Pikachu
  p.vidaMax = 0;
  InicializarStarters(&p);
  return p;
}

Pokemon crearEevee() {
  Pokemon p;
  strncpy(p.nombre, "Eevee", 255);
  p.tipo[0] = NORMAL; p.tipo[1] = VACIO;
  p.bVida = 55; p.bAtk = 55; p.bDef = 50; p.bAtkSp = 45; p.bDefSp = 65; p.bVel = 55;
  p.curva = MEDIO_RAPIDO;
  p.id = 4; // Eevee
  p.vidaMax = 0;
  InicializarStarters(&p);
  return p;
}

Pokemon crearMew() {
  Pokemon p;
  strncpy(p.nombre, "Mew", 255);
  p.tipo[0] = PSIQUICO; p.tipo[1] = VACIO;
  p.bVida = 100; p.bAtk = 100; p.bDef = 100; p.bAtkSp = 100; p.bDefSp = 100; p.bVel = 100;
  p.curva = MEDIO_LENTO;
  p.id = 5; // Mew
  p.vidaMax = 0;
  InicializarStarters(&p);
  return p;
}
