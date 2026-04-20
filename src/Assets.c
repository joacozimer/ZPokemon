#include "Assets.h"
#include "Utils.h"
#include "recursos.h"
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

// Prototipo del wrapper (definido en Win32Resource.c)
int GetResourceData(int resourceId, const char* resourceType, void** outData, unsigned int* outSize);

// Texturas globales
Texture2D texBulbasaur;
Texture2D texCharmander;
Texture2D texSquirtle;
Texture2D texPikachu;
Texture2D texEevee;
Texture2D texMew;
Texture2D texBattleBG;
Texture2D texMenuBG;
Texture2D texIntroBG;
Texture2D texStartBG;
Texture2D texLoadingBG;

// Helper para cargar una textura desde los recursos embebidos del .exe
static Texture2D LoadTextureFromResource(int resourceId) {
    void* pData = NULL;
    unsigned int size = 0;

    int err = GetResourceData(resourceId, NULL, &pData, &size);
    if (err != 0) {
        LogDebug("[ERROR] No se pudo encontrar recurso %d. WinErr: %d (%s)", resourceId, err, GetWinErrorStr(err));
        return (Texture2D){ 0 };
    }

    unsigned char* b = (unsigned char*)pData;
    if (size >= 8) {
        LogDebug("[LOG] Recurso %d: %u bytes. Cabecera: %02X %02X %02X %02X", resourceId, size, b[0], b[1], b[2], b[3]);
    }

    // Ahora que los archivos son PNG reales gracias al conversor GDI+,
    // podemos cargarlos directamente desde la memoria del ejecutable de forma segura.
    Image img = LoadImageFromMemory(".png", (unsigned char*)pData, size);
    
    if (img.data == NULL) {
        LogDebug("[ERROR] Raylib no pudo decodificar el recurso %d despus de la conversion a PNG.", resourceId);
        return (Texture2D){ 0 };
    }
    
    LogDebug("[LOG] Recurso %d decodificado con exito: %dx%d", resourceId, img.width, img.height);
    
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    if (tex.id == 0) LogDebug("[ERROR] Fallo al subir textura %d a la GPU.", resourceId);
    else LogDebug("[LOG] Textura %d lista (ID: %u)", resourceId, tex.id);
    
    return tex;
}

static void DibujarPantallaCarga(float progreso, const char* detalle) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    // Si ya cargamos el fondo, lo usamos
    if (texLoadingBG.id > 0) {
        float scale = (float)GetScreenWidth() / (float)texLoadingBG.width;
        if ((float)GetScreenHeight() / (float)texLoadingBG.height > scale) scale = (float)GetScreenHeight() / (float)texLoadingBG.height;
        
        DrawTextureEx(texLoadingBG, (Vector2){0,0}, 0, scale, WHITE);
        // Oscurecer un poco el fondo para que se vea la barra
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0,0,0,100});
    }
    
    float barW = GetScreenWidth() * 0.6f;
    float barH = 25;
    Rectangle barRect = { GetScreenWidth()/2.0f - barW/2.0f, GetScreenHeight() * 0.8f, barW, barH };
    
    // Dibujar borde de la barra manualmente para más estilo
    DrawRectangleLinesEx((Rectangle){barRect.x - 2, barRect.y - 2, barRect.width + 4, barRect.height + 4}, 2, GOLD);
    GuiProgressBar(barRect, NULL, TextFormat("%d%%", (int)(progreso * 100)), &progreso, 0, 1);
    
    DrawText(detalle, (int)barRect.x, (int)barRect.y - 30, 20, WHITE);
    
    EndDrawing();
}

