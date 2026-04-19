#include "Mouse.h"
#include "Utils.h"
#include <stdio.h>
#include <string.h>

#ifdef ANDROID
#include <SDL.h>

void inicializarMouse() {}
void desactivarMouse() {}
void ocultarCursor() {}

// obtenerMouseState, EsperarSueltaMouse implementados en Renderer_Android.c

#elif defined(_WIN32)
#include <windows.h>

static HANDLE hInput;

void inicializarMouse() {
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

void desactivarMouse() {
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
}

void ocultarCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(hOut, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);
}

MouseState obtenerMouseState() {
    ProcesarMensajes();
    MouseState state = {0, 0, 0};
    
    HWND hWnd = FindWindow("PokemonGDI", NULL);
    if (hWnd) {
        POINT p;
        RECT rect;
        if (GetCursorPos(&p) && GetClientRect(hWnd, &rect)) {
            ScreenToClient(hWnd, &p);
            int tileW = rect.right / 80;
            int tileH = rect.bottom / 25;
            if (tileW < 1) tileW = 1;
            if (tileH < 1) tileH = 1;
            int offsetX = (rect.right - (80 * tileW)) / 2;
            int offsetY = (rect.bottom - (25 * tileH)) / 2;

            if (p.x >= 0 && p.y >= 0) {
                state.x = (p.x - offsetX) / tileW;
                state.y = (p.y - offsetY) / tileH;
                if (state.x < 0) state.x = 0; if (state.x >= 80) state.x = 79;
                if (state.y < 0) state.y = 0; if (state.y >= 25) state.y = 24;
                
                if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                    state.clicked = 1;
                    return state;
                }
            }
        }
    }
    return state; // Si no hay ventana GDI, retornar estado vacío
}

void EsperarSueltaMouse() {
    while (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        ProcesarMensajes();
        Sleep(10);
    }
    Sleep(30); // Pequeño margen extra para evitar que el siguiente frame lo detecte
}



#else
// Linux / Termux
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static struct termios orig;

void inicializarMouse() {
    tcgetattr(STDIN_FILENO, &orig);
    struct termios raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf("\033[?1000h\033[?1006h\033[?25l");
    fflush(stdout);
    int f = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, f | O_NONBLOCK);
}

void desactivarMouse() {
    printf("\033[?1000l\033[?1006l\033[?25h");
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

void ocultarCursor() { printf("\033[?25l"); fflush(stdout); }

MouseState obtenerMouseState() {
    MouseState state = {0, 0, 0};
    char buf[32];
    int n = read(STDIN_FILENO, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = 0;
        if (strncmp(buf, "\033[<", 3) == 0) {
            int b, x, y; char t;
            if (sscanf(buf + 3, "%d;%d;%d%c", &b, &x, &y, &t) == 4) {
                state.x = x - 1; state.y = y - 1;
                if (t == 'M' && b == 0) state.clicked = 1;
            }
        }
    }
    return state;
}
#endif

int mouseEnRango(int x1, int y1, int x2, int y2, MouseState state) {
    return (state.clicked && state.x >= x1 && state.x <= x2 && state.y >= y1 && state.y <= y2);
}
