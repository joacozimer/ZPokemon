#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#ifdef ANDROID
#include <SDL.h>
#endif
#include "Utils.h"

#define MAX_ERRORES 10
#define MAX_MSG_LEN 255

static char errores[MAX_ERRORES][MAX_MSG_LEN];
static int numErrores = 0;

void RegistrarError(const char* msg) {
    if (numErrores < MAX_ERRORES) {
        strncpy(errores[numErrores], msg, MAX_MSG_LEN);
        numErrores++;
    }
}

void ImprimirErrores() {
    if (numErrores == 0) return;
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, 12); // Rojo
#else
    printf("\033[1;31m");
#endif
    printf("\n--- ERRORES DETECTADOS ---\n");
    for (int i = 0; i < numErrores; i++) {
        printf("[!] %s\n", errores[i]);
    }
    printf("--------------------------\n");
#ifdef _WIN32
    SetConsoleTextAttribute(hOut, 7); // Reset Color
#else
    printf("\033[0m");
#endif
}

void LimpiarErrores() {
    numErrores = 0;
}

int HayErrores() {
    return numErrores > 0;
}

void EscribirLento(const char* texto, int ms) {
    for (int i = 0; texto[i] != '\0'; i++) {
        printf("%c", texto[i]);
        fflush(stdout);
#ifdef _WIN32
        ProcesarMensajes(); 
        Sleep(ms);
#else
        usleep(ms * 1000);
#endif
    }
}

void ImprimirTipoBadge(tipoPokemon t) {
    if (t == VACIO) return;

    static const char* nombres[] = {
        "VACIO", "ACERO", "VOLADOR", "AGUA", "HIELO", "PLANTA", "BICHO", 
        "ELECTRICO", "NORMAL", "ROCA", "TIERRA", "FUEGO", "LUCHA", 
        "HADA", "PSIQUICO", "VENENO", "DRAGON", "FANTASMA", "SINIESTRO"
    };

    int color = 7;
    int ansi = 0;
    switch(t) {
        case FUEGO: color = 12; ansi = 31; break;
        case AGUA: color = 9; ansi = 34; break;
        case PLANTA: color = 10; ansi = 32; break;
        case ELECTRICO: color = 14; ansi = 33; break;
        case HIELO: color = 11; ansi = 36; break;
        case VENENO: color = 13; ansi = 35; break;
        case LUCHA: color = 4; ansi = 31; break;
        case TIERRA: color = 6; ansi = 33; break;
        case VOLADOR: color = 3; ansi = 36; break;
        case PSIQUICO: color = 13; ansi = 35; break;
        case BICHO: color = 2; ansi = 32; break;
        case ROCA: color = 8; ansi = 37; break;
        case FANTASMA: color = 5; ansi = 35; break;
        case DRAGON: color = 1; ansi = 34; break;
        case ACERO: color = 8; ansi = 37; break;
        case HADA: color = 13; ansi = 35; break;
        default: color = 7; ansi = 0; break;
    }

    CambiarColor(color);
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
    printf("[%s]", nombres[t]);
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif
    CambiarColor(7);
}

void BorrarPantallaCompleta() {
    ClearVirtualBuffer(); // Limpiar el buffer de la ventana propia
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
}

void LimpiarArea(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        MoverCursor(x, y + i);
        for (int j = 0; j < w; j++) printf(" ");
    }
}

void MoverCursor(int x, int y) {
    UpdateVirtualCursor(x, y); // Siempre actualizar el buffer virtual
#ifdef _WIN32
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#elif !defined(ANDROID)
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
#endif
}

void CambiarColor(int color) {
    UpdateVirtualColor(color); // Siempre actualizar el buffer virtual
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
#elif !defined(ANDROID)
    // Mapeo simple de colores Windows a ANSI
    int ansi = 37;
    switch(color) {
        case 12: ansi = 31; break; // Rojo
        case 10: ansi = 32; break; // Verde
        case 14: ansi = 33; break; // Amarillo
        case 9:  ansi = 34; break; // Azul
        case 13: ansi = 35; break; // Magent
        case 11: ansi = 36; break; // Cyan
        default: ansi = 37; break;
    }
    if (color == 7) printf("\033[0m");
    else printf("\033[1;%dm", ansi);
#endif
}

void DibujarCajaUTF8(int x, int y, int w, int h, int color) {
    CambiarColor(color);
    MoverCursor(x, y); printf("┌"); for(int i=0; i<w-2; i++) printf("─"); printf("┐");
    for(int i=1; i<h-1; i++) {
        MoverCursor(x, y+i); printf("│");
        MoverCursor(x+w-1, y+i); printf("│");
    }
    MoverCursor(x, y+h-1); printf("└"); for(int i=0; i<w-2; i++) printf("─"); printf("┘");
    CambiarColor(7);
}
