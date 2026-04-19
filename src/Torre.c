#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Torre.h"
#include "Mouse.h"
#include "Batalla.h"
#include "Registro.h"
#include "Utils.h"
#include "Menu.h"

Jugador* GenerarRivalTorre(int nivelTorre) {
    Jugador* rival = (Jugador*)malloc(sizeof(Jugador));
    memset(rival, 0, sizeof(Jugador));
    sprintf(rival->nombre, "Entrenador Torre Lvl %d", nivelTorre);
    
    int numPkmn = 1;
    if (nivelTorre >= 3) numPkmn = 2;
    if (nivelTorre >= 5) numPkmn = 3;
    
    int lvlPkmn = nivelTorre * 5;
    
    for (int i = 0; i < numPkmn; i++) {
        int pkIdx = rand() % GetTotalPokemons();
        Pokemon p = GetPokemonPorIndice(pkIdx);
        p.nivel = lvlPkmn;
        CalcularStats(&p);
        
        // Asignar movimientos aleatorios (incluyendo los nuevos)
        int totalMovs = GetTotalMovimientos();
        for (int m = 0; m < 4; m++) {
            p.movimientos[m] = GetMovimientoPorIndice(rand() % totalMovs);
        }
        
        rival->pokemons[rival->cantidadPokemons++] = p;
    }
    
    return rival;
}

void IniciarTorre(Jugador* j) {
    srand(time(NULL));
    int cx = 40; // GetCentroX() hardcoded or include Menu.h
    
    for (int lvl = 1; lvl <= 5; lvl++) {
        BorrarPantallaCompleta();
        ImprimirCentrado(5, "--- MODO TORRE DE DESAFIO ---", 14);
        char b[100];
        sprintf(b, "Nivel Actual: %d / 5", lvl);
        ImprimirCentrado(7, b, 11);
        
        if (lvl == 5) ImprimirCentrado(9, "¡¡ NIVEL DE JEFE !!", 12);
        
        ImprimirCentrado(12, "[ COMENZAR BATALLA ]", 10);
        ImprimirCentrado(14, "[ ABANDONAR TORRE  ]", 12);
        
        while (1) {
            MouseState mouse = obtenerMouseState();
            if (mouseEnRango(cx - 10, 12, cx + 10, 12, mouse)) {
                Jugador* rival = GenerarRivalTorre(lvl);
                IniciarBatalla(j, rival);
                
                // Comprobar si el jugador ganó
                int ganaste = 0;
                for(int pk=0; pk<j->cantidadPokemons; pk++) if(j->pokemons[pk].vida > 0) ganaste = 1;
                
                if (ganaste) {
                    // Recompensas
                    if (lvl < 5) {
                        int premio = lvl * 500;
                        j->monedas += premio;
                        BorrarPantallaCompleta();
                        sprintf(b, "¡Nivel %d Superado! Ganaste $%d monedas.", lvl, premio);
                        ImprimirCentrado(10, b, 10);
                        Sleep(1500);
                    } else {
                        j->gemas += 5;
                        BorrarPantallaCompleta();
                        ImprimirCentrado(10, "¡TORRE COMPLETADA!", 14);
                        ImprimirCentrado(11, "Ganaste 5 Gemas de Jefe.", 13);
                        Sleep(2000);
                        free(rival);
                        return; // Fin de torre
                    }
                } else {
                    BorrarPantallaCompleta();
                    ImprimirCentrado(10, "Fuiste derrotado en la Torre...", 12);
                    Sleep(2000);
                    free(rival);
                    return;
                }
                free(rival);
                break;
            }
            if (mouseEnRango(cx - 10, 14, cx + 10, 14, mouse)) {
                return;
            }
            ProcesarMensajes();
            Sleep(50);
        }
    }
}
