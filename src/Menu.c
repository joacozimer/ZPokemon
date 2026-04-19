#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Menu.h"
#include "Mouse.h"
#include "Pokemon.h"
#include "Guardado.h"
#include "Batalla.h"
#include "Utils.h"
#include "Misiones.h"
#include "Registro.h"
#include "Torre.h"

#define COLOR_DEFAULT 7
#define COLOR_TITULO 11 
#define COLOR_BOTON 10  
#define COLOR_SALIR 12  
#define COLOR_TEXTO 15  
#define COLOR_ORO 14

int GetCentroX() {
    return 40; // Midpoint of our 80-column virtual terminal
}

int CalcularAnchoDisplay(const char* texto) {
    int ancho = 0;
    while (*texto) {
        if ((*texto & 0xc0) != 0x80) ancho++;
        texto++;
    }
    return ancho;
}

// BorrarPantallaCompleta ahora está en Utils.c

void ImprimirCentrado(int y, const char* texto, int color) {
    int ancho = CalcularAnchoDisplay(texto);
    int x = GetCentroX() - (ancho / 2);
    MoverCursor(x, y);
    CambiarColor(color);
    printf("%s", texto);
    CambiarColor(COLOR_DEFAULT);
}

void DibujarBotonCentrado(int y, const char* texto, int color) {
    char buffer[255];
    sprintf(buffer, "[ %-10s ]", texto);
    ImprimirCentrado(y, buffer, color);
}

void MostrarMenuPrincipal(Jugador* jugador) {
    BorrarPantallaCompleta();
    int cx = GetCentroX();
    while (1) {
        MoverCursor(0, 0);
        DibujarCajaUTF8(cx - 40, 1, 80, 22, 11);
        
        ImprimirCentrado(3, "◈  ZPOKEMON  ◈", 11);
        ImprimirCentrado(4, "────────────────────────────────────────────────────────", 8);
        
        char buffer[255];
        sprintf(buffer, "ENTRENADOR: %-12s ║ NIVEL: %d", jugador->nombre, jugador->nivel);
        ImprimirCentrado(6, buffer, 15);
        
        MoverCursor(cx - 30, 8); CambiarColor(14); printf("MONEDAS: %-8ld", jugador->monedas);
        MoverCursor(cx + 5, 8); CambiarColor(13); printf("GEMAS: %-5d", jugador->gemas);
        
        ImprimirCentrado(10, "-------------------- MENU --------------------", 8);

        int bXLeft = cx - 25;
        int bXRight = cx + 5;
        
        MoverCursor(bXLeft, 12); CambiarColor(10); printf("[ INICIAR COMBATE ]");
        MoverCursor(bXRight, 12); CambiarColor(11); printf("[ MIS POKEMONS    ]");
        
        MoverCursor(bXLeft, 14); CambiarColor(14); printf("[ TIENDA POKEMON  ]");
        MoverCursor(bXRight, 14); CambiarColor(13); printf("[ MODO TORRE      ]");
        
        MoverCursor(bXLeft, 16); CambiarColor(13); printf("[ POKEDEX         ]");
        MoverCursor(bXRight, 16); CambiarColor(12); printf("[ GUARDAR Y SALIR ]");
        MoverCursor(cx - 10, 18); CambiarColor(8);  printf("[ CONFIGURACION   ]");

        MouseState mouse = obtenerMouseState();
        
        if (mouseEnRango(bXLeft, 12, bXLeft + 19, 12, mouse)) {
            ConfigurarRivalMenu(jugador);
            BorrarPantallaCompleta();
        } else if (mouseEnRango(bXRight, 12, bXRight + 19, 12, mouse)) {
            VerPokemons(jugador);
            BorrarPantallaCompleta();
        } else if (mouseEnRango(bXLeft, 14, bXLeft + 19, 14, mouse)) {
            MostrarTienda(jugador);
            BorrarPantallaCompleta();
        } else if (mouseEnRango(bXRight, 14, bXRight + 19, 14, mouse)) {
            IniciarTorre(jugador);
            BorrarPantallaCompleta();
        } else if (mouseEnRango(bXLeft, 16, bXLeft + 19, 16, mouse)) {
            MostrarPokedex();
            BorrarPantallaCompleta();
        } else if (mouseEnRango(bXRight, 16, bXRight + 19, 16, mouse)) {
            GuardarPartida(jugador);
            ImprimirCentrado(22, "¡PROGRESO GUARDADO CON ÉXITO!", 10);
            Sleep(800);
            BorrarPantallaCompleta();
            printf("Saliendo del juego... ¡Hasta la próxima, Entrenador!\n");
            exit(0);
        } else if (mouseEnRango(cx - 10, 18, cx + 10, 18, mouse)) {
            MostrarConfiguracion(jugador);
            BorrarPantallaCompleta();
        }
        Sleep(50);
    }
}

