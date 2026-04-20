#include "Utils.h"
#undef printf
#include "raylib.h"
#include "raygui.h"
#include "Assets.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Memoria de la consola virtual
#define COLS 80
#define ROWS 30
static int currentColor = 7;
static char vScreen[COLS][ROWS];
static int vColors[COLS][ROWS];
static int cursorX = 0;
static int cursorY = 0;
static char lastLog[256];

static Color getColorFromCode(int code) {
    switch (code) {
        case 0: return BLACK;
        case 1: return BLUE;
        case 2: return GREEN;
        case 3: return SKYBLUE;
        case 4: return MAROON;
        case 5: return PURPLE;
        case 6: return DARKGREEN;
        case 7: return LIGHTGRAY;
        case 8: return GRAY;
        case 9: return BLUE;
        case 10: return GREEN;
        case 11: return SKYBLUE;
        case 12: return RED;
        case 13: return PINK;
        case 14: return YELLOW;
        case 15: return WHITE;
        default: return WHITE;
    }
}

void BorrarPantallaCompleta() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            vScreen[x][y] = ' ';
            vColors[x][y] = 7;
        }
    }
}

void MoverCursor(int x, int y) {
    if (x >= 0 && x < COLS) cursorX = x;
    if (y >= 0 && y < ROWS) cursorY = y;
}

void CambiarColor(int color) {
    currentColor = color;
}

void TerminalPrint(const char* fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            cursorY++;
            cursorX = 0;
        } else {
            if (cursorX < COLS && cursorY < ROWS) {
                vScreen[cursorX][cursorY] = buffer[i];
                vColors[cursorX][cursorY] = currentColor;
                cursorX++;
            }
        }
    }
}

void DibujarCajaUTF8(int x, int y, int w, int h, int color) {
    for(int i=0; i<w; i++) {
        MoverCursor(x+i, y); TerminalPrint("-");
        MoverCursor(x+i, y+h-1); TerminalPrint("-");
    }
    for(int i=0; i<h; i++) {
        MoverCursor(x, y+i); TerminalPrint("|");
        MoverCursor(x+w-1, y+i); TerminalPrint("|");
    }
}

void LimpiarArea(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (x+j < COLS && y+i < ROWS) {
                vScreen[x+j][y+i] = ' ';
            }
        }
    }
}

void DibujarRectangulo(int x, int y, int w, int h) {
    DrawRectangle(x, y, w, h, DARKGRAY);
}

void RenderRaylibFrame() {
    if (!IsWindowReady()) return;
    BeginDrawing();
    ClearBackground(BLACK);
    
    int charWidth = 10;
    int charHeight = 20;
    
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (vScreen[x][y] != ' ') {
                char s[2] = { vScreen[x][y], '\0' };
                DrawText(s, x * charWidth, y * charHeight, 20, getColorFromCode(vColors[x][y]));
            }
        }
    }
    EndDrawing();
}

void ProcesarMensajes() {
    // Raylib maneja los mensajes internamente
}

void WaitMs(int ms) {
    double startTime = GetTime();
    while (GetTime() - startTime < (double)ms / 1000.0) {
        if (WindowShouldClose()) exit(0);
    }
}

void EscribirLento(const char* texto, int ms) {
    for (int i = 0; texto[i] != '\0'; i++) {
        printf("%c", texto[i]);
        RenderRaylibFrame();
        WaitMs(ms);
    }
}

char GetLastChar() {
    int key = GetCharPressed();
    return (char)key;
}

int GetCentroX() { return COLS / 2; }

void ImprimirCentrado(int y, const char* texto, int color) {
    int x = (COLS - strlen(texto)) / 2;
    MoverCursor(x, y);
    CambiarColor(color);
    TerminalPrint(texto);
}

void DibujarBarraVida(int x, int y, int w, int h, float pct) {
    DrawRectangle(x, y, w, h, DARKGRAY);
    Color c = GREEN;
    if (pct < 0.2f) c = RED;
    else if (pct < 0.5f) c = YELLOW;
    DrawRectangle(x, y, (int)(w * pct), h, c);
    DrawRectangleLines(x, y, w, h, BLACK);
}

