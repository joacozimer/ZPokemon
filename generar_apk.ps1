# Script para generar el APK de Pokemon RPG usando Docker
Write-Host "--- Generando APK de Pokemon RPG ---" -ForegroundColor Cyan

# 1. Construir la imagen de Docker
Write-Host "[1/3] Construyendo imagen de Android (esto puede tardar varios minutos)..."
docker build -t pokemon-apk .

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Falló la construcción de la imagen de Docker." -ForegroundColor Red
    exit $LASTEXITCODE
}

# 2. Asegurar que la carpeta bin existe
if (!(Test-Path -Path "bin")) {
    New-Item -ItemType Directory -Path "bin"
}

# 3. Correr el contenedor, compilar con Gradle y extraer el APK
Write-Host "[2/3] Compilando APK con Gradle dentro de Docker (puede tardar unos minutos)..."
docker run --rm -v "${PWD}:/out" pokemon-apk bash -c `
    "cd /pokebuild && ./gradlew assembleDebug --no-daemon 2>&1 | tail -80 && cp app/build/outputs/apk/debug/app-debug.apk /out/bin/PokemonRPG.apk && echo 'OK' || (echo 'GRADLE_FALLO' && exit 1)"

if ($LASTEXITCODE -eq 0) {
    Write-Host "[3/3] ¡ÉXITO! APK generado en: bin/PokemonRPG.apk" -ForegroundColor Green
} else {
    Write-Host "ERROR: Falló la compilación con Gradle. Revisa el log de arriba." -ForegroundColor Red
    exit 1
}
