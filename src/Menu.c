#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Assets.h"
#include "Utils.h"
#include "Batalla.h"
#include "Torre.h"
#include "Guardado.h"
#include "Misiones.h"

// ─── MostrarMenuPrincipal ───────────────────────────────────────────────────
bool MostrarMenuPrincipal(Jugador* jugador) {
    EsperarSoltarMouse();

    while (!WindowShouldClose()) {
        float btnW = SW() * 0.22f; // Reducido para mayor elegancia
        float btnH = SH() * 0.055f; // Reducido para evitar que sea muy pesado
        float bx   = CX() - btnW / 2.0f;
        float gap  = btnH + SH() * 0.012f;
        float startY = SH() * 0.28f;

        BeginDrawing();
        ClearBackground(BLACK);

        // Fondo
        DrawBackgroundExt(texMenuBG, 1.0f);

        // Panel opaco central más estilizado
        float panelW = btnW + 60;
        float panelH = gap * 9 + 40; // Ajustado para un botón más (Log)
        float panelX = CX() - panelW / 2.0f;
        float panelY = startY - 20;
        DrawRectangleRounded((Rectangle){panelX, panelY, panelW, panelH}, 0.1f, 10, (Color){10,10,25,200});
        DrawRectangleRoundedLines((Rectangle){panelX, panelY, panelW, panelH}, 0.1f, 10, 2, (Color){255,215,0,100});

        // Título con más sombra y elegancia
        float titleSz = SH() * 0.1f;
        if (titleSz < 34) titleSz = 34;
        int tsz = (int)titleSz;
        DrawText("Z-POKEMON", (int)(CX() - MeasureText("Z-POKEMON", tsz)/2.0f + 2), (int)(startY - tsz - 28 + 2), tsz, DARKGRAY);
        DrawText("Z-POKEMON", (int)(CX() - MeasureText("Z-POKEMON", tsz)/2.0f), (int)(startY - tsz - 28), tsz, GOLD);
        DrawText("v1.0.3-DEBUG", (int)(CX() + MeasureText("Z-POKEMON", tsz)/2.0f - 80), (int)(startY - 25), 14, GRAY);

        // Info jugador refinada
        DrawText(TextFormat("Entrenador: %s", jugador->nombre), (int)(panelX + 15), (int)(panelY + 10), 16, WHITE);
        DrawText(TextFormat("$ %ld", jugador->monedas), (int)(panelX + panelW - 100), (int)(panelY + 10), 16, GOLD);

        bool done = false;
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();

        // BOTONES (con Input Lock)
        float y = startY + 30;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "POKEDEX") && !locked)           { BloquearInput(0.8f); MostrarPokedex(); }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "INICIAR COMBATE") && !locked)   { BloquearInput(0.8f); ConfigurarRivalMenu(jugador); }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "MODO TORRE") && !locked)        { BloquearInput(0.8f); IniciarTorre(jugador); }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "MISIONES") && !locked)          { BloquearInput(0.8f); MostrarMenuMisiones(jugador); }
        y += gap + SH()*0.01f; // separador visual

        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "MI EQUIPO") && !locked)         { BloquearInput(0.8f); VerPokemons(jugador); }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "TIENDA") && !locked)            { BloquearInput(0.8f); MostrarTienda(jugador); }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "GUARDAR PARTIDA") && !locked)   { BloquearInput(0.8f); GuardarPartida(jugador); }
        y += gap + SH()*0.01f;

        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "SALIR") && !locked)             { done = true; BloquearInput(0.8f); }

        EndDrawing();
        if (done) return false;
    }
    return true;
}

