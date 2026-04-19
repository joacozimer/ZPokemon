#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Utils.h"

#define V_WIDTH 80
#define V_HEIGHT 25

static uint32_t v_buffer[V_HEIGHT][V_WIDTH];
static int v_colors[V_HEIGHT][V_WIDTH];
static int cur_x = 0, cur_y = 0;
static int cur_color = 7;

void UpdateVirtualCursor(int x, int y) {
    if (x >= 0 && x < V_WIDTH) cur_x = x;
    if (y >= 0 && y < V_HEIGHT) cur_y = y;
}

void UpdateVirtualColor(int color) {
    cur_color = color;
}

#include <stdarg.h>

void VirtualPrintf(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    VirtualPrint(buffer);
}

// Simple UTF-8 decoder
static const char* GetNextCodepoint(const char* s, uint32_t* cp) {
    unsigned char c = (unsigned char)*s;
    if (c < 0x80) { *cp = c; return s + 1; }
    if ((c & 0xE0) == 0xC0) { *cp = ((c & 0x1F) << 6) | (s[1] & 0x3F); return s + 2; }
    if ((c & 0xF0) == 0xE0) { *cp = ((c & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F); return s + 3; }
    if ((c & 0xF8) == 0xF0) { *cp = ((c & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F); return s + 4; }
    *cp = c; return s + 1;
}

void VirtualPrint(const char* s) {
    while (*s) {
        if (*s == '\n') {
            cur_x = 0;
            cur_y++;
            if (cur_y >= 25) cur_y = 0; 
            s++;
            continue;
        }
        if (*s == '\r') {
            cur_x = 0;
            s++;
            continue;
        }
        
        if (cur_x < 80 && cur_y < 25) {
            uint32_t cp;
            const char* next = GetNextCodepoint(s, &cp);
            if (next == s) { s++; continue; } 
            s = next;
            v_buffer[cur_y][cur_x] = cp;
            v_colors[cur_y][cur_x] = cur_color;
            cur_x++;
        } else {
            s++;
        }
    }
}

void ClearVirtualBuffer() {
    memset(v_buffer, 0, sizeof(v_buffer));
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            v_buffer[y][x] = ' ';
            v_colors[y][x] = 15;
        }
    }
    cur_x = 0; cur_y = 0;
}

int GetVirtualCursorX() { return cur_x; }
int GetVirtualCursorY() { return cur_y; }

uint32_t GetVirtualChar(int x, int y) {
    return v_buffer[y][x];
}

int GetVirtualColor(int x, int y) {
    return v_colors[y][x];
}

static int global_render_state = 0;
static int combat_pID = 0;
static int combat_rID = 0;

void SetRenderState(int s) { global_render_state = s; }
int GetRenderState() { return global_render_state; }

void SetCombatSprites(int pID, int rID) {
    combat_pID = pID;
    combat_rID = rID;
}

void GetCombatSprites(int* pID, int* rID) {
    *pID = combat_pID;
    *rID = combat_rID;
}
