#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Torre.h"
#include "Batalla.h"
#include "Registro.h"
#include "Utils.h"
#include "Menu.h"
#include "raylib.h"
#include "raygui.h"

Jugador* GenerarRivalTorre(int nivelTorre) {
    Jugador* rival = (Jugador*)malloc(sizeof(Jugador));
    memset(rival, 0, sizeof(Jugador));
    sprintf(rival->nombre, "Entrenador Torre Lvl %d", nivelTorre);
    
    int numPkmn = (nivelTorre >= 5) ? 3 : (nivelTorre >= 3 ? 2 : 1);
    int lvlPkmn = nivelTorre * 5;
    
    for (int i = 0; i < numPkmn; i++) {
        int pkIdx = rand() % GetTotalPokemons();
        Pokemon p = GetPokemonPorIndice(pkIdx);
        p.nivel = lvlPkmn;
        CalcularStats(&p);
        
        int totalMovs = GetTotalMovimientos();
        for (int m = 0; m < 4; m++) {
            p.movimientos[m] = GetMovimientoPorIndice(rand() % totalMovs);
        }
        p.vida = p.vidaMax;
        rival->pokemons[rival->cantidadPokemons++] = p;
    }
    return rival;
}

void IniciarTorre(Jugador* j) {
    srand(time(NULL));
    
    for (int lvl = 1; lvl <= 5; lvl++) {
        while (!WindowShouldClose()) {
            ActualizarInputLock(GetFrameTime());
            bool locked = IsInputLocked();

            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("--- MODO TORRE DE DESAFIO ---", 400 - MeasureText("--- MODO TORRE DE DESAFIO ---", 30)/2, 100, 30, YELLOW);
            DrawText(TextFormat("Nivel Actual: %d / 5", lvl), 400 - MeasureText(TextFormat("Nivel Actual: %d / 5", lvl), 20)/2, 180, 20, WHITE);
            
            if (lvl == 5) DrawText("¡¡ NIVEL DE JEFE !!", 400 - MeasureText("¡¡ NIVEL DE JEFE !!", 25)/2, 220, 25, RED);
            
            if (GuiButton((Rectangle){ 300, 300, 200, 50 }, "COMENZAR BATALLA") && !locked) {
                BloquearInput(0.8f);
                Jugador* rival = GenerarRivalTorre(lvl);
                IniciarBatalla(j, rival);
                
                int ganaste = HayPokemonsVivos(j);
                if (ganaste) {
                    if (lvl < 5) {
                        int premio = lvl * 500;
                        j->monedas += premio;
                        WaitMs(1500);
                    } else {
                        j->gemas += 5;
                        WaitMs(2000);
                        free(rival);
                        return;
                    }
                } else {
                    WaitMs(2000);
                    free(rival);
                    return;
                }
                free(rival);
                break;
            }
            if (GuiButton((Rectangle){ 300, 380, 200, 50 }, "ABANDONAR") && !locked) {
                BloquearInput(0.8f);
                return;
            }
            
            EndDrawing();
        }
    }
}
