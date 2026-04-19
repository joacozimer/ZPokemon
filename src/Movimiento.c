#include <string.h>
#include "Movimiento.h"
#include "Pokemon.h"

// Definición de tipos para evitar circulares si es necesario, 
// pero aquí ya tenemos los headers necesarios.

Movimiento crearPlacaje() {
    Movimiento m;
    strncpy(m.nombre, "Placaje", 255);
    m.potencia = 40;
    m.precision = 100;
    m.tipo = (int)NORMAL; 
    m.clase = FISICO;
    m.ppMax = 20;
    m.pp = 20;
    m.probEstado = 0;
    m.estadoEfecto = DEBILITADO;
    return m;
}

Movimiento crearAscuas() {
    Movimiento m;
    strncpy(m.nombre, "Ascuas", 255);
    m.potencia = 40;
    m.precision = 100;
    m.tipo = (int)FUEGO;
    m.clase = ESPECIAL;
    m.ppMax = 25;
    m.pp = 25;
    m.probEstado = 10;
    m.estadoEfecto = QUEMADO;
    return m;
}

Movimiento crearLatigoCepa() {
    Movimiento m;
    strncpy(m.nombre, "Latigo Cepa", 255);
    m.potencia = 45;
    m.precision = 100;
    m.tipo = (int)PLANTA;
    m.clase = FISICO;
    m.ppMax = 25;
    m.pp = 25;
    m.probEstado = 0;
    m.estadoEfecto = SALUDABLE;
    return m;
}

Movimiento crearBurbuja() {
    Movimiento m;
    strncpy(m.nombre, "Burbuja", 255);
    m.potencia = 40;
    m.precision = 100;
    m.tipo = (int)AGUA;
    m.clase = ESPECIAL;
    m.ppMax = 30;
    m.pp = 30;
    m.probEstado = 10;
    m.estadoEfecto = SALUDABLE;
    return m;
}

// HELPER
Movimiento iniciarMov(const char* nom, int pot, int prec, tipoPokemon t, claseMovimiento c, int pp) {
    Movimiento m;
    strncpy(m.nombre, nom, 255);
    m.potencia = pot;
    m.precision = prec;
    m.tipo = (int)t;
    m.clase = c;
    m.ppMax = pp;
    m.pp = pp;
    m.probEstado = 0;
    m.estadoEfecto = (int)SALUDABLE;
    m.golpeMin = 1; m.golpeMax = 1;
    m.esOHKO = 0;
    m.buffStat = 0;
    m.cantBuff = 0;
    return m;
}

Movimiento crearDestructor() {
    return iniciarMov("Destructor", 40, 100, NORMAL, FISICO, 35);
}

Movimiento crearGolpeKarate() {
    return iniciarMov("Golpe Karate", 50, 100, LUCHA, FISICO, 25);
}

Movimiento crearDobleBofeton() {
    Movimiento m = iniciarMov("Doble Bofeton", 15, 85, NORMAL, FISICO, 10);
    m.golpeMin = 2; m.golpeMax = 5;
    return m;
}

Movimiento crearPunoCometa() {
    Movimiento m = iniciarMov("Puno Cometa", 18, 85, NORMAL, FISICO, 15);
    m.golpeMin = 2; m.golpeMax = 5;
    return m;
}

Movimiento crearMegapuno() {
    return iniciarMov("Megapuno", 80, 85, NORMAL, FISICO, 20);
}

Movimiento crearDiaDePago() {
    return iniciarMov("Dia de Pago", 40, 100, NORMAL, FISICO, 20);
}

Movimiento crearPunoFuego() {
    Movimiento m = iniciarMov("Puno Fuego", 75, 100, FUEGO, FISICO, 15);
    m.probEstado = 10; m.estadoEfecto = (int)QUEMADO;
    return m;
}

Movimiento crearPunoHielo() {
    Movimiento m = iniciarMov("Puno Hielo", 75, 100, HIELO, FISICO, 15);
    m.probEstado = 10; m.estadoEfecto = (int)CONGELADO;
    return m;
}

Movimiento crearPunoTrueno() {
    Movimiento m = iniciarMov("Puno Trueno", 75, 100, ELECTRICO, FISICO, 15);
    m.probEstado = 10; m.estadoEfecto = (int)PARALIZADO;
    return m;
}

Movimiento crearAranazo() {
    return iniciarMov("Aranazo", 40, 100, NORMAL, FISICO, 35);
}

Movimiento crearFuerzaDeGarra() {
    return iniciarMov("Fuerza de Garra", 55, 100, NORMAL, FISICO, 30);
}

Movimiento crearGuillotina() {
    Movimiento m = iniciarMov("Guillotina", 0, 30, NORMAL, FISICO, 5);
    m.esOHKO = 1;
    return m;
}

Movimiento crearVientoCortante() {
    return iniciarMov("Viento Cortante", 80, 100, NORMAL, ESPECIAL, 10);
}

Movimiento crearDanzaDeEspadas() {
    Movimiento m = iniciarMov("Danza Espadas", 0, 100, NORMAL, ESTADO, 20);
    m.buffStat = 1; // Atk
    m.cantBuff = 2;
    return m;
}

