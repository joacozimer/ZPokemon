#include "Intro.h"
#include "Utils.h"
#include "Assets.h"
#include "raygui.h"
#include <math.h>

void MostrarIntro() {
    // Fase 1: Fade-in del splash screen
    float alpha = 0.0f;
    while (alpha < 1.0f && !WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);

        // Fondo de intro con fade-in
        DrawBackgroundExt(texLoadingBG, alpha);

        // Título con fade-in
        int tsz = (int)(SH() * 0.1f);
        if (tsz < 30) tsz = 30;
        DrawText("ZPOKEMON",
                 (int)(CX() - MeasureText("ZPOKEMON", tsz)/2),
                 (int)(SH() * 0.35f), tsz, Fade(YELLOW, alpha));

        int ssz = (int)(SH() * 0.04f);
        if (ssz < 18) ssz = 18;
        DrawText("LA AVENTURA COMIENZA",
                 (int)(CX() - MeasureText("LA AVENTURA COMIENZA", ssz)/2),
                 (int)(SH() * 0.52f), ssz, Fade(WHITE, alpha));

        EndDrawing();
        alpha += 0.008f;
        if (GetKeyPressed() != 0) break;
    }

    // Fase 2: Esperar input del jugador (con parpadeo)
    bool done = false;
    while (!WindowShouldClose() && !done) {
        ActualizarInputLock(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);

        DrawBackgroundExt(texLoadingBG, 1.0f);

        int tsz = (int)(SH() * 0.1f);
        if (tsz < 30) tsz = 30;
        DrawText("ZPOKEMON",
                 (int)(CX() - MeasureText("ZPOKEMON", tsz)/2),
                 (int)(SH() * 0.3f), tsz, YELLOW);

        int ssz = (int)(SH() * 0.038f);
        if (ssz < 16) ssz = 16;
        DrawText("LA AVENTURA COMIENZA",
                 (int)(CX() - MeasureText("LA AVENTURA COMIENZA", ssz)/2),
                 (int)(SH() * 0.48f), ssz, WHITE);

        // Subtítulo parpadeante y llamativo (Dorado)
        float pulse = (float)((sin(GetTime() * 4.0) + 1.0) / 2.0);
        int psz = (int)(SH() * 0.035f);
        if (psz < 18) psz = 18;
        
        Color goldPulse = { 0 };
        goldPulse.r = (unsigned char)(255);
        goldPulse.g = (unsigned char)(200 + 55 * pulse); // Oscila entre 200 y 255
        goldPulse.b = (unsigned char)(0);
        goldPulse.a = (unsigned char)(150 + 105 * pulse); // Oscila entre 150 y 255
        
        DrawText("PRESIONA CUALQUIER TECLA PARA COMENZAR",
                 (int)(CX() - MeasureText("PRESIONA CUALQUIER TECLA PARA COMENZAR", psz)/2),
                 (int)(SH() * 0.75f), psz, goldPulse);

        EndDrawing();
        
        // El usuario quiere que empiece con cualquier tecla o clic
        if (GetKeyPressed() != 0 || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            done = true;
        }
    }

    EsperarSoltarMouse(); // Evita el click-through al primer menú
    BorrarPantallaCompleta();
}