void CargarAssets() {
    printf("[LOG] Iniciando CargarAssets con pantalla de carga...\n");
    
    // 1. Cargar el fondo de carga primero para poder mostrar algo
    texLoadingBG = LoadTextureFromResource(ID_LOADING_BG);
    
    const int total = 11;
    int actual = 0;
    
    #define ACTUALIZAR_CARGA(nombre) actual++; DibujarPantallaCarga((float)actual/total, nombre);
    
    DibujarPantallaCarga(0.05f, "Iniciando sistema de archivos...");
    
    texBulbasaur  = LoadTextureFromResource(ID_BULBASAUR);  ACTUALIZAR_CARGA("Cargando Bulbasaur...");
    texCharmander = LoadTextureFromResource(ID_CHARMANDER); ACTUALIZAR_CARGA("Cargando Charmander...");
    texSquirtle   = LoadTextureFromResource(ID_SQUIRTLE);   ACTUALIZAR_CARGA("Cargando Squirtle...");
    texPikachu    = LoadTextureFromResource(ID_PIKACHU);    ACTUALIZAR_CARGA("Cargando Pikachu...");
    texEevee      = LoadTextureFromResource(ID_EEVEE);      ACTUALIZAR_CARGA("Cargando Eevee...");
    texMew        = LoadTextureFromResource(ID_MEW);        ACTUALIZAR_CARGA("Cargando Mew...");
    
    texBattleBG   = LoadTextureFromResource(ID_BATTLE_BG);  ACTUALIZAR_CARGA("Cargando Fondo de Batalla...");
    texMenuBG     = LoadTextureFromResource(ID_MENU_BG);    ACTUALIZAR_CARGA("Cargando Menu Principal...");
    texIntroBG    = LoadTextureFromResource(ID_INTRO_BG);   ACTUALIZAR_CARGA("Cargando Introduccion...");
    texStartBG    = LoadTextureFromResource(ID_START_BG);   ACTUALIZAR_CARGA("Cargando Pantalla de Inicio...");
    
    // Ya cargamos texLoadingBG al principio, así que solo marcamos completado
    actual++; 
    DibujarPantallaCarga(1.0f, "¡Carga completada!");
    WaitTime(0.2); // Pequeña pausa para que se vea el 100%
    
    if (texBulbasaur.id == 0) {
        RegistrarError("Fallo critico: No se encontraron los recursos embebidos en el .exe");
    }
}

void CerrarAssets() {
    UnloadTexture(texBulbasaur);
    UnloadTexture(texCharmander);
    UnloadTexture(texSquirtle);
    UnloadTexture(texPikachu);
    UnloadTexture(texEevee);
    UnloadTexture(texMew);
    UnloadTexture(texBattleBG);
    UnloadTexture(texMenuBG);
    UnloadTexture(texIntroBG);
    UnloadTexture(texStartBG);
    UnloadTexture(texLoadingBG);
}

Texture2D GetPokemonTexture(int id) {
    if (id == 0) return texBulbasaur;
    if (id == 1) return texCharmander;
    if (id == 2) return texSquirtle;
    if (id == 3) return texPikachu;
    if (id == 4) return texEevee;
    if (id == 5) return texMew;
    return texBulbasaur;
}

// Dibuja un pokemon escalado para encajar dentro de la Rectangle indicada,
// respetando el aspect ratio y centrandolo dentro del recuadro.
void DrawPokemonInRect(int id, Rectangle destRect, bool flip, Color tint) {
    Texture2D tex = GetPokemonTexture(id);
    if (tex.id == 0 || tex.width == 0 || tex.height == 0) {
        DrawRectangleRec(destRect, (Color){60,60,80,180});
        return;
    }

    float scaleX = destRect.width  / (float)tex.width;
    float scaleY = destRect.height / (float)tex.height;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;

    float drawW = tex.width  * scale;
    float drawH = tex.height * scale;
    float drawX = destRect.x + (destRect.width  - drawW) / 2.0f;
    float drawY = destRect.y + (destRect.height - drawH) / 2.0f;

    Rectangle src  = { 0, 0, (float)tex.width * (flip ? -1 : 1), (float)tex.height };
    Rectangle dst  = { drawX, drawY, drawW, drawH };
    Vector2   orig = { 0, 0 };
    DrawTexturePro(tex, src, dst, orig, 0.0f, tint);
}
