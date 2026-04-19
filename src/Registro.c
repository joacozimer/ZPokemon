#include "Registro.h"
#include <string.h>

// POKEMONS
int GetTotalPokemons() { return 3; }
Pokemon GetPokemonPorIndice(int idx) {
    if (idx == 0) return crearBulbasaur();
    if (idx == 1) return crearCharmander();
    return crearSquirtle();
}

// MOVIMIENTOS
int GetTotalMovimientos() { return 18; }
Movimiento GetMovimientoPorIndice(int idx) {
    switch(idx) {
        case 0: return crearPlacaje();
        case 1: return crearAscuas();
        case 2: return crearLatigoCepa();
        case 3: return crearBurbuja();
        case 4: return crearDestructor();
        case 5: return crearGolpeKarate();
        case 6: return crearDobleBofeton();
        case 7: return crearPunoCometa();
        case 8: return crearMegapuno();
        case 9: return crearDiaDePago();
        case 10: return crearPunoFuego();
        case 11: return crearPunoHielo();
        case 12: return crearPunoTrueno();
        case 13: return crearAranazo();
        case 14: return crearFuerzaDeGarra();
        case 15: return crearGuillotina();
        case 16: return crearVientoCortante();
        case 17: return crearDanzaDeEspadas();
        default: return crearPlacaje();
    }
}

// OBJETOS
int GetTotalObjetos() { return 3; }
Objeto GetObjetoPorIndice(int idx) {
    Objeto o;
    if (idx == 0) {
        strcpy(o.nombre, "Pocion");
        strcpy(o.descripcion, "Restaura 20 PS de un Pokemon.");
        o.precio = 100;
    } else if (idx == 1) {
        strcpy(o.nombre, "Super Bala");
        strcpy(o.descripcion, "Aumenta la precision temporalmente.");
        o.precio = 500;
    } else {
        strcpy(o.nombre, "Antidoto");
        strcpy(o.descripcion, "Cura el envenenamiento.");
        o.precio = 100;
    }
    return o;
}