Pokemon ElegirStarterMouse() {
    BorrarPantallaCompleta();
    while (1) {
        ImprimirCentrado(2, "=== ELIGE TU POKEMON INICIAL ===", COLOR_TITULO);
        
        int cx = GetCentroX();
        
        // BULBASAUR BOX (Izquierda)
        DibujarCajaUTF8(cx - 38, 6, 22, 6, 10);
        MoverCursor(cx - 32, 8); CambiarColor(15); printf("BULBASAUR");
        MoverCursor(cx - 36, 9); ImprimirTipoBadge(PLANTA); printf(" "); ImprimirTipoBadge(VENENO);

        // CHARMANDER BOX (Centro)
        DibujarCajaUTF8(cx - 11, 6, 22, 6, 12);
        MoverCursor(cx - 5, 8); CambiarColor(15); printf("CHARMANDER");
        MoverCursor(cx - 4, 9); ImprimirTipoBadge(FUEGO);

        // SQUIRTLE BOX (Derecha)
        DibujarCajaUTF8(cx + 15, 6, 22, 6, 9);
        MoverCursor(cx + 22, 8); CambiarColor(15); printf("SQUIRTLE");
        MoverCursor(cx + 23, 9); ImprimirTipoBadge(AGUA);

        MouseState mouse = obtenerMouseState();
        if (mouseEnRango(cx - 38, 6, cx - 16, 12, mouse)) return crearBulbasaur();
        if (mouseEnRango(cx - 11, 6, cx + 11, 12, mouse)) return crearCharmander();
        if (mouseEnRango(cx + 15, 6, cx + 37, 12, mouse)) return crearSquirtle();

        Sleep(50);
    }
}

void CompactarEquipo(Jugador* j) {
    Pokemon nuevoEquipo[6];
    memset(nuevoEquipo, 0, sizeof(nuevoEquipo));
    int count = 0;
    for(int i=0; i<6; i++) {
        if(j->pokemons[i].nombre[0] != '\0') {
            nuevoEquipo[count++] = j->pokemons[i];
        }
    }
    memcpy(j->pokemons, nuevoEquipo, sizeof(nuevoEquipo));
    j->cantidadPokemons = count;
}

