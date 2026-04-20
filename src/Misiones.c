#include <stdio.h>
#include <string.h>
#include "Misiones.h"
#include "Utils.h"
#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Assets.h"

static Mision misiones[10];
static int m_init = 0;

void InicializarMisiones() {
    if (m_init) return;

    // FACIL (100)
    strcpy(misiones[0].nombre, "Primer Encuentro");
    strcpy(misiones[0].descripcion, "Derrota a un entrenador nivel 5.");
    misiones[0].dificultad = FACIL; misiones[0].recompensa = 100; misiones[0].completada = 0;

    strcpy(misiones[1].nombre, "Ahorrador");
    strcpy(misiones[1].descripcion, "Acumula 500 monedas en total.");
    misiones[1].dificultad = FACIL; misiones[1].recompensa = 150; misiones[1].completada = 0;

    strcpy(misiones[2].nombre, "Entrenador Novato");
    strcpy(misiones[2].descripcion, "Que un Pokemon llegue a nivel 10.");
    misiones[2].dificultad = FACIL; misiones[2].recompensa = 200; misiones[2].completada = 0;

    // MEDIO (500)
    strcpy(misiones[3].nombre, "Fuego Cruzado");
    strcpy(misiones[3].descripcion, "Vence a un equipo de tipo Fuego.");
    misiones[3].dificultad = MEDIO; misiones[3].recompensa = 500; misiones[3].completada = 0;

    strcpy(misiones[4].nombre, "Coleccionista");
    strcpy(misiones[4].descripcion, "Ten 3 Pokemon en tu equipo.");
    misiones[4].dificultad = MEDIO; misiones[4].recompensa = 600; misiones[4].completada = 0;

    strcpy(misiones[5].nombre, "Duelo de Agua");
    strcpy(misiones[5].descripcion, "Vence a un rival usando solo Agua.");
    misiones[5].dificultad = MEDIO; misiones[5].recompensa = 700; misiones[5].completada = 0;

    strcpy(misiones[6].nombre, "Veterano");
    strcpy(misiones[6].descripcion, "Gana 5 combates seguidos.");
    misiones[6].dificultad = MEDIO; misiones[6].recompensa = 800; misiones[6].completada = 0;

    // DIFICIL (2000)
    strcpy(misiones[7].nombre, "Campeon Regional");
    strcpy(misiones[7].descripcion, "Vence un equipo nivel 50.");
    misiones[7].dificultad = DIFICIL; misiones[7].recompensa = 2000; misiones[7].completada = 0;

    strcpy(misiones[8].nombre, "Maestro Pokemon");
    strcpy(misiones[8].descripcion, "Llega nivel 100 con un Pokemon.");
    misiones[8].dificultad = DIFICIL; misiones[8].recompensa = 3000; misiones[8].completada = 0;

    strcpy(misiones[9].nombre, "El Gran Reto");
    strcpy(misiones[9].descripcion, "Derrota al rival de 6 Pokemons.");
    misiones[9].dificultad = DIFICIL; misiones[9].recompensa = 5000; misiones[9].completada = 0;

    m_init = 1;
}

int EsMisionCumplida(int idx, Jugador* j) {
    if (idx < 0 || idx >= 10) return 0;
    
    int maxLvlEquipo = 0;
    for(int i=0; i<j->cantidadPokemons; i++) if(j->pokemons[i].nivel > maxLvlEquipo) maxLvlEquipo = j->pokemons[i].nivel;

    switch(idx) {
        case 0: return j->stats.maxNivelVencido >= 5;
        case 1: return j->monedas >= 500;
        case 2: return maxLvlEquipo >= 10;
        case 3: return j->stats.vitoriaTipoFuego == 1;
        case 4: return j->cantidadPokemons >= 3;
        case 5: return j->stats.victoriaAguaSoloAgua == 1;
        case 6: return j->stats.combatesSeguidos >= 5;
        case 7: return j->stats.maxNivelVencido >= 50;
        case 8: return maxLvlEquipo >= 100;
        case 9: return j->stats.vitoriaRival6v6 == 1;
        default: return 0;
    }
}

void MostrarMenuMisiones(Jugador* j) {
    InicializarMisiones();
    BorrarPantallaCompleta();
    
    while(!WindowShouldClose()) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        
        BeginDrawing();
        ClearBackground(BLACK);
        DrawBackgroundExt(texMenuBG, 0.2f);
        
        float panelW = SW() * 0.85f; float panelH = SH() * 0.85f;
        Rectangle panelRec = { CX() - panelW/2, CY() - panelH/2, panelW, panelH };
        DrawRectangleRounded(panelRec, 0.05f, 10, Fade(BLACK, 0.8f));
        DrawRectangleRoundedLines(panelRec, 0.05f, 10, 3, SKYBLUE);
        
        DrawText("PANEL DE MISIONES", (int)(CX() - MeasureText("PANEL DE MISIONES", 30)/2), (int)(panelRec.y + 20), 30, YELLOW);
        
        float itemH = (panelH - 120) / 10.0f;
        float startY = panelRec.y + 70;
        
        for (int i = 0; i < 10; i++) {
            float y = startY + i * (itemH + 4);
            Rectangle itemRec = { panelRec.x + 20, y, panelW - 40, itemH };
            
            Color diffC = (misiones[i].dificultad == FACIL) ? GREEN : (misiones[i].dificultad == MEDIO) ? ORANGE : RED;
            DrawRectangleRounded(itemRec, 0.2f, 8, Fade(DARKGRAY, 0.4f));
            DrawRectangleRoundedLines(itemRec, 0.2f, 8, 1.5f, Fade(diffC, 0.6f));
            
            // Dificultad tag
            const char* diffStr = (misiones[i].dificultad == FACIL) ? "FACIL" : (misiones[i].dificultad == MEDIO) ? "MEDIO" : "DIFICIL";
            DrawText(diffStr, (int)(itemRec.x + 10), (int)(y + itemH/2 - 8), 16, diffC);
            
            // Nombre y Descripcion
            DrawText(misiones[i].nombre, (int)(itemRec.x + 85), (int)(y + itemH/2 - 10), 19, WHITE);
            DrawText(misiones[i].descripcion, (int)(itemRec.x + 280), (int)(y + itemH/2 - 8), 16, LIGHTGRAY);
            
            // Estado / Recompensa
            if (misiones[i].completada) {
                DrawText("COMPLETA", (int)(itemRec.x + panelW - 150), (int)(y + itemH/2 - 8), 18, GRAY);
            } else if (EsMisionCumplida(i, j)) {
                Rectangle btnRec = { itemRec.x + panelW - 160, y + 5, 110, itemH - 10 };
                if (GuiButton(btnRec, "RECLAMAR") && !locked) {
                    misiones[i].completada = 1;
                    j->monedas += misiones[i].recompensa;
                    BloquearInput(0.3f);
                }
            } else {
                DrawText(TextFormat("$ %d", misiones[i].recompensa), (int)(itemRec.x + panelW - 140), (int)(y + itemH/2 - 8), 18, GOLD);
            }
        }
        
        if (GuiButton((Rectangle){ CX() - 100, panelRec.y + panelH - 45, 200, 35 }, "VOLVER") && !locked) {
            BloquearInput(0.4f);
            EndDrawing();
            break;
        }
        
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE)) break;
    }
}
