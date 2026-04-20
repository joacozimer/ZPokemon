#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "gdiplus.lib")

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0, size = 0;
    GdipGetImageEncodersSize(&num, &size);
    if (size == 0) return -1;
    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) return -1;
    GdipGetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return -1;
}

void ConvertToPng(const char* filename) {
    WCHAR wFilename[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, wFilename, MAX_PATH);

    GpImage *image;
    if (GdipLoadImageFromFile(wFilename, &image) == Ok) {
        CLSID pngClsid;
        GetEncoderClsid(L"image/png", &pngClsid);
        
        WCHAR wOutName[MAX_PATH];
        swprintf(wOutName, MAX_PATH, L"%s.true.png", wFilename);
        
        if (GdipSaveImageToFile(image, wOutName, &pngClsid, NULL) == Ok) {
            printf("[OK] %s convertido exitosamente.\n", filename);
        } else {
            printf("[ERROR] No se pudo guardar %s como PNG.\n", filename);
        }
        GdipDisposeImage(image);
        
        // Reemplazar original
        DeleteFileW(wFilename);
        MoveFileW(wOutName, wFilename);
    } else {
        printf("[ERROR] No se pudo cargar %s (¿archivo corrupto?)\n", filename);
    }
}

int main() {
    GdiplusStartupInput gdiplusStartupInput = {1, NULL, FALSE, FALSE};
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    const char* assets[] = {
        "assets/bulbasaur.png", "assets/charmander.png", "assets/squirtle.png",
        "assets/pikachu.png", "assets/eevee.png", "assets/mew.png",
        "assets/battle_bg.png", "assets/menu_bg.png", "assets/intro_bg.png",
        "assets/start_bg.png", "assets/loading_bg.png"
    };

    printf("Iniciando conversion de assets a PNG real...\n");
    for (int i = 0; i < 11; i++) {
        ConvertToPng(assets[i]);
    }

    GdiplusShutdown(gdiplusToken);
    printf("Proceso finalizado.\n");
    return 0;
}
