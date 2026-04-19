#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Guardado.h"
#include "Utils.h"

void GuardarPartida(Jugador* jugador) {
    FILE* file = fopen(SAVE_FILE, "wb");
    if (file == NULL) {
        RegistrarError("No se pudo abrir el archivo para guardar.");
        return;
    }
    
    SaveHeader header = { "PKMN", SAVE_VERSION };
    fwrite(&header, sizeof(SaveHeader), 1, file);
    fwrite(jugador, sizeof(Jugador), 1, file);
    fclose(file);
}

// Estructuras antiguas para migración
typedef struct {
    char nombre[255];
    int potencia, precision;
} V1Movimiento;

typedef struct {
    char nombre[255];
    int tipo[2];
    int vida, atk, def, atkSp, defSp, vel;
    int estado;
    V1Movimiento movimientos[4];
} V1Pokemon;

typedef struct {
    char nombre[255];
    int nivel;
    V1Pokemon pokemons[6];
    int cantidadPokemons;
} V1Jugador;

void MigrarV1toV2(V1Jugador* v1, Jugador* v2) {
    strncpy(v2->nombre, v1->nombre, 255);
    v2->nivel = v1->nivel;
    v2->cantidadPokemons = v1->cantidadPokemons;
    for (int i = 0; i < v1->cantidadPokemons; i++) {
        strncpy(v2->pokemons[i].nombre, v1->pokemons[i].nombre, 255);
        v2->pokemons[i].vida = v1->pokemons[i].vida;
        v2->pokemons[i].vidaMax = v1->pokemons[i].vida; // Asumimos vida max = actual
        v2->pokemons[i].nivel = 5; // Default para migrados
        v2->pokemons[i].atk = v1->pokemons[i].atk;
        v2->pokemons[i].def = v1->pokemons[i].def;
        v2->pokemons[i].atkSp = v1->pokemons[i].atkSp;
        v2->pokemons[i].defSp = v1->pokemons[i].defSp;
        v2->pokemons[i].vel = v1->pokemons[i].vel;
        v2->pokemons[i].estado = (estadoPokemon)v1->pokemons[i].estado;
        
        for (int j = 0; j < 4; j++) {
            strncpy(v2->pokemons[i].movimientos[j].nombre, v1->pokemons[i].movimientos[j].nombre, 255);
            v2->pokemons[i].movimientos[j].potencia = v1->pokemons[i].movimientos[j].potencia;
            v2->pokemons[i].movimientos[j].precision = v1->pokemons[i].movimientos[j].precision;
            v2->pokemons[i].movimientos[j].pp = 20; // Default
            v2->pokemons[i].movimientos[j].ppMax = 20;
            v2->pokemons[i].movimientos[j].clase = FISICO;
        }
    }
}

typedef struct {
    char nombre[255];
    int nivel;
    Pokemon pokemons[6];
    int cantidadPokemons;
    long monedas;
    int gemas;
} V2Jugador;

void MigrarV2toV3(V2Jugador* v2, Jugador* v3) {
    strncpy(v3->nombre, v2->nombre, 255);
    v3->nivel = v2->nivel;
    v3->monedas = v2->monedas;
    v3->gemas = v2->gemas;
    v3->cantidadPokemons = v2->cantidadPokemons;
    for (int i = 0; i < 6; i++) v3->pokemons[i] = v2->pokemons[i];
    v3->cantidadPC = 0;
}

Jugador* CargarPartida() {
    FILE* file = fopen(SAVE_FILE, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    Jugador* jugador = (Jugador*)malloc(sizeof(Jugador));
    memset(jugador, 0, sizeof(Jugador));

    // Migración V1 (sin cabecera)
    if (size == sizeof(V1Jugador)) {
        V1Jugador v1;
        fread(&v1, sizeof(V1Jugador), 1, file);
        MigrarV1toV2(&v1, jugador);
        fclose(file);
        GuardarPartida(jugador);
        return jugador;
    }

    SaveHeader header;
    if (fread(&header, sizeof(SaveHeader), 1, file) != 1) {
        free(jugador);
        fclose(file);
        return NULL;
    }

    // Migración V2 (con cabecera, versión 2)
    if (strncmp(header.firma, "PKMN", 4) == 0 && header.version == 2) {
        V2Jugador v2;
        fread(&v2, sizeof(V2Jugador), 1, file);
        MigrarV2toV3(&v2, jugador);
        fclose(file);
        GuardarPartida(jugador);
        return jugador;
    }

    if (strncmp(header.firma, "PKMN", 4) != 0 || header.version != SAVE_VERSION) {
        fclose(file);
        RegistrarError("Version de partida incompatible.");
        free(jugador);
        return NULL;
    }

    if (fread(jugador, sizeof(Jugador), 1, file) != 1) {
        free(jugador);
        fclose(file);
        return NULL;
    }
    fclose(file);
    return jugador;
}

void BorrarPartida() {
    remove(SAVE_FILE);
}

int ExistePartida() {
    FILE* file = fopen(SAVE_FILE, "rb");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}