// ─── MostrarPokedex ─────────────────────────────────────────────────────────
void MostrarPokedex() {
    EsperarSoltarMouse();
    int categoria = 0;
    const char* nombres[] = { "Bulbasaur", "Charmander", "Squirtle" };

    bool done = false;
    while (!WindowShouldClose() && !done) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        BeginDrawing();
        ClearBackground((Color){12,12,40,255});
        DrawBackgroundExt(texMenuBG, 0.2f);

        DrawText("Z-POKEDEX", (int)(CX() - MeasureText("Z-POKEDEX", 34)/2), 20, 34, YELLOW);

        float tabW = SW() * 0.22f;
        if (GuiButton((Rectangle){ CX()-tabW*1.5f, SH()*0.12f, tabW, 38 }, "POKEMONS") && !locked)   { BloquearInput(0.3f); categoria = 0; }
        if (GuiButton((Rectangle){ CX()-tabW*0.5f, SH()*0.12f, tabW, 38 }, "MOVIMIENTOS") && !locked) { BloquearInput(0.3f); categoria = 1; }
        if (GuiButton((Rectangle){ CX()+tabW*0.5f, SH()*0.12f, tabW, 38 }, "OBJETOS") && !locked)    { BloquearInput(0.3f); categoria = 2; }

        DrawRectangle((int)(SW()*0.05f), (int)(SH()*0.22f), (int)(SW()*0.9f), (int)(SH()*0.65f), (Color){0,0,0,160});

        if (categoria == 0) {
            float cardW = SW() * 0.26f;
            float cardH = SH() * 0.55f;
            for (int i = 0; i < 3; i++) {
                float cx = SW() * 0.14f + i * (cardW + SW()*0.03f);
                float cy = SH() * 0.23f;
                DrawRectangleRounded((Rectangle){cx, cy, cardW, cardH}, 0.08f, 8, (Color){20,20,50,200});
                DrawRectangleRoundedLines((Rectangle){cx, cy, cardW, cardH}, 0.08f, 8, 2, DARKGRAY);
                DrawPokemonInRect(i, (Rectangle){cx+10, cy+10, cardW-20, cardH-60}, false, WHITE);
                DrawText(TextFormat("#%03d  %s", i+1, nombres[i]),
                         (int)(cx + cardW/2 - MeasureText(TextFormat("#%03d  %s", i+1, nombres[i]), 18)/2),
                         (int)(cy + cardH - 48), 18, WHITE);
            }
        } else {
            DrawText("Contenido proximamente...", (int)(CX()-MeasureText("Contenido proximamente...", 20)/2), (int)(SH()*0.5f), 20, GRAY);
        }

        if (GuiButton((Rectangle){ CX()-100, SH()*0.92f, 200, 44 }, "VOLVER") && !locked) { done = true; BloquearInput(0.8f); }
        EndDrawing();
    }
}

// Redundant local MostrarMisiones implementation removed. Call MostrarMenuMisiones instead.


// ─── ElegirStarterMouse ─────────────────────────────────────────────────────
Pokemon ElegirStarterMouse() {
    EsperarSoltarMouse();
    int seleccionado = -1;
    const char* nombres[] = { "Bulbasaur",  "Charmander", "Squirtle" };
    const char* tipos[]   = { "Planta",      "Fuego",      "Agua"    };
    Color colores[]       = { GREEN,         ORANGE,       SKYBLUE   };

    while (!WindowShouldClose()) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawBackgroundExt(texIntroBG, 0.6f);

        int titleSz = (int)(SH() * 0.05f);
        if (titleSz < 22) titleSz = 22;
        DrawText("ELIGE TU COMPAÑERO INICIAL", (int)(CX() - MeasureText("ELIGE TU COMPAÑERO INICIAL", titleSz)/2), (int)(SH()*0.05f), titleSz, WHITE);

        float cardW = SW() * 0.24f; float cardH = SH() * 0.62f;
        float totalW = 3 * cardW + 2 * SW() * 0.03f;
        float startX = CX() - totalW / 2.0f;

        for (int i = 0; i < 3; i++) {
            float cx = startX + i * (cardW + SW() * 0.03f);
            float cy = SH() * 0.16f;
            Rectangle rec = { cx, cy, cardW, cardH };
            bool hover = CheckCollisionPointRec(GetMousePosition(), rec);
            DrawRectangleRounded(rec, 0.1f, 10, (Color){15,15,35,220});
            if (hover) { DrawRectangleRounded(rec, 0.1f, 10, (Color){colores[i].r, colores[i].g, colores[i].b, 40}); }
            DrawRectangleRoundedLines(rec, 0.1f, 10, hover ? 3 : 1, hover ? colores[i] : DARKGRAY);
            DrawPokemonInRect(i, (Rectangle){ cx + cardW*0.05f, cy + 10, cardW*0.9f, cardH*0.7f }, false, WHITE);
            int nameSize = (int)(SH() * 0.032f); if (nameSize < 16) nameSize = 16;
            DrawText(nombres[i], (int)(cx + cardW/2 - MeasureText(nombres[i], nameSize)/2), (int)(cy + cardH - nameSize*2 - 18), nameSize, colores[i]);
            DrawText(TextFormat("Tipo: %s", tipos[i]), (int)(cx + cardW/2 - MeasureText(TextFormat("Tipo: %s", tipos[i]), 14)/2), (int)(cy + cardH - 22), 14, LIGHTGRAY);
            if (hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !locked) { seleccionado = i; BloquearInput(0.8f); }
        }
        if (seleccionado == 0) { EndDrawing(); return crearBulbasaur(); }
        if (seleccionado == 1) { EndDrawing(); return crearCharmander(); }
        if (seleccionado == 2) { EndDrawing(); return crearSquirtle(); }
        EndDrawing();
    }
    return crearBulbasaur();
}