void DrawPokemonSprite(int id, int x, int y, float scale, bool flip) {
    Texture2D tex = GetPokemonTexture(id);
    Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
    if (flip) source.width *= -1;
    Rectangle dest = { (float)x, (float)y, (float)tex.width * scale, (float)tex.height * scale };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
}

void LogDebug(const char* fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // Silenciamos la salida a consola para la versión final
    // fprintf(stderr, "%s\n", buffer); 
    // fflush(stderr);
    strncpy(lastLog, buffer, 255);
    lastLog[255] = '\0';
}

void RegistrarError(const char* msg) {
    LogDebug("[ERROR] %s", msg);
}

void MostrarPantallaLogs() {
    EsperarSoltarMouse();
    while(!WindowShouldClose()) {
        ActualizarInputLock(GetFrameTime());
        bool locked = IsInputLocked();
        
        BeginDrawing();
        ClearBackground((Color){20, 20, 30, 255});
        
        float panelW = SW() * 0.9f; float panelH = SH() * 0.85f;
        DrawRectangleRounded((Rectangle){CX()-panelW/2, CY()-panelH/2, panelW, panelH}, 0.05f, 10, (Color){10,10,20,240});
        DrawRectangleRoundedLines((Rectangle){CX()-panelW/2, CY()-panelH/2, panelW, panelH}, 0.05f, 10, 2, MAGENTA);
        
        DrawText("SISTEMA DE ESTADO Y LOGS", (int)(CX()-MeasureText("SISTEMA DE ESTADO Y LOGS", 24)/2), (int)(CY()-panelH/2+20), 24, YELLOW);
        
        DrawText("ÚLTIMO EVENTO:", (int)(CX()-panelW/2+30), (int)(CY()-panelH/2+80), 20, GRAY);
        DrawText(lastLog, (int)(CX()-panelW/2+30), (int)(CY()-panelH/2+110), 18, WHITE);
        
        DrawText("(Revisa la consola negra para el historial completo)", (int)(CX()-panelW/2+30), (int)(CY()+panelH/2-100), 16, GRAY);

        if (GuiButton((Rectangle){ CX()-100, CY()+panelH/2-50, 200, 40 }, "VOLVER AL MENÚ") && !locked) {
            BloquearInput(0.5f);
            break;
        }
        
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) break;
    }
}

const char* GetWinErrorStr(int err) {
    switch(err) {
        case 0: return "SUCCESS";
        case 2: return "FILE NOT FOUND";
        case 3: return "PATH NOT FOUND";
        case 1812: return "RESOURCE DATA NOT FOUND";
        case 1813: return "RESOURCE TYPE NOT FOUND";
        case 1814: return "RESOURCE NAME NOT FOUND";
        case 1815: return "RESOURCE LANG NOT FOUND";
        default: return "UNKNOWN ERROR";
    }
}

void ImprimirErrores() {}
int HayErrores() { return 0; }

// Espera a que el jugador suelte el mouse antes de continuar.
// Esto evita el bug de "click-through" entre pantallas.
void EsperarSoltarMouse() {
    while (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !WindowShouldClose()) {
        BeginDrawing();
        // No limpiamos la pantalla para que la pantalla anterior
        // siga visible mientras el jugador suelta el boton
        EndDrawing();
    }
}
// Helpers responsivos
float SW() { return (float)GetScreenWidth(); }
float SH() { return (float)GetScreenHeight(); }
float CX() { return SW() / 2.0f; }
float CY() { return SH() / 2.0f; }

void DrawBackgroundExt(Texture2D tex, float alpha) {
    if (tex.id > 0) {
        DrawTexturePro(tex,
            (Rectangle){0,0,(float)tex.width,(float)tex.height},
            (Rectangle){0,0,SW(),SH()},
            (Vector2){0,0}, 0.0f, Fade(WHITE, alpha));
    } else {
        ClearBackground(BLACK);
    }
}
void InitGameStyle() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
    GuiSetStyle(BUTTON, TEXT_PADDING, 10);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x1a1a1aff);
}