void VerPokemons(Jugador* jugador) {
    BorrarPantallaCompleta();
    int cx = GetCentroX();
    int selIdx = -1; 
    
    while(1) {
        MoverCursor(0, 0);
        ImprimirCentrado(1, "=== GESTION DE POKEMONS ===", COLOR_TITULO);
        ImprimirCentrado(2, "(Haz clic en dos para intercambiarlos)", 8);

        MoverCursor(cx - 39, 4); CambiarColor(11); printf("MI EQUIPO");
        for (int i = 0; i < 6; i++) {
            MoverCursor(cx - 39, 6 + i);
            if (selIdx == i) CambiarColor(14);
            else if (i < jugador->cantidadPokemons) CambiarColor(10);
            else CambiarColor(8);

            if (i < jugador->cantidadPokemons && jugador->pokemons[i].nombre[0] != '\0') {
                printf("%d. %-10s ", i+1, jugador->pokemons[i].nombre);
            } else {
                printf("%d. [ VACIO ] ", i+1);
            }
        }

        // PC COLUMNA 1
        MoverCursor(cx - 13, 4); CambiarColor(11); printf("PC (1-15)");
        for (int i = 0; i < 15; i++) {
            MoverCursor(cx - 13, 6 + i);
            int idx = i + 6;
            if (selIdx == idx) CambiarColor(14);
            else if (i < jugador->cantidadPC) CambiarColor(7);
            else CambiarColor(8);
            if(i < jugador->cantidadPC && jugador->pc[i].nombre[0] != '\0') {
                printf("%2d. %-8s ", i+1, jugador->pc[i].nombre);
            } else printf("%2d. ---     ", i+1);
        }

        // PC COLUMNA 2
        MoverCursor(cx + 17, 4); CambiarColor(11); printf("PC (16-30)");
        for (int i = 0; i < 15; i++) {
            MoverCursor(cx + 17, 6 + i);
            int idx = i + 15 + 6;
            if (selIdx == idx) CambiarColor(14);
            else if ((i+15) < jugador->cantidadPC) CambiarColor(7);
            else CambiarColor(8);
            if((i+15) < jugador->cantidadPC && jugador->pc[i+15].nombre[0] != '\0') {
                printf("%2d. %-8s ", i+16, jugador->pc[i+15].nombre);
            } else printf("%2d. ---     ", i+16);
        }

        ImprimirCentrado(23, "[ VOLVER ]", 12);
        
        MouseState m = obtenerMouseState();
        if (mouseEnRango(cx - 5, 23, cx + 5, 23, m)) break;

        // LOGICA DE INTERCAMBIO
        for(int i=0; i<6; i++) {
            if(mouseEnRango(cx-39, 6+i, cx-20, 6+i, m)) {
                if(selIdx == -1) selIdx = i;
                // ... (rest of logic)
                if(selIdx == -1) selIdx = i;
                else {
                    if(selIdx != i) {
                        if(selIdx < 6) { 
                            Pokemon temp = jugador->pokemons[i];
                            jugador->pokemons[i] = jugador->pokemons[selIdx];
                            jugador->pokemons[selIdx] = temp;
                        } else { 
                            int pcIdx = selIdx - 6;
                            if (pcIdx < jugador->cantidadPC) {
                                if (jugador->cantidadPokemons == 1 && jugador->pc[pcIdx].nombre[0] == '\0' && i == 0) {
                                     ImprimirCentrado(24, "¡ERROR: No puedes vaciar el equipo!", 12);
                                     Sleep(800); MoverCursor(0,24); printf("                                     ");
                                } else {
                                    Pokemon temp = jugador->pokemons[i];
                                    jugador->pokemons[i] = jugador->pc[pcIdx];
                                    jugador->pc[pcIdx] = temp;
                                }
                            }
                        }
                    }
                    CompactarEquipo(jugador);
                    selIdx = -1;
                    Sleep(200);
                }
            }
        }
        for(int i=0; i<30; i++) {
            int col = (i < 15) ? -15 : 25;
            int row = (i < 15) ? i : i - 15;
            if(mouseEnRango(cx+col, 6+row, cx+col+15, 6+row, m)) {
                if(selIdx == -1) selIdx = i + 6;
                else {
                    if(selIdx != (i + 6)) {
                        if(selIdx >= 6) { 
                             int idx1 = selIdx - 6;
                             int idx2 = i;
                             if(idx1 < jugador->cantidadPC && idx2 < jugador->cantidadPC) {
                                 Pokemon temp = jugador->pc[idx1];
                                 jugador->pc[idx1] = jugador->pc[idx2];
                                 jugador->pc[idx2] = temp;
                             }
                        } else { 
                            int eqIdx = selIdx;
                            int pcIdx = i;
                            if (eqIdx < jugador->cantidadPokemons) {
                                if (jugador->cantidadPokemons == 1 && jugador->pc[pcIdx].nombre[0] == '\0') {
                                    ImprimirCentrado(24, "¡ERROR: No puedes vaciar el equipo!", 12);
                                    Sleep(800); MoverCursor(0,24); printf("                                     ");
                                } else {
                                    Pokemon temp = jugador->pc[pcIdx];
                                    jugador->pc[pcIdx] = jugador->pokemons[eqIdx];
                                    jugador->pokemons[eqIdx] = temp;
                                }
                            }
                        }
                    }
                    CompactarEquipo(jugador);
                    // Actualizar cantidadPC
                    int cPC = 0;
                    for(int k=0; k<30; k++) if(jugador->pc[k].nombre[0] != '\0') cPC = k+1;
                    jugador->cantidadPC = (cPC > jugador->cantidadPC) ? cPC : jugador->cantidadPC; // Mantener slots si se desea
                    
                    selIdx = -1;
                    Sleep(200);
                }
            }
        }
        Sleep(50);
    }
}

