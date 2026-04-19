LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := PokemonGame
LOCAL_SRC_FILES := ../src/Main.c ../src/Pokemon.c ../src/Jugador.c ../src/Menu.c \
                   ../src/Utils.c ../src/Batalla.c ../src/Guardado.c ../src/Mouse.c \
                   ../src/Misiones.c ../src/Renderer.c ../src/Movimiento.c \
                   ../src/Renderer_Android.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_CFLAGS := -DANDROID
LOCAL_LDLIBS := -llog -landroid -lGLESv2
LOCAL_STATIC_LIBRARIES := SDL2 SDL2_ttf
include $(BUILD_SHARED_LIBRARY)

$(call import-module,SDL2)
$(call import-module,SDL2_ttf)
