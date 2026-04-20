#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Pokemon.h"
#include "Utils.h"
#include "Batalla.h"
#include "Assets.h"
#include "raylib.h"
#include "raygui.h"

typedef enum { BATTLE_CHOOSING, BATTLE_SEQUENCING, BATTLE_RESULTS } BattleState;

void IniciarBatalla(Jugador* jugador, Jugador* rival) {
    int idxMi = 0;
    int idxRival = 0;
    char logMsg[255] = "¡COMIENZA EL COMBATE!";
    float logTimer = 2.0f;
    bool mostrarMoves = false;
    BattleState state = BATTLE_CHOOSING;
    int sequenceStep = 0;
    int attacker1 = 0; // 0: Player, 1: Rival
    int moveIdx1 = 0;
    int moveIdx2 = 0;
    float stepTimer = 0;
    int monedasGano = 0;
    bool batallaTerminada = false;

    ResetearEquipo(jugador);
    for(int i=0; i<jugador->cantidadPokemons; i++) jugador->pokemons[i].vidaVisual = (float)jugador->pokemons[i].vida;
    for(int i=0; i<rival->cantidadPokemons; i++) rival->pokemons[i].vidaVisual = (float)rival->pokemons[i].vida;

    while (!WindowShouldClose()) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        
        Pokemon* miPk = &jugador->pokemons[idxMi];
        Pokemon* rivalPk = &rival->pokemons[idxRival];

        float dt = GetFrameTime();
        if (stepTimer > 0) stepTimer -= dt;
        if (logTimer > 0) logTimer -= dt;

        // Animación de vida fluida
        float miAnimSpeed = (float)miPk->vidaMax / 0.8f;
        float rivalAnimSpeed = (float)rivalPk->vidaMax / 0.8f;

        if (miPk->vidaVisual > (float)miPk->vida) { miPk->vidaVisual -= miAnimSpeed * dt; if (miPk->vidaVisual < (float)miPk->vida) miPk->vidaVisual = (float)miPk->vida; }
        else if (miPk->vidaVisual < (float)miPk->vida) { miPk->vidaVisual += miAnimSpeed * dt; if (miPk->vidaVisual > (float)miPk->vida) miPk->vidaVisual = (float)miPk->vida; }

        if (rivalPk->vidaVisual > (float)rivalPk->vida) { rivalPk->vidaVisual -= rivalAnimSpeed * dt; if (rivalPk->vidaVisual < (float)rivalPk->vida) rivalPk->vidaVisual = (float)rivalPk->vida; }
        else if (rivalPk->vidaVisual < (float)rivalPk->vida) { rivalPk->vidaVisual += rivalAnimSpeed * dt; if (rivalPk->vidaVisual > (float)rivalPk->vida) rivalPk->vidaVisual = (float)rivalPk->vida; }

        bool isAnimating = (fabs(miPk->vidaVisual - (float)miPk->vida) > 0.5f) || (fabs(rivalPk->vidaVisual - (float)rivalPk->vida) > 0.5f);

        // --- LÓGICA DE SECUENCIA ---
        if (state == BATTLE_SEQUENCING && stepTimer <= 0 && !isAnimating) {
            if (sequenceStep == 1) { // Ataque 1
                Pokemon* atk = (attacker1 == 0) ? miPk : rivalPk;
                Pokemon* def = (attacker1 == 0) ? rivalPk : miPk;
                int mv = (attacker1 == 0) ? moveIdx1 : moveIdx2;
                
                float eff = GetEfectividad(atk->movimientos[mv].tipo, def->tipo[0]);
                if (def->tipo[1] != VACIO) eff *= GetEfectividad(atk->movimientos[mv].tipo, def->tipo[1]);
                
                int dano = (int)((atk->movimientos[mv].potencia/5+5)*eff);
                def->vida -= dano;
                if (def->vida < 0) def->vida = 0;

                sprintf(logMsg, "¡%s usó %s!", atk->nombre, atk->movimientos[mv].nombre);
                logTimer = 1.5f;
                stepTimer = 1.6f;
                sequenceStep = 2;
            } 
            else if (sequenceStep == 2) { // Verificar si el defensor de T1 murió
                if (rivalPk->vida <= 0 || miPk->vida <= 0) {
                    sequenceStep = 5; // Saltear al final del turno
                    stepTimer = 0.5f;
                } else {
                    sequenceStep = 3; // Turno 2
                    stepTimer = 0.5f;
                }
            }
            else if (sequenceStep == 3) { // Ataque 2
                int attacker2 = (attacker1 == 0) ? 1 : 0;
                Pokemon* atk = (attacker2 == 0) ? miPk : rivalPk;
                Pokemon* def = (attacker2 == 0) ? rivalPk : miPk;
                int mv = (attacker2 == 0) ? moveIdx1 : moveIdx2;

                float eff = GetEfectividad(atk->movimientos[mv].tipo, def->tipo[0]);
                if (def->tipo[1] != VACIO) eff *= GetEfectividad(atk->movimientos[mv].tipo, def->tipo[1]);

                int dano = (int)((atk->movimientos[mv].potencia/5+5)*eff);
                def->vida -= dano;
                if (def->vida < 0) def->vida = 0;

                sprintf(logMsg, "¡%s usó %s!", atk->nombre, atk->movimientos[mv].nombre);
                logTimer = 1.5f;
                stepTimer = 1.6f;
                sequenceStep = 4;
            }
            else if (sequenceStep == 4 || sequenceStep == 5) { // Procesar bajas
                if (rivalPk->vida <= 0) {
                    if (HayPokemonsVivos(rival)) {
                        while(idxRival < rival->cantidadPokemons && rival->pokemons[idxRival].vida <= 0) idxRival++;
                        sprintf(logMsg, "¡Rival envió a %s!", rival->pokemons[idxRival].nombre);
                        logTimer = 2.0f;
                        stepTimer = 2.2f;
                        state = BATTLE_CHOOSING;
                    } else {
                        monedasGano = rival->pokemons[0].nivel * 50;
                        jugador->monedas += monedasGano;
                        state = BATTLE_RESULTS;
                        batallaTerminada = true;
                    }
                } else if (miPk->vida <= 0) {
                    if (HayPokemonsVivos(jugador)) {
                        while(idxMi < jugador->cantidadPokemons && jugador->pokemons[idxMi].vida <= 0) idxMi++;
                        sprintf(logMsg, "¡Adelante %s!", jugador->pokemons[idxMi].nombre);
                        logTimer = 2.0f;
                        stepTimer = 2.2f;
                        state = BATTLE_CHOOSING;
                    } else {
                        monedasGano = 0;
                        state = BATTLE_RESULTS;
                        batallaTerminada = true;
                    }
                } else {
                    state = BATTLE_CHOOSING;
                }
            }
        }

        if (batallaTerminada && logTimer <= 0 && state == BATTLE_RESULTS) break;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawBackgroundExt(texBattleBG, 1.0f);
        
        float spriteSz = SH() * 0.35f;
        DrawPokemonInRect(miPk->id, (Rectangle){ SW()*0.15f, SH()*0.42f, spriteSz, spriteSz }, false, (miPk->vida <= 0) ? GRAY : WHITE);
        DrawPokemonInRect(rivalPk->id, (Rectangle){ SW()*0.6f, SH()*0.08f, spriteSz, spriteSz }, true, (rivalPk->vida <= 0) ? GRAY : WHITE);
        
        float infoW = SW() * 0.38f; float infoH = SH() * 0.14f;
        // Rival
        DrawRectangleRounded((Rectangle){ SW()*0.52f, SH()*0.02f, infoW, infoH }, 0.2f, 8, Fade(BLACK, 0.7f));
        DrawRectangleRoundedLines((Rectangle){ SW()*0.52f, SH()*0.02f, infoW, infoH }, 0.2f, 8, 2, RED);
        DrawText(rivalPk->nombre, (int)(SW()*0.54f), (int)(SH()*0.04f), 24, WHITE);
        DrawText(TextFormat("Niv. %d", rivalPk->nivel), (int)(SW()*0.82f), (int)(SH()*0.04f), 18, YELLOW);
        DibujarBarraVida((int)(SW()*0.54f), (int)(SH()*0.09f), (int)(infoW*0.9f), 18, rivalPk->vidaVisual / rivalPk->vidaMax);
        // Aliado
        DrawRectangleRounded((Rectangle){ SW()*0.05f, SH()*0.72f, infoW, infoH }, 0.2f, 8, Fade(BLACK, 0.7f));
        DrawRectangleRoundedLines((Rectangle){ SW()*0.05f, SH()*0.72f, infoW, infoH }, 0.2f, 8, 2, SKYBLUE);
        DrawText(miPk->nombre, (int)(SW()*0.07f), (int)(SH()*0.74f), 24, WHITE);
        DrawText(TextFormat("Niv. %d", miPk->nivel), (int)(SW()*0.35f), (int)(SH()*0.74f), 18, YELLOW);
        DrawText(TextFormat("%d / %d", (int)miPk->vidaVisual, miPk->vidaMax), (int)(SW()*0.30f), (int)(SH()*0.82f), 16, WHITE);
        DibujarBarraVida((int)(SW()*0.07f), (int)(SH()*0.79f), (int)(infoW*0.9f), 18, miPk->vidaVisual / miPk->vidaMax);

        float menuW = SW() * 0.52f; float menuH = SH() * 0.24f;
        float menuX = SW() * 0.46f; float menuY = SH() * 0.73f;
        DrawRectangleRounded((Rectangle){ menuX, menuY, menuW, menuH }, 0.1f, 8, Fade(DARKGRAY, 0.9f));
        DrawRectangleRoundedLines((Rectangle){ menuX, menuY, menuW, menuH }, 0.1f, 8, 3, LIGHTGRAY);
        
        if (state == BATTLE_SEQUENCING || isAnimating || logTimer > 0) {
            if (logTimer > 0) {
                // Mensaje centrado en la caja de texto
                DrawText(logMsg, (int)(menuX + menuW/2 - MeasureText(logMsg, 22)/2), (int)(menuY + menuH/2 - 11), 22, WHITE);
            } else {
                DrawText("ESPERANDO...", (int)(menuX + menuW/2 - MeasureText("ESPERANDO...", 20)/2), (int)(menuY + menuH/2 - 10), 20, GRAY);
            }
        } else if (state == BATTLE_CHOOSING) {
            if (!mostrarMoves) {
                float btnW = menuW * 0.44f; float btnH = menuH * 0.36f;
                float gapX = menuW * 0.04f; float gapY = menuH * 0.08f;
                if (GuiButton((Rectangle){ menuX + gapX, menuY + gapY, btnW, btnH }, "ATACAR") && !locked) { BloquearInput(0.4f); mostrarMoves = true; }
                if (GuiButton((Rectangle){ menuX + btnW + gapX*2, menuY + gapY, btnW, btnH }, "MOCHILA") && !locked) {}
                if (GuiButton((Rectangle){ menuX + gapX, menuY + btnH + gapY*2, btnW, btnH }, "POKEMON") && !locked) {}
                if (GuiButton((Rectangle){ menuX + btnW + gapX*2, menuY + btnH + gapY*2, btnW, btnH }, "HUIR") && !locked) { state = BATTLE_RESULTS; batallaTerminada = true; break; }
            } else {
                float btnW = menuW * 0.46f; float btnH = menuH * 0.42f;
                for (int i = 0; i < 4; i++) {
                    float bx = menuX + (i % 2) * (btnW + 6) + 8; float by = menuY + (i / 2) * (btnH + 6) + 8;
                    Rectangle bRec = { bx, by, btnW, btnH };
                    if (miPk->movimientos[i].nombre[0] != '\0') {
                        float eff = GetEfectividad(miPk->movimientos[i].tipo, rivalPk->tipo[0]);
                        if (rivalPk->tipo[1] != VACIO) eff *= GetEfectividad(miPk->movimientos[i].tipo, rivalPk->tipo[1]);
                        
                        bool hover = CheckCollisionPointRec(GetMousePosition(), bRec);
                        Color borderC = GetTipoColor(miPk->movimientos[i].tipo);
                        Color textC = (eff > 1.1f) ? (Color){0, 150, 0, 255} : (eff < 0.9f) ? (Color){200, 0, 0, 255} : BLACK;
                        
                        DrawRectangleRec(bRec, hover ? LIGHTGRAY : (Color){220, 220, 220, 255});
                        DrawRectangleLinesEx(bRec, 3.5f, borderC); // Borde de 3-4px
                        
                        DrawTypeIcon(miPk->movimientos[i].tipo, (Vector2){bx + 8, by + btnH/2 - 12}, 24);
                        DrawText(miPk->movimientos[i].nombre, (int)(bx + 40), (int)(by + btnH/2 - 9), 18, textC);

                        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !locked) {
                            BloquearInput(0.81f);
                            // Inteligencia Rival Optimizada: Busca el mayor daño posible
                            int rivalMV = 0; float maxD = -1;
                            for(int j=0; j<4; j++) {
                                if(rivalPk->movimientos[j].nombre[0] == '\0') continue;
                                float e = GetEfectividad(rivalPk->movimientos[j].tipo, miPk->tipo[0]);
                                if(miPk->tipo[1] != VACIO) e *= GetEfectividad(rivalPk->movimientos[j].tipo, miPk->tipo[1]);
                                float dTemp = (float)rivalPk->movimientos[j].potencia * e;
                                if(dTemp > maxD) { maxD = dTemp; rivalMV = j; }
                            }
                            
                            moveIdx1 = i;
                            moveIdx2 = rivalMV;
                            attacker1 = (miPk->vel > rivalPk->vel) ? 0 : (miPk->vel < rivalPk->vel) ? 1 : GetRandomValue(0,1);
                            
                            state = BATTLE_SEQUENCING;
                            sequenceStep = 1;
                            mostrarMoves = false;
                        }
                    } else { 
                        DrawRectangleRec(bRec, (Color){80, 80, 80, 150}); 
                        DrawText("-", (int)(bx + btnW/2 - 5), (int)(by + btnH/2 - 10), 20, DARKGRAY); 
                    }
                }
                if (IsKeyPressed(KEY_ESCAPE) || (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !locked)) { BloquearInput(0.3f); mostrarMoves = false; }
            }
        }
        EndDrawing();
    }

    // Pantalla de Resultados
    while (!WindowShouldClose()) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawBackgroundExt(texMenuBG, 0.25f);
        
        float rW = SW()*0.65f; float rH = SH()*0.45f;
        Rectangle resRec = { CX()-rW/2, CY()-rH/2, rW, rH };
        DrawRectangleRounded(resRec, 0.1f, 10, (Color){30,35,50,245});
        DrawRectangleRoundedLines(resRec, 0.1f, 10, 3.5f, (monedasGano > 0) ? GOLD : (Color){200, 50, 50, 255});
        
        if (monedasGano > 0) {
            DrawText("¡VICTORIA!", (int)(CX()-MeasureText("¡VICTORIA!", 50)/2), (int)(resRec.y + 50), 50, GOLD);
            DrawText(TextFormat("Has derrotado a %s", rival->nombre), (int)(CX()-MeasureText(TextFormat("Has derrotado a %s", rival->nombre), 22)/2), (int)(CY()-25), 22, WHITE);
            DrawText(TextFormat("Recompensa: %d monedas", monedasGano), (int)(CX()-MeasureText(TextFormat("Recompensa: %d monedas", monedasGano), 24)/2), (int)(CY()+25), 24, YELLOW);
        } else {
            DrawText("¡DERROTA!", (int)(CX()-MeasureText("¡DERROTA!", 50)/2), (int)(resRec.y + 50), 50, RED);
            DrawText("Tu equipo ha sido debilitado.", (int)(CX()-MeasureText("Tu equipo ha sido debilitado.", 22)/2), (int)(CY()-10), 22, LIGHTGRAY);
            DrawText("No has obtenido monedas.", (int)(CX()-MeasureText("No has obtenido monedas.", 20)/2), (int)(CY()+40), 20, GRAY);
        }
        
        float skipY = resRec.y + rH - 60;
        DrawText("Presiona cualquier boton para continuar", (int)(CX()-MeasureText("Presiona cualquier boton para continuar", 20)/2), (int)skipY, 20, (Color){200,200,200,200});
        
        EndDrawing();

        if ((GetKeyPressed() > 0 || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) && !locked) break;
    }
    BorrarPantallaCompleta();
    BloquearInput(0.8f);
}

int HayPokemonsVivos(Jugador* j) {
    if(!j) return 0;
    for(int i=0; i<j->cantidadPokemons; i++) if(j->pokemons[i].vida > 0) return 1;
    return 0;
}
void ResetearEquipo(Jugador* j) {
    if(!j) return;
    for (int i = 0; i < j->cantidadPokemons; i++) {
        j->pokemons[i].vida = j->pokemons[i].vidaMax;
        j->pokemons[i].estado = SALUDABLE;
    }
}
void ConfigurarRivalMenu(Jugador* jugador) { IniciarBatalla(jugador, jugador); }
void PersonalizarRival(Jugador* rival) {}
void MenuBatalla(Jugador* jugador, Jugador* rival) {}
