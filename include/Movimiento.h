#ifndef Movimiento_H
#define Movimiento_H
#include "Estado.h"

typedef enum {
    FISICO, ESPECIAL, ESTADO
} claseMovimiento;

// Forzamos la inclusión de tipoPokemon aquí o usamos un header común
// Para simplificar, asumiremos que tipoPokemon ya es conocido o usamos int
// Pero mejor incluirlo si es posible. Dado que Pokemon.h incluye este, 
// a veces hay circulares. Usaremos un entero o forward declaration.
typedef int tipoMovInt; 

typedef struct{
  char nombre[255];
  int potencia;
  int precision;
  int tipo; // Usaremos el enum tipoPokemon de Pokemon.h (casteado a int para evitar circulares)
  claseMovimiento clase;
  int pp;
  int ppMax;
  int probEstado; // Probabilidad de 0 a 100
  int estadoEfecto; // casteado a int
  
  // EFECTOS ESPECIALES
  int golpeMin, golpeMax; // Para ataques múltiples (Doble Bofetón)
  int esOHKO;             // Para Guillotina
  int buffStat;           // 0: Ninguno, 1: Atk, 2: Def, 3: AtkSp, 4: DefSp, 5: Vel
  int cantBuff;           // Cuantos niveles sube/baja
} Movimiento;

Movimiento crearPlacaje();
Movimiento crearAscuas();
Movimiento crearLatigoCepa();
Movimiento crearBurbuja();

// GENERACION 1
Movimiento crearDestructor();      // Pound
Movimiento crearGolpeKarate();     // Karate Chop
Movimiento crearDobleBofeton();    // Double Slap
Movimiento crearPunoCometa();      // Comet Punch
Movimiento crearMegapuno();        // Mega Punch
Movimiento crearDiaDePago();       // Pay Day
Movimiento crearPunoFuego();       // Fire Punch
Movimiento crearPunoHielo();       // Ice Punch
Movimiento crearPunoTrueno();      // Thunder Punch
Movimiento crearAranazo();         // Scratch
Movimiento crearFuerzaDeGarra();   // Vice Grip
Movimiento crearGuillotina();      // Guillotine
Movimiento crearVientoCortante();   // Razor Wind
Movimiento crearDanzaDeEspadas();   // Swords Dance

#endif
