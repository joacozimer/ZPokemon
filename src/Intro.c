#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Intro.h"
#include "Utils.h"

void MostrarIntro() {
    BorrarPantallaCompleta();
    
    const char* logo[] = {
        "                                  .''.       ",
        "        .''.             ..       :o_o:      ",
        "        :o_o:      .'.  : o :  .'.  :'''.    ",
        "       : ' ' :    : o :  '..' : o :  '...'   ",
        "        '...'      '..'        '..'          ",
        "             _______  _______  ___   _  _______  __   __  _______  __    _ ",
        "            |       ||       ||   | | ||       ||  |_|  ||       ||  |  | |",
        "            |    _  ||   _   ||   |_| ||    ___||       ||   _   ||   |_| |",
        "            |   |_| ||  | |  ||      _||   |___ |       ||  | |  ||       |",
        "            |    ___||  |_|  ||     |_ |    ___||       ||  |_|  ||  _    |",
        "            |   |    |       ||    _  ||   |___ | ||_|| ||       || | |   |",
        "            |___|    |_______||___| |_||_______||_|   |_||_______||_|  |__|",
        "                                                                           ",
        "                               ◈  ZPOKEMON  ◈                               "
    };

    int lineas = sizeof(logo) / sizeof(logo[0]);
    
    // Animación de aparición
    for (int i = 0; i < lineas; i++) {
        MoverCursor(1, 4 + i);
        CambiarColor(11); // Cyan
        if (i >= 5 && i <= 11) CambiarColor(14); // Yellow for the logo
        printf("%s", logo[i]);
        Sleep(50);
    }

    CambiarColor(15);
    MoverCursor(20, 20);
    EscribirLento(">>> Presiona cualquier tecla para continuar <<<", 20);
    
    // Esperar input (si es posible con Utils.c)
    // En este motor solemos usar GetLastChar o similar
    while(1) {
        ProcesarMensajes();
        if (GetLastChar() != 0) break;
        Sleep(30);
    }
    
    BorrarPantallaCompleta();
}