Color GetTipoColor(tipoPokemon t) {
    switch (t) {
        case FUEGO:    return (Color){ 255, 68, 68, 255 };
        case AGUA:     return (Color){ 68, 163, 255, 255 };
        case PLANTA:   return (Color){ 102, 255, 102, 255 };
        case ELECTRICO:return (Color){ 255, 255, 102, 255 };
        case HIELO:    return (Color){ 173, 216, 230, 255 };
        case LUCHA:    return (Color){ 194, 46, 40, 255 };
        case VENENO:   return (Color){ 160, 64, 160, 255 };
        case TIERRA:   return (Color){ 224, 192, 104, 255 };
        case VOLADOR:  return (Color){ 168, 144, 240, 255 };
        case PSIQUICO: return (Color){ 248, 88, 136, 255 };
        case BICHO:    return (Color){ 168, 184, 32, 255 };
        case ROCA:     return (Color){ 184, 160, 56, 255 };
        case FANTASMA: return (Color){ 112, 88, 152, 255 };
        case DRAGON:   return (Color){ 112, 56, 248, 255 };
        case SINIESTRO:return (Color){ 112, 88, 72, 255 };
        case ACERO:    return (Color){ 184, 184, 208, 255 };
        case HADA:     return (Color){ 238, 153, 172, 255 };
        default:       return (Color){ 168, 168, 120, 255 }; // NORMAL
    }
}

float GetEfectividad(tipoPokemon tAtk, tipoPokemon tDef) {
    if (tAtk == VACIO || tDef == VACIO) return 1.0f;
    // Tabla simplificada (Fuego, Agua, Planta, Electrico)
    if (tAtk == FUEGO) {
        if (tDef == PLANTA || tDef == HIELO || tDef == BICHO || tDef == ACERO) return 2.0f;
        if (tDef == AGUA || tDef == ROCA || tDef == FUEGO || tDef == DRAGON) return 0.5f;
    }
    if (tAtk == AGUA) {
        if (tDef == FUEGO || tDef == TIERRA || tDef == ROCA) return 2.0f;
        if (tDef == AGUA || tDef == PLANTA || tDef == DRAGON) return 0.5f;
    }
    if (tAtk == PLANTA) {
        if (tDef == AGUA || tDef == TIERRA || tDef == ROCA) return 2.0f;
        if (tDef == FUEGO || tDef == PLANTA || tDef == VOLADOR || tDef == BICHO || tDef == VENENO || tDef == DRAGON || tDef == ACERO) return 0.5f;
    }
    if (tAtk == ELECTRICO) {
        if (tDef == AGUA || tDef == VOLADOR) return 2.0f;
        if (tDef == TIERRA) return 0.0f;
        if (tDef == PLANTA || tDef == ELECTRICO || tDef == DRAGON) return 0.5f;
    }
    return 1.0f;
}

void DrawTypeIcon(tipoPokemon t, Vector2 pos, float size) {
    Color c = GetTipoColor(t);
    DrawRectangleRounded((Rectangle){pos.x, pos.y, size, size}, 0.3f, 6, c);
    const char* letter = "N";
    switch(t){
        case FUEGO: letter = "F"; break;
        case AGUA: letter = "A"; break;
        case PLANTA: letter = "P"; break;
        case ELECTRICO: letter = "E"; break;
        case HIELO: letter = "H"; break;
        case TIERRA: letter = "T"; break;
        case VOLADOR: letter = "V"; break;
        case VENENO: letter = "X"; break;
        case PSIQUICO: letter = "S"; break;
        case LUCHA: letter = "L"; break;
        case NORMAL: letter = "N"; break;
        default: letter = "N"; break; // Por defecto Normal "N"
    }
    DrawText(letter, (int)(pos.x + size/2 - 5), (int)(pos.y + size/2 - 8), 16, WHITE);
}

// Input Lock System
static float globalInputLock = 0;
void ActualizarInputLock(float dt) { if (globalInputLock > 0) globalInputLock -= dt; }
bool IsInputLocked() { return globalInputLock > 0; }
void BloquearInput(float duracion) { globalInputLock = duracion; }
