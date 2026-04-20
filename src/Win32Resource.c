#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#include <stdio.h>

int GetResourceData(int resourceId, const char* resourceType, void** outData, unsigned int* outSize) {
    LPCSTR type = (resourceType == NULL) ? (LPCSTR)10 : resourceType; // 10 is RT_RCDATA
    // Usamos UINT_PTR para evitar advertencias de conversion de puntero en 64 bits
    HRSRC hRes = FindResourceA(NULL, (LPCSTR)(UINT_PTR)resourceId, type);
    if (!hRes) {
        return (int)GetLastError();
    }

    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) {
        return (int)GetLastError();
    }

    void* data = LockResource(hData);
    if (!data) return (int)GetLastError();

    *outData = data;
    *outSize = (unsigned int)SizeofResource(NULL, hRes);
    
    return 0; // SUCCESS
}
