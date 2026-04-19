FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

# ---- Sistema ----
RUN apt-get update && apt-get install -y \
    wget unzip git build-essential openjdk-17-jdk-headless \
    libncurses5 python3 curl file && \
    rm -rf /var/lib/apt/lists/*

# ---- Gradle 7.6 (versión manual, la de apt es muy antigua) ----
RUN wget -q https://services.gradle.org/distributions/gradle-7.6-bin.zip -O /tmp/gradle.zip && \
    unzip -q /tmp/gradle.zip -d /opt && \
    ln -s /opt/gradle-7.6/bin/gradle /usr/local/bin/gradle && \
    rm /tmp/gradle.zip

# ---- Android SDK + NDK ----
RUN mkdir -p /opt/android-sdk/cmdline-tools && \
    wget -q https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip -O tools.zip && \
    unzip -q tools.zip -d /opt/android-sdk/cmdline-tools && \
    mv /opt/android-sdk/cmdline-tools/cmdline-tools /opt/android-sdk/cmdline-tools/latest && \
    rm tools.zip

ENV ANDROID_HOME=/opt/android-sdk
ENV PATH=$PATH:$ANDROID_HOME/cmdline-tools/latest/bin:$ANDROID_HOME/platform-tools

RUN yes | sdkmanager --licenses > /dev/null 2>&1 || true
RUN sdkmanager --install "platforms;android-33" "build-tools;33.0.2" "ndk;25.2.9519653"

# ---- SDL2 + SDL2_ttf para NDK ----
RUN mkdir -p /libs && \
    wget -q https://github.com/libsdl-org/SDL/releases/download/release-2.26.5/SDL2-2.26.5.zip -O sdl2.zip && \
    unzip -q sdl2.zip -d /libs && mv /libs/SDL2-2.26.5 /libs/SDL2 && \
    wget -q https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-2.20.2.zip -O sdl2ttf.zip && \
    unzip -q sdl2ttf.zip -d /libs && mv /libs/SDL2_ttf-2.20.2 /libs/SDL2_ttf && \
    rm -f sdl2.zip sdl2ttf.zip

# ---- Fuente TTF ----
RUN mkdir -p /pokebuild/app/src/main/assets && \
    wget -q "https://github.com/google/fonts/raw/main/apache/robotomono/static/RobotoMono-Regular.ttf" \
    -O /pokebuild/app/src/main/assets/terminal.ttf 2>/dev/null || \
    wget -q "https://github.com/notofonts/noto-fonts/raw/main/hinted/ttf/NotoMono/NotoMono-Regular.ttf" \
    -O /pokebuild/app/src/main/assets/terminal.ttf || true

# ---- Copiar fuentes del proyecto ----
WORKDIR /app
COPY . .

# ---- Armar proyecto Gradle en /pokebuild ----
RUN mkdir -p /pokebuild/app/src/main/jni && \
    cp android/jni/Android.mk android/jni/Application.mk /pokebuild/app/src/main/jni/ && \
    cp -r src /pokebuild/app/src/main/c && \
    cp -r include /pokebuild/app/src/main/include && \
    cp android/AndroidManifest.xml /pokebuild/app/src/main/AndroidManifest.xml && \
    cp -r img /pokebuild/app/src/main/assets/ 2>/dev/null || true

# Ajustar rutas en Android.mk  ../src/ -> ../c/  y  ../include -> ../include (ya estan bien)
RUN sed -i 's|\.\./src/|../c/|g' /pokebuild/app/src/main/jni/Android.mk

# ---- Gradle files ----
RUN echo "include ':app'" > /pokebuild/settings.gradle

RUN echo "buildscript { repositories { google(); mavenCentral() }; dependencies { classpath 'com.android.tools.build:gradle:7.4.2' } }; allprojects { repositories { google(); mavenCentral() } }" \
    > /pokebuild/build.gradle

RUN printf "apply plugin: 'com.android.application'\n\
android {\n\
    compileSdk 33\n\
    defaultConfig {\n\
        applicationId 'com.pokemon.rpg'\n\
        minSdk 21\n\
        targetSdk 33\n\
        versionCode 1\n\
        versionName '1.0'\n\
        externalNativeBuild { ndkBuild { arguments 'NDK_MODULE_PATH=/libs' } }\n\
    }\n\
    sourceSets {\n\
        main {\n\
            manifest.srcFile 'src/main/AndroidManifest.xml'\n\
            java.srcDirs = []\n\
            res.srcDirs = []\n\
            assets.srcDirs = ['src/main/assets']\n\
        }\n\
    }\n\
    externalNativeBuild { ndkBuild { path 'src/main/jni/Android.mk' } }\n\
}\n" > /pokebuild/app/build.gradle

# ---- Generar gradle wrapper ----
RUN cd /pokebuild && gradle wrapper --gradle-version=7.6 --distribution-type=bin

# ---- Compilar APK cuando se ejecute el contenedor ----
CMD ["bash", "-c", "cd /pokebuild && ./gradlew assembleDebug --no-daemon 2>&1 | tail -100"]