void MostrarTienda(Jugador* j) {
    BorrarPantallaCompleta();
    while (1) {
        MoverCursor(0, 0);
        ImprimirCentrado(2, "=== TIENDA POKEMON ===", 14);
        MoverCursor(2, 4); CambiarColor(14); printf("Monedas: %-10ld", j->monedas);
        MoverCursor(2, 5); CambiarColor(13); printf("Gemas: %-5d", j->gemas);

        DibujarBotonCentrado(10, "POKEMON SALVAJE (1000)", 14);
        DibujarBotonCentrado(15, "VOLVER", 12);

        MouseState m = obtenerMouseState();
        int cx = GetCentroX() - 8;
        if (mouseEnRango(cx, 10, cx + 15, 10, m)) {
            if (j->monedas >= 1000) {
                j->monedas -= 1000;
                Pokemon p;
                int r = rand() % 3;
                if(r == 0) p = crearBulbasaur(); else if(r == 1) p = crearCharmander(); else p = crearSquirtle();
                InicializarStarters(&p);
                char res[100]; sprintf(res, "¡Ha salido un %s salvaje!", p.nombre);
                ImprimirCentrado(17, res, 10);
                
                if (j->cantidadPokemons < 6) {
                    j->pokemons[j->cantidadPokemons++] = p;
                    ImprimirCentrado(18, "¡Se ha unido a tu EQUIPO!", 10);
                } else if (j->cantidadPC < 30) {
                    j->pc[j->cantidadPC++] = p;
                    ImprimirCentrado(18, "¡Enviado directamente al PC!", 11);
                }
                Sleep(1500);
            } else { ImprimirCentrado(17, "No tienes monedas", 12); Sleep(800); }
        } else if (mouseEnRango(cx, 15, cx + 15, 15, m)) {
            return;
        }
        Sleep(50);
    }
}

void MostrarConfiguracion(Jugador* jugador) {
    BorrarPantallaCompleta();
    int cx = GetCentroX();
    while (1) {
        MoverCursor(0, 0);
        ImprimirCentrado(2, "=== CONFIGURACION ===", 11);
        
        DibujarBotonCentrado(10, "BORRAR PARTIDA", 12);
        DibujarBotonCentrado(15, "VOLVER", 10);
        
        MouseState m = obtenerMouseState();
        int btnX = cx - 8;
        
        if (mouseEnRango(btnX, 10, btnX + 15, 10, m)) {
            BorrarPantallaCompleta();
            while (1) {
                MoverCursor(0, 0);
                ImprimirCentrado(10, "¿ESTÁS SEGURO DE QUE QUIERES BORRAR TU PARTIDA?", 12);
                ImprimirCentrado(12, "[ SÍ, BORRAR ]    [ NO, VOLVER ]", 15);
                
                MouseState m2 = obtenerMouseState();
                if (mouseEnRango(cx - 16, 12, cx - 2, 12, m2)) {
                    BorrarPartida();
                    printf("Partida borrada. Cerrando...\n");
                    exit(0);
                }
                if (mouseEnRango(cx + 2, 12, cx + 16, 12, m2)) break;
                Sleep(50);
                ProcesarMensajes();
            }
            BorrarPantallaCompleta();
        } else if (mouseEnRango(btnX, 15, btnX + 15, 15, m)) {
            return;
        }
        
        ProcesarMensajes();
        Sleep(50);
    }
}

