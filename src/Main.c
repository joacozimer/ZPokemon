#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Jugador.h"
#include "Pokemon.h"
#include "Menu.h"
#include "Guardado.h"
#include "Intro.h"
#include "Utils.h"
#include "Batalla.h"
#include "Assets.h"
#include "raylib.h"
#include "raygui.h"

int main(int argc, char* argv[]){
  // Inicialización de Raylib
  InitWindow(800, 600, "ZPokemon V1.0.3 - Graphical Edition");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
  InitGameStyle(); // Aplicar estilo global (tipografía y colores)
  CargarAssets();
  
  // 1. Mostrar Intro
  MostrarIntro();
  
  Jugador* jugador = NULL;
  int modoNuevo = 0;

  // 2. Selección de Partida
  if (ExistePartida()) {
      while (!WindowShouldClose()) {
          ActualizarInputLock(GetFrameTime());
          bool locked = IsInputLocked();
          BeginDrawing();
          ClearBackground(BLACK);
          
          DrawBackgroundExt(texStartBG, 1.0f);
          
          // Panel opaco para que los botones sobresalgan
          float btnW = SW() * 0.25f;
          float btnH = SH() * 0.08f;
          float panelW = btnW + 100;
          float panelH = btnH * 2 + 150;
          DrawRectangleRounded((Rectangle){CX() - panelW/2, CY() - panelH/2, panelW, panelH}, 0.1f, 10, (Color){0,0,0,160});
          DrawRectangleRoundedLines((Rectangle){CX() - panelW/2, CY() - panelH/2, panelW, panelH}, 0.1f, 10, 2, YELLOW);

          float titleSz = SH() * 0.08f;
          if (titleSz < 30) titleSz = 30;
          DrawText("Z-POKEMON", (int)(CX() - MeasureText("Z-POKEMON", (int)titleSz)/2), (int)(CY() - panelH/2 + 30), (int)titleSz, YELLOW);
          
          if (GuiButton((Rectangle){ CX() - btnW/2, CY() - 20, btnW, btnH }, "CONTINUAR PARTIDA") && !locked) {
              jugador = CargarPartida();
              if (jugador != NULL) {
                  BloquearInput(0.5f);
                  EsperarSoltarMouse();
                  break;
              }
          }
          if (GuiButton((Rectangle){ CX() - btnW/2, CY() + btnH + 10, btnW, btnH }, "NUEVA PARTIDA") && !locked) {
              modoNuevo = 1;
              BloquearInput(0.5f);
              EsperarSoltarMouse();
              break;
          }
          
          EndDrawing();
      }
  } else {
      modoNuevo = 1;
  }

  // 3. Crear Nueva Partida
  if (modoNuevo) {
      char nombre[255] = "";
      int pos = 0;
      
      while (!WindowShouldClose()) {
          ActualizarInputLock(GetFrameTime());
          bool locked = IsInputLocked();
          BeginDrawing();
          ClearBackground(BLACK);
          DrawBackgroundExt(texStartBG, 0.8f);

          float panelW = SW() * 0.6f;
          float panelH = SH() * 0.5f;
          DrawRectangleRounded((Rectangle){CX() - panelW/2, CY() - panelH/2, panelW, panelH}, 0.1f, 10, (Color){0,0,0,180});
          DrawRectangleRoundedLines((Rectangle){CX() - panelW/2, CY() - panelH/2, panelW, panelH}, 0.1f, 10, 2, SKYBLUE);
          
          int bannerSz = (int)(SH() * 0.05f); if (bannerSz < 20) bannerSz = 20;
          DrawText("¡BIENVENIDO ENTRENADOR!", (int)(CX() - MeasureText("¡BIENVENIDO ENTRENADOR!", bannerSz)/2), (int)(CY() - panelH/2 + 40), bannerSz, SKYBLUE);
          DrawText("ESCRIBE TU NOMBRE:", (int)(CX() - MeasureText("ESCRIBE TU NOMBRE:", 20)/2), (int)(CY() - 40), 20, WHITE);
          
          float inputW = 300; float inputH = 50;
          DrawRectangle((int)(CX() - inputW/2), (int)(CY() + 10), (int)inputW, (int)inputH, DARKGRAY);
          DrawText(nombre, (int)(CX() - inputW/2 + 10), (int)(CY() + 10 + 15), 20, YELLOW);
          
          int key = GetCharPressed();
          while (key > 0) {
              if ((key >= 32) && (key <= 125) && (pos < 20)) {
                  nombre[pos++] = (char)key;
                  nombre[pos] = '\0';
              }
              key = GetCharPressed();
          }
          if (IsKeyPressed(KEY_BACKSPACE) && pos > 0) {
              pos--;
              nombre[pos] = '\0';
          }
          
          if (pos > 0 && GuiButton((Rectangle){ CX() - 75, CY() + 100, 150, 40 }, "CONFIRMAR") && !locked) {
              BloquearInput(0.5f);
              EsperarSoltarMouse();
              break;
          }
          
          EndDrawing();
      }

      if (strlen(nombre) == 0) strcpy(nombre, "Joaco");

      Pokemon inicial = ElegirStarterMouse();
      jugador = crearJugador(nombre, 1);
      agregarPokemon(jugador, inicial);
      GuardarPartida(jugador);
  }
  
  // 4. Bucle Principal (Menú)
  if (jugador != NULL) {
      while (!WindowShouldClose()) {
          ActualizarInputLock(GetFrameTime());
          if (!MostrarMenuPrincipal(jugador)) break;
      }
  }

  // Finalización
  CerrarAssets();
  CloseWindow();
  
  return 0;
}
