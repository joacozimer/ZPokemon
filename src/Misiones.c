#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Misiones.h"
#include "Utils.h"
#include "Mouse.h"
#include "Menu.h"

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
    
    // Check max level in team
    int maxLvlEquipo = 0;
    for(int i=0; i<j->cantidadPokemons; i++) if(j->pokemons[i].nivel > maxLvlEquipo) maxLvlEquipo = j->pokemons[i].nivel;

    switch(idx) {
        case 0: return j->stats.maxNivelVencido >= 5;
        case 1: return j->monedas >= 500;
        case 2: return maxLvlEquipo >= 10;
        case 3: return j->stats.vitoriaTipoFuego == 1;
        case 4: return j->cantidadPokemons >= 3;
        case 5: return j->stats.victoriaAguaSoloAgua == 1; // Aun no implementado full detection pero el flag existe
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
    int cx = GetCentroX();

    while(1) {
        MoverCursor(0, 0);
        DibujarCajaUTF8(cx - 39, 1, 78, 23, 11);
        ImprimirCentrado(3, "=== PANEL DE MISIONES ===", 11);

        for (int i = 0; i < 10; i++) {
            MoverCursor(cx - 37, 5 + i);
            int color = (misiones[i].dificultad == FACIL ? 10 : (misiones[i].dificultad == MEDIO ? 14 : 12));
            
            CambiarColor(color);
            printf("[%s] ", (misiones[i].dificultad == FACIL ? "FAC" : (misiones[i].dificultad == MEDIO ? "MED" : "DIF")));
            
            if (misiones[i].completada) CambiarColor(8); else CambiarColor(15);
            printf("%-15s ", misiones[i].nombre);
            
            CambiarColor(7);
            printf("║ %-35s ", misiones[i].descripcion);
            
            if (misiones[i].completada) {
                CambiarColor(10); printf(" [ COMPLETA ] ");
            } else if (EsMisionCumplida(i, j)) {
                CambiarColor(11); printf(" [ RECLAMAR ] ");
            } else {
                CambiarColor(14); printf("  $%d  ", misiones[i].recompensa);
            }
        }

        ImprimirCentrado(19, "Haz misiones para ganar dinero y ser el mejor.", 15);
        ImprimirCentrado(22, "[ VOLVER AL MENÚ ]", 14);

        MouseState m = obtenerMouseState();
        if (mouseEnRango(cx - 10, 22, cx + 10, 22, m)) return;

        // Lógica de Reclamo (Detectar clic en la zona derecha)
        for(int i=0; i<10; i++) {
            if (mouseEnRango(cx + 25, 5+i, cx+40, 5+i, m) && !misiones[i].completada && EsMisionCumplida(i, j)) {
                misiones[i].completada = 1;
                j->monedas += misiones[i].recompensa;
                ImprimirCentrado(21, "¡RECOMPENSA RECLAMADA CON ÉXITO!", 10);
                Sleep(800);
            }
        }
        Sleep(60);
    }
}
