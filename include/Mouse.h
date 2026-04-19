#ifndef Mouse_H
#define Mouse_H

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    int x;
    int y;
    int clicked;
} MouseState;

void inicializarMouse();
void desactivarMouse();
MouseState obtenerMouseState();
int mouseEnRango(int x1, int y1, int x2, int y2, MouseState state);
void ocultarCursor();
void EsperarSueltaMouse();

#endif