void MostrarPokedex() {
    int cx = GetCentroX();
    while (1) {
        BorrarPantallaCompleta();
        ImprimirCentrado(2, "=== Z-POKEDEX ===", 11);
        
        DibujarBotonCentrado(8, "POKEMONS", 10);
        DibujarBotonCentrado(10, "MOVIMIENTOS", 11);
        DibujarBotonCentrado(12, "OBJETOS", 14);
        DibujarBotonCentrado(15, "VOLVER", 12);
        
        MouseState m = obtenerMouseState();
        int btnX = cx - 8;
        
        if (mouseEnRango(btnX, 8, btnX + 15, 8, m)) {
            // LISTA POKEMONS
            BorrarPantallaCompleta();
            while(1) {
                ImprimirCentrado(1, "--- POKEMONS ---", 10);
                for(int i=0; i<GetTotalPokemons(); i++) {
                    Pokemon p = GetPokemonPorIndice(i);
                    MoverCursor(cx - 20, 4 + i);
                    CambiarColor(15); printf("%d. %-12s ", i+1, p.nombre);
                    ImprimirTipoBadge(p.tipo[0]);
                }
                ImprimirCentrado(20, "[ VOLVER ]", 12);
                MouseState m2 = obtenerMouseState();
                if (mouseEnRango(cx-5, 20, cx+5, 20, m2)) break;
                Sleep(50); ProcesarMensajes();
            }
        } else if (mouseEnRango(btnX, 10, btnX + 15, 10, m)) {
            // LISTA MOVIMIENTOS
            BorrarPantallaCompleta();
            int offset = 0;
            while(1) {
                BorrarPantallaCompleta();
                ImprimirCentrado(1, "--- MOVIMIENTOS ---", 11);
                for(int i=offset; i<offset+15 && i<GetTotalMovimientos(); i++) {
                    Movimiento mov = GetMovimientoPorIndice(i);
                    MoverCursor(5, 3 + (i-offset));
                    CambiarColor(14); printf("%-15s", mov.nombre);
                    CambiarColor(7); printf(" | Pot: %3d | Acc: %3d | PP: %2d", mov.potencia, mov.precision, mov.ppMax);
                }
                ImprimirCentrado(20, "[ VOLVER ]", 12);
                if (offset > 0) MoverCursor(5, 19), printf("^ ANTERIOR");
                if (offset + 15 < GetTotalMovimientos()) MoverCursor(65, 19), printf("v SIGUIENTE");

                MouseState m2 = obtenerMouseState();
                if (mouseEnRango(cx-5, 20, cx+5, 20, m2)) break;
                if (mouseEnRango(5, 19, 15, 19, m2) && offset > 0) { offset -= 15; Sleep(200); }
                if (mouseEnRango(65, 19, 75, 19, m2) && offset + 15 < GetTotalMovimientos()) { offset += 15; Sleep(200); }
                Sleep(50); ProcesarMensajes();
            }
        } else if (mouseEnRango(btnX, 12, btnX + 15, 12, m)) {
            // LISTA OBJETOS
            BorrarPantallaCompleta();
            while(1) {
                ImprimirCentrado(1, "--- OBJETOS ---", 14);
                for(int i=0; i<GetTotalObjetos(); i++) {
                    Objeto o = GetObjetoPorIndice(i);
                    MoverCursor(5, 4 + i*2);
                    CambiarColor(14); printf("%-12s: ", o.nombre);
                    CambiarColor(7); printf("%s", o.descripcion);
                }
                ImprimirCentrado(20, "[ VOLVER ]", 12);
                MouseState m2 = obtenerMouseState();
                if (mouseEnRango(cx-5, 20, cx+5, 20, m2)) break;
                Sleep(50); ProcesarMensajes();
            }
        } else if (mouseEnRango(btnX, 15, btnX + 15, 15, m)) {
            return;
        }
        
        ProcesarMensajes();
        Sleep(50);
    }
}


