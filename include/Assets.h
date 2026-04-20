#ifndef Assets_H
#define Assets_H

#include "raylib.h"
#include "Pokemon.h"

// Texturas globales
extern Texture2D texBulbasaur;
extern Texture2D texCharmander;
extern Texture2D texSquirtle;
extern Texture2D texPikachu;
extern Texture2D texEevee;
extern Texture2D texMew;
extern Texture2D texBattleBG;
extern Texture2D texMenuBG;
extern Texture2D texIntroBG;
extern Texture2D texStartBG;
extern Texture2D texLoadingBG;

void CargarAssets();
void CerrarAssets();
Texture2D GetPokemonTexture(int id);

// Dibuja el sprite de un pokemon escalado para caber en un Rectangle
void DrawPokemonInRect(int id, Rectangle destRect, bool flip, Color tint);

#endif
