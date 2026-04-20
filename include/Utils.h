#ifndef Utils_H
#define Utils_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include "raylib.h"
#include "Pokemon.h"

// UI System
void BorrarPantallaCompleta();
void MoverCursor(int x, int y);
void CambiarColor(int color);
void DibujarCajaUTF8(int x, int y, int ancho, int alto, int color);
void DibujarRectangulo(int x, int y, int ancho, int alto);
void LimpiarArea(int x, int y, int ancho, int alto);
void ProcesarMensajes();

// Raylib / Graphical Interface
void TerminalPrint(const char* fmt, ...);
int GetCentroX();
void RenderRaylibFrame();
void DibujarBarraVida(int x, int y, int w, int h, float pct);
void DrawPokemonSprite(int id, int x, int y, float scale, bool flip);

// Helpers responsivos
float SW();
float SH();
float CX();
float CY();
void DrawBackgroundExt(Texture2D tex, float alpha);

// Sistema de Estilos y Tipos
void InitGameStyle();
Color GetTipoColor(tipoPokemon t);
float GetEfectividad(tipoPokemon tAtk, tipoPokemon tDef);
void DrawTypeIcon(tipoPokemon t, Vector2 pos, float size);

// Sistema de Input (Anti-Martilleo)
void ActualizarInputLock(float dt);
bool IsInputLocked();
void BloquearInput(float duracion);
void EsperarSoltarMouse();
void WaitMs(int ms);

// Decoración
void ImprimirTipoBadge(tipoPokemon t);
void ImprimirCentrado(int y, const char* texto, int color);
void RegistrarError(const char* msg);
void LogDebug(const char* fmt, ...);
void MostrarPantallaLogs();
void ImprimirErrores();
int HayErrores();
const char* GetWinErrorStr(int err);

// Redefinimos printf para que todo el código existente imprima en la ventana Raylib
#define printf(...) LogDebug(__VA_ARGS__)

#endif
