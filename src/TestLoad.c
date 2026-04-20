#include "raylib.h"
#include <stdio.h>

int main() {
    InitWindow(100, 100, "Test");
    Image img = LoadImage("assets/bulbasaur.png");
    if (img.data == NULL) {
        printf("[FAIL] No se pudo cargar assets/bulbasaur.png desde el disco.\n");
    } else {
        printf("[SUCCESS] Se cargo assets/bulbasaur.png del disco. Res: %dx%d\n", img.width, img.height);
        UnloadImage(img);
    }
    CloseWindow();
    return 0;
}