// ─── MostrarTienda ──────────────────────────────────────────────────────────
void MostrarTienda(Jugador* jugador) {
    EsperarSoltarMouse();
    bool done = false;
    while (!WindowShouldClose() && !done) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        BeginDrawing();
        ClearBackground((Color){8,8,20,255});
        DrawBackgroundExt(texMenuBG, 0.15f);
        int titleSz = (int)(SH() * 0.07f); if (titleSz < 26) titleSz = 26;
        DrawText("Z-TIENDA", (int)(CX() - MeasureText("Z-TIENDA", titleSz)/2), (int)(SH()*0.04f), titleSz, GOLD);
        char infoTxt[128]; sprintf(infoTxt, "Monedas: %ld      Gemas: %d", jugador->monedas, jugador->gemas);
        DrawText(infoTxt, (int)(CX() - MeasureText(infoTxt, 20)/2), (int)(SH()*0.14f), 20, YELLOW);
        DrawLineEx((Vector2){SW()*0.1f, SH()*0.2f}, (Vector2){SW()*0.9f, SH()*0.2f}, 1.5f, DARKGRAY);
        float btnW = SW() * 0.4f; float btnH = SH() * 0.072f;
        float bx   = CX() - btnW / 2.0f; float gap  = btnH + SH() * 0.015f;
        float y = SH() * 0.23f;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "Pocion           100 monedas") && !locked) { if (jugador->monedas >= 100) { jugador->monedas -= 100; BloquearInput(0.3f); } }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "Super Pocion     300 monedas") && !locked) { if (jugador->monedas >= 300) { jugador->monedas -= 300; BloquearInput(0.3f); } }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "Revivir          500 monedas") && !locked) { if (jugador->monedas >= 500) { jugador->monedas -= 500; BloquearInput(0.3f); } }
        y += gap;
        if (GuiButton((Rectangle){ bx, y, btnW, btnH }, "Gema de Jefe   2000 monedas") && !locked) { if (jugador->monedas >= 2000) { jugador->monedas -= 2000; jugador->gemas++; BloquearInput(0.3f); } }
        DrawLineEx((Vector2){SW()*0.1f, SH()*0.76f}, (Vector2){SW()*0.9f, SH()*0.76f}, 1.5f, DARKGRAY);
        float vbW = SW() * 0.22f;
        if (GuiButton((Rectangle){ CX()-vbW/2, SH()*0.83f, vbW, btnH }, "VOLVER") && !locked) { done = true; BloquearInput(0.8f); }
        EndDrawing();
    }
}

// ─── VerPokemons ────────────────────────────────────────────────────────────
void VerPokemons(Jugador* jugador) {
    EsperarSoltarMouse();
    bool done = false;
    while (!WindowShouldClose() && !done) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawBackgroundExt(texMenuBG, 0.15f);
        DrawText("MI EQUIPO", (int)(CX()-MeasureText("MI EQUIPO",32)/2), (int)(SH()*0.03f), 32, YELLOW);
        float rowH  = SH() * 0.11f; float rowY0 = SH() * 0.12f;
        float rowW  = SW() * 0.88f; float rowX  = SW() * 0.06f;
        for (int i = 0; i < jugador->cantidadPokemons; i++) {
            Pokemon* p = &jugador->pokemons[i]; float ry = rowY0 + i * (rowH + SH()*0.012f);
            DrawRectangleRounded((Rectangle){rowX, ry, rowW, rowH}, 0.08f, 8, (Color){15,15,40,210});
            DrawRectangleRoundedLines((Rectangle){rowX, ry, rowW, rowH}, 0.08f, 8, 1, DARKGRAY);
            DrawPokemonInRect(p->id, (Rectangle){rowX+4, ry+2, rowH-4, rowH-4}, false, WHITE);
            float tx = rowX + rowH + 10;
            DrawText(p->nombre, (int)tx, (int)(ry + rowH*0.12f), 20, WHITE);
            DrawText(TextFormat("Niv. %d", p->nivel), (int)(tx + rowW*0.35f), (int)(ry + rowH*0.12f), 18, YELLOW);
            float vidaPct = (p->vidaMax > 0) ? (float)p->vida / p->vidaMax : 0.0f;
            DrawText(TextFormat("HP  %d / %d", p->vida, p->vidaMax), (int)tx, (int)(ry + rowH*0.55f), 16, LIGHTGRAY);
            DibujarBarraVida((int)(tx + rowW*0.28f), (int)(ry + rowH*0.56f), (int)(rowW*0.55f), (int)(rowH*0.2f), vidaPct);
        }
        float vbW = SW() * 0.22f;
        if (GuiButton((Rectangle){ CX()-vbW/2, SH()*0.91f, vbW, 44 }, "VOLVER") && !locked) { done = true; BloquearInput(0.8f); }
        EndDrawing();
    }
}
