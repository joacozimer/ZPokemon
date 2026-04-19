#ifdef ANDROID
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include "Utils.h"

static uint32_t lastChar_android = 0;

uint32_t GetLastChar() {
    uint32_t c = lastChar_android;
    lastChar_android = 0;
    return c;
}

void EsperarSueltaMouse() {
    // En Android el input es por eventos, no hay que esperar
    SDL_Delay(80);
}

static SDL_Window*   gWindow   = NULL;
static SDL_Renderer* gRenderer = NULL;
static TTF_Font*     gFont     = NULL;

void RenderVirtualTerminal() {
    SDL_Color colors[] = {
        {0,0,0,255}, {0,0,128,255}, {0,128,0,255}, {0,128,128,255},
        {128,0,0,255}, {128,0,128,255}, {128,128,0,255}, {192,192,192,255},
        {128,128,128,255}, {0,0,255,255}, {0,255,0,255}, {0,255,255,255},
        {255,0,0,255}, {255,0,255,255}, {255,255,0,255}, {255,255,255,255}
    };

    if (!gRenderer || !gFont) return;

    int w, h;
    SDL_GetWindowSize(gWindow, &w, &h);
    int charW = w / 80;
    int charH = h / 25;
    if (charW < 1) charW = 1;
    if (charH < 1) charH = 1;

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            uint32_t cp = GetVirtualChar(x, y);
            if (cp != ' ' && cp != 0) {
                SDL_Surface* surf = TTF_RenderGlyph_Blended(gFont, (Uint16)cp, colors[GetVirtualColor(x, y) % 16]);
                if (surf) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer, surf);
                    SDL_Rect rect = {x * charW, y * charH, charW, charH};
                    SDL_RenderCopy(gRenderer, tex, NULL, &rect);
                    SDL_DestroyTexture(tex);
                    SDL_FreeSurface(surf);
                }
            }
        }
    }
    SDL_RenderPresent(gRenderer);
}

// Mouse state para Android
static int  g_mx = 0, g_my = 0, g_clicked = 0;

#include "Mouse.h"
MouseState obtenerMouseState() {
    ProcesarMensajes();
    MouseState s = {g_mx, g_my, g_clicked};
    g_clicked = 0;
    return s;
}

int ProcesarMensajes() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_KEYDOWN) {
            SDL_Keycode k = e.key.keysym.sym;
            if (k == SDLK_RETURN || k == SDLK_KP_ENTER) lastChar_android = '\r';
            else if (k == SDLK_BACKSPACE) lastChar_android = '\b';
            else if (k >= 32 && k < 127) lastChar_android = (uint32_t)k;
        }
        if (e.type == SDL_TEXTINPUT) {
            lastChar_android = (uint32_t)(unsigned char)e.text.text[0];
        }
        if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_FINGERDOWN) {
            int mx, my;
            int w, h;
            SDL_GetWindowSize(gWindow, &w, &h);
            if (e.type == SDL_MOUSEBUTTONDOWN) { mx = e.button.x; my = e.button.y; }
            else { mx = (int)(e.tfinger.x * w); my = (int)(e.tfinger.y * h); }
            int charW = w / 80; if (charW < 1) charW = 1;
            int charH = h / 25; if (charH < 1) charH = 1;
            g_mx = mx / charW;
            g_my = my / charH;
            g_clicked = 1;
        }
    }
    RenderVirtualTerminal();
    return 1;
}

// Android usa SDL_main
int SDL_main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    gWindow = SDL_CreateWindow("Pokemon RPG", 0, 0, 960, 600, SDL_WINDOW_FULLSCREEN_DESKTOP);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gFont = TTF_OpenFont("assets/terminal.ttf", 20);
    if (!gFont) { SDL_Log("ERROR: No se pudo cargar la fuente!"); }

    extern int main(int, char**);
    main(argc, argv);

    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
#endif
