#ifndef Utils_H
#define Utils_H

#include "Pokemon.h"

void BorrarPantallaCompleta();
void RegistrarError(const char* msg);
void ImprimirErrores();
void LimpiarErrores();
int HayErrores();
void EscribirLento(const char* texto, int ms);
void ImprimirTipoBadge(tipoPokemon t);
void SetRenderState(int state);
void SetCombatSprites(int pID, int rID);
int GetRenderState();
void LimpiarArea(int x, int y, int w, int h);
void MoverCursor(int x, int y);
void CambiarColor(int color);
void DibujarCajaUTF8(int x, int y, int w, int h, int color);

#include <stdint.h>

void UpdateVirtualCursor(int x, int y);
int GetVirtualCursorX();
int GetVirtualCursorY();
void UpdateVirtualColor(int color);
void ClearVirtualBuffer();
void VirtualPrint(const char* s);
#include <stdio.h>
#include <stdarg.h>

void VirtualPrintf(const char* format, ...);
uint32_t GetVirtualChar(int x, int y);
int GetVirtualColor(int x, int y);
int ProcesarMensajes();
uint32_t GetLastChar();

#if defined(ANDROID) || defined(_WIN32)
#undef printf
#define printf(...) VirtualPrintf(__VA_ARGS__)
#endif

#ifdef ANDROID
#include <SDL.h>
#define Sleep(ms) SDL_Delay(ms)
#endif

#endif
