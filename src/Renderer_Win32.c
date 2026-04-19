#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "Utils.h"

// GDI+ Flat API
typedef void* GpImage;
typedef void* GpGraphics;
typedef struct { UINT32 GdiplusVersion; void* DebugEventCallback; BOOL SuppressBackgroundThread; BOOL SuppressExternalCodecs; } GdiplusStartupInput;
extern int __stdcall GdiplusStartup(ULONG_PTR*, const GdiplusStartupInput*, void*);
extern int __stdcall GdipCreateBitmapFromFile(const wchar_t*, GpImage**);
extern int __stdcall GdipDisposeImage(GpImage*);
extern int __stdcall GdipCreateFromHDC(HDC, GpGraphics**);
extern int __stdcall GdipDeleteGraphics(GpGraphics*);
extern int __stdcall GdipDrawImageRectI(GpGraphics*, GpImage*, int, int, int, int);
extern int __stdcall GdipImageGetFrameCount(GpImage*, const GUID*, UINT*);
extern int __stdcall GdipImageSelectActiveFrame(GpImage*, const GUID*, UINT);

static GUID FrameDimensionTime = {0x6aedbd6d, 0x3fb5, 0x418a, {0x83, 0xa6, 0x7f, 0x45, 0x22, 0x9d, 0xc8, 0x72}};
static ULONG_PTR gdiplusToken;
static GpImage* imgStartersFrente[3] = {NULL, NULL, NULL}; // 0:Bulb, 1:Charm, 2:Squir
static GpImage* imgEspaldaCharmander = NULL;
static UINT frameCount = 1;

static HWND hWnd = NULL;
static HFONT hFont = NULL;
static uint32_t lastChar = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Doble buffer para evitar parpadeo y asegurar limpieza
            HDC memDC = CreateCompatibleDC(hdc);
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            SelectObject(memDC, memBitmap);
            
            // Fondo negro
            FillRect(memDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
            
            SelectObject(memDC, hFont);
            SetBkMode(memDC, TRANSPARENT);
            
            // Calcular tamaño de cada "celda" de terminal
            int tileW = rect.right / 80;
            int tileH = rect.bottom / 25;
            if (tileW < 1) tileW = 1;
            if (tileH < 1) tileH = 1;
            
            // Centrado: Calcular offsets sobrantes
            int offsetX = (rect.right - (80 * tileW)) / 2;
            int offsetY = (rect.bottom - (25 * tileH)) / 2;

            COLORREF winColors[] = {
                RGB(0,0,0), RGB(0,0,128), RGB(0,128,0), RGB(0,128,128),
                RGB(128,0,0), RGB(128,0,128), RGB(128,128,0), RGB(192,192,192),
                RGB(128,128,128), RGB(0,0,255), RGB(0,255,0), RGB(0,255,255),
                RGB(255,0,0), RGB(255,0,255), RGB(255,255,0), RGB(255,255,255)
            };

            for (int y = 0; y < 25; y++) {
                for (int x = 0; x < 80; x++) {
                    uint32_t cp = GetVirtualChar(x, y);
                    if (cp != 0 && cp != ' ') {
                        SetTextColor(memDC, winColors[GetVirtualColor(x, y) % 16]);
                        wchar_t wc = (wchar_t)cp;
                        // Usar tileW, tileH y offsets para el posicionamiento centrado
                        TextOutW(memDC, offsetX + x * tileW, offsetY + y * tileH, &wc, 1);
                    }
                }
            }

            // [NUEVO] Dibujar imagenes de combate
            if (GetRenderState() == 2) { // 2 = BATALLA
                extern void GetCombatSprites(int*, int*);
                int pID, rID;
                GetCombatSprites(&pID, &rID);
                extern void DibujarCombateImagenesExt(HDC, int, int, int, int, int, int);
                DibujarCombateImagenesExt(memDC, offsetX, offsetY, tileW, tileH, pID, rID);
            }
            
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
            
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SIZE: {
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        case WM_CHAR: {
            lastChar = (uint32_t)wParam;
            return 0;
        }
        case WM_ERASEBKGND:
            return 1; 
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

uint32_t GetLastChar() {
    uint32_t c = lastChar;
    lastChar = 0;
    return c;
}

void IniciarVentanaWindows() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PokemonGDI";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    
    RegisterClass(&wc);
    
    hWnd = CreateWindowEx(0, "PokemonGDI", "Pokemon RPG - Windows Native", 
                         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                         CW_USEDEFAULT, CW_USEDEFAULT, 960, 600,
                         NULL, NULL, hInstance, NULL);
                         
    // Inicializar GDI+
    GdiplusStartupInput gsi = {1, NULL, FALSE, FALSE};
    GdiplusStartup(&gdiplusToken, &gsi, NULL);

    // Cargar imagenes de combate
    GdipCreateBitmapFromFile(L"img/Bulbasaur_Frente.gif", (GpImage**)&imgStartersFrente[0]);
    if (!imgStartersFrente[0]) GdipCreateBitmapFromFile(L"../img/Bulbasaur_Frente.gif", (GpImage**)&imgStartersFrente[0]);

    GdipCreateBitmapFromFile(L"img/Charmander_Frente.png", (GpImage**)&imgStartersFrente[1]);
    if (!imgStartersFrente[1]) GdipCreateBitmapFromFile(L"../img/Charmander_Frente.png", (GpImage**)&imgStartersFrente[1]);

    GdipCreateBitmapFromFile(L"img/Squirtle_Frente.gif", (GpImage**)&imgStartersFrente[2]);
    if (!imgStartersFrente[2]) GdipCreateBitmapFromFile(L"../img/Squirtle_Frente.gif", (GpImage**)&imgStartersFrente[2]);
    
    GdipCreateBitmapFromFile(L"img/Charmander_Espalda.png", (GpImage**)&imgEspaldaCharmander);
    if (!imgEspaldaCharmander) GdipCreateBitmapFromFile(L"../img/Charmander_Espalda.png", (GpImage**)&imgEspaldaCharmander);

    // Crear un font que se vea bien en varios tamaños
    hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      FIXED_PITCH | FF_MODERN, "Consolas");
}

void DibujarCombateImagenesExt(HDC hdc, int offsetX, int offsetY, int tileW, int tileH, int pID, int rID) {
    GpGraphics* graphics = NULL;
    GdipCreateFromHDC(hdc, &graphics);
    if (!graphics) return;

    // Rival: Arriba Derecha (Info esta en x=2, y=2)
    GpImage* imgR = (rID >= 0 && rID < 3) ? imgStartersFrente[rID] : NULL;
    if (imgR) {
        int rX = offsetX + 58 * tileW;
        int rY = offsetY + 1 * tileH;
        GdipDrawImageRectI(graphics, imgR, rX, rY, 18 * tileW, 10 * tileH);
    }

    // Jugador: Abajo Izquierda (Info esta en x=42, y=10)
    GpImage* imgP = (pID == 1) ? imgEspaldaCharmander : ((pID >= 0 && pID < 3) ? imgStartersFrente[pID] : NULL);
    if (imgP) {
        int jX = offsetX + 5 * tileW;
        int jY = offsetY + 4 * tileH; 
        GdipDrawImageRectI(graphics, imgP, jX, jY, 22 * tileW, 12 * tileH);
    }

    GdipDeleteGraphics(graphics);
}

int ProcesarMensajes() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) exit(0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (hWnd) InvalidateRect(hWnd, NULL, FALSE);
    return 1;
}
#endif
