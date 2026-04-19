#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Jugador.h"
#include "Pokemon.h"
#include "Mouse.h"
#include "Menu.h"
#include "Guardado.h"
#include "Intro.h"
#include "Utils.h"
#include "Batalla.h"

#ifdef ANDROID
#include <SDL.h>
#define main SDL_main
#endif

#ifdef _WIN32
void IniciarVentanaWindows();
#endif

int main(int argc, char* argv[]){
#ifdef _WIN32
  SetConsoleOutputCP(65001); // UTF-8
  SetConsoleCP(65001);
  IniciarVentanaWindows();
#endif
  
  MostrarIntro();
  
  desactivarMouse();
  Jugador* jugador = NULL;
  int modoNuevo = 0;

  if (ExistePartida()) {
      inicializarMouse();
      while (1) {
          BorrarPantallaCompleta();
          ImprimirCentrado(5, "--- POKEMON C: MENU DE INICIO ---", 11);
          
          int cx = 40;
          MoverCursor(cx - 20, 10); CambiarColor(10); printf("[ CONTINUAR PARTIDA ]");
          MoverCursor(cx + 2, 10);  CambiarColor(14); printf("[ NUEVA PARTIDA ]");
          
          MouseState mouse = obtenerMouseState();
          if (mouseEnRango(cx - 20, 10, cx - 1, 10, mouse)) {
              jugador = CargarPartida();
              if (jugador != NULL) break;
          }
          if (mouseEnRango(cx + 2, 10, cx + 18, 10, mouse)) {
              modoNuevo = 1;
              break;
          }
          ProcesarMensajes();
          Sleep(50);
      }
  } else {
      modoNuevo = 1;
  }

  if (modoNuevo) {
      char nombre[255] = "";
      BorrarPantallaCompleta();
      
      DibujarCajaUTF8(10, 5, 60, 10, 11);
      MoverCursor(15, 8); 
      EscribirLento("¡BIENVENIDO ENTRENADOR! ESCRIBE TU NOMBRE: ", 30);
      
      int pos = 0;
      while (pos < 20) {
          ProcesarMensajes();
          uint32_t c = GetLastChar();
          if (c == '\r' || c == '\n') {
              if (pos > 0) break;
              else continue;
          }
          if (c == '\b' && pos > 0) {
              pos--;
              nombre[pos] = '\0';
              MoverCursor(GetVirtualCursorX() - 1, GetVirtualCursorY()); 
              printf(" "); 
              MoverCursor(GetVirtualCursorX() - 1, GetVirtualCursorY());
          } else if (c >= 32 && c < 127) {
              nombre[pos++] = (char)c;
              nombre[pos] = '\0';
              printf("%c", (char)c);
          }
          Sleep(20);
      }

      if (strlen(nombre) == 0) strcpy(nombre, "Joaco");

      BorrarPantallaCompleta();
      inicializarMouse();
      Pokemon inicial = ElegirStarterMouse();
      jugador = crearJugador(nombre, 1);
      agregarPokemon(jugador, inicial);
      GuardarPartida(jugador);
  }
  
  MostrarMenuPrincipal(jugador);

#ifdef _WIN32
  while(1) ProcesarMensajes();
#endif
  
  return 0;
}
