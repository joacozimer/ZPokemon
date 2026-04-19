#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "Pokemon.h"
#include "Mouse.h"
#include "Menu.h"
#include "Utils.h"
#include "Batalla.h"

// HISTORIAL DE BATALLA
char historial[3][100] = {"", "", ""};

void DibujarRectangulo(int x, int y, int w, int h) {
    MoverCursor(x, y);
    printf("┌"); for (int i = 0; i < w - 2; i++) printf("─"); printf("┐");

    for (int i = 1; i < h - 1; i++) {
        MoverCursor(x, y + i);
        printf("│");
        MoverCursor(x + w - 1, y + i);
        printf("│");
    }

    MoverCursor(x, y + h - 1);
    printf("└"); for (int i = 0; i < w - 2; i++) printf("─"); printf("┘");
}

void RedibujarHistorial() {
    for(int i=0; i<3; i++) {
        MoverCursor(42, 21 + i);
        printf("%-35s", ""); 
        if (strlen(historial[i]) > 0) {
            MoverCursor(42, 21 + i);
            CambiarColor(i == 2 ? 15 : 8); 
            char temp[36]; strncpy(temp, historial[i], 35); temp[35] = '\0';
            printf("%s", temp);
        }
    }
    CambiarColor(7);
}

void MostrarInfoPokemon(Pokemon* p, int x, int y) {
    MoverCursor(x + 2, y + 1);
    char buf[100];
    sprintf(buf, "%-20s (Niv. %d)", p->nombre, p->nivel);
    CambiarColor(11); 
    printf("%s", buf);
    
    MoverCursor(x + 2, y + 2);
    ImprimirTipoBadge(p->tipo[0]);
    if (p->tipo[1] != VACIO) { printf(" "); ImprimirTipoBadge(p->tipo[1]); }

    MoverCursor(x + 2, y + 3);
    CambiarColor(15);
    sprintf(buf, "PS: %3d / %3d    ", p->vida, p->vidaMax);
    printf("%s", buf);
    
    MoverCursor(x + 2, y + 4);
    if (p->estado == QUEMADO) { CambiarColor(12); printf("ST: QUEMADO   "); }
    else if (p->estado == DEBILITADO) { CambiarColor(8); printf("ST: DEBILITADO"); }
    else { CambiarColor(10); printf("ST: OK        "); }
    CambiarColor(7);
}

// Lógica de daño básica
float GetMultiplicador(tipoPokemon atk, tipoPokemon def) {
    if (atk == VACIO || def == VACIO || atk >= 18 || def >= 18) return 1.0f;
    static float tabla[18][18];
    static int init = 0;
    if(!init) {
        for(int i=0; i<18; i++) for(int j=0; j<18; j++) tabla[i][j] = 1.0f;
        // FUEGO
        tabla[FUEGO][PLANTA] = 2.0; tabla[FUEGO][HIELO] = 2.0; tabla[FUEGO][BICHO] = 2.0; tabla[FUEGO][ACERO] = 2.0;
        tabla[FUEGO][FUEGO] = 0.5; tabla[FUEGO][AGUA] = 0.5; tabla[FUEGO][ROCA] = 0.5; tabla[FUEGO][DRAGON] = 0.5;
        // AGUA
        tabla[AGUA][FUEGO] = 2.0; tabla[AGUA][TIERRA] = 2.0; tabla[AGUA][ROCA] = 2.0;
        tabla[AGUA][AGUA] = 0.5; tabla[AGUA][PLANTA] = 0.5; tabla[AGUA][DRAGON] = 0.5;
        // PLANTA
        tabla[PLANTA][AGUA] = 2.0; tabla[PLANTA][TIERRA] = 2.0; tabla[PLANTA][ROCA] = 2.0;
        tabla[PLANTA][FUEGO] = 0.5; tabla[PLANTA][PLANTA] = 0.5; tabla[PLANTA][VENENO] = 0.5; tabla[PLANTA][VOLADOR] = 0.5; tabla[PLANTA][BICHO] = 0.5; tabla[PLANTA][ACERO] = 0.5;
        // ELECTRICO
        tabla[ELECTRICO][AGUA] = 2.0; tabla[ELECTRICO][VOLADOR] = 2.0; 
        tabla[ELECTRICO][ELECTRICO] = 0.5; tabla[ELECTRICO][PLANTA] = 0.5; tabla[ELECTRICO][DRAGON] = 0.5;
        tabla[ELECTRICO][TIERRA] = 0.0;
        // VENENO (Nuevo)
        tabla[VENENO][PLANTA] = 2.0; tabla[VENENO][HADA] = 2.0;
        tabla[VENENO][VENENO] = 0.5; tabla[VENENO][TIERRA] = 0.5; tabla[VENENO][ROCA] = 0.5; tabla[VENENO][FANTASMA] = 0.5;
        tabla[VENENO][ACERO] = 0.0;
        // VOLADOR (Nuevo)
        tabla[VOLADOR][PLANTA] = 2.0; tabla[VOLADOR][LUCHA] = 2.0; tabla[VOLADOR][BICHO] = 2.0;
        tabla[VOLADOR][ELECTRICO] = 0.5; tabla[VOLADOR][ROCA] = 0.5; tabla[VOLADOR][ACERO] = 0.5;
        init = 1;
    }
    return tabla[(int)atk][(int)def];
}

int CalcularDanio(Movimiento m, Pokemon* atacante, Pokemon* defensor) {
    if (m.clase == ESTADO || m.potencia == 0) return 0;
    if (m.esOHKO) return defensor->vida;
    
    float a = (m.clase == FISICO) ? (float)atacante->atk : (float)atacante->atkSp;
    float d = (m.clase == FISICO) ? (float)defensor->def : (float)defensor->defSp;
    
    // Multiplicador de tipos
    float mult = GetMultiplicador((tipoPokemon)m.tipo, defensor->tipo[0]);
    if (defensor->tipo[1] != VACIO) mult *= GetMultiplicador((tipoPokemon)m.tipo, defensor->tipo[1]);
    
    // STAB (Same Type Attack Bonus)
    float stab = 1.0f;
    if ((tipoPokemon)m.tipo == atacante->tipo[0] || (tipoPokemon)m.tipo == atacante->tipo[1]) stab = 1.5f;

    // Formula mejorada para niveles bajos
    // ( ( (2 * Nivel / 5 + 2) * Poder * A/D ) / 40 + 2) * Multiplicador * STAB
    float base = ((((2.0f * atacante->nivel) / 5.0f + 2.0f) * m.potencia * (a / d)) / 40.0f + 2.0f);
    int danio = (int)(base * mult * stab);
    
    return danio < 1 ? 1 : danio; // Minimo 1 de daño
}

void LogBatalla(const char* msg) {
    strcpy(historial[0], historial[1]);
    strcpy(historial[1], historial[2]);
    strncpy(historial[2], msg, 99);
    RedibujarHistorial();
    Sleep(600);
}

void ResetearEquipo(Jugador* j) {
    for (int i = 0; i < j->cantidadPokemons; i++) {
        j->pokemons[i].vida = j->pokemons[i].vidaMax;
        j->pokemons[i].estado = SALUDABLE;
    }
}

void DibujarUIEstatica() {
    BorrarPantallaCompleta();
    int ctx = GetCentroX(); // 40
    // Rival: Arriba Izquierda
    DibujarRectangulo(ctx - 38, 2, 35, 7);
    // Jugador: Abajo Derecha
    DibujarRectangulo(ctx + 2, 10, 35, 7);
    
    // Contenedores inferiores: 40 de ancho cada uno
    DibujarRectangulo(0, 18, 40, 7); // Menu
    DibujarRectangulo(40, 18, 40, 7); // Historial
    
    MoverCursor(54, 18); printf(" HISTORIAL ");
    MoverCursor(14, 18); printf(" MENU ");

    RedibujarHistorial();
}

int HayPokemonsVivos(Jugador* j) {
    for(int i=0; i<j->cantidadPokemons; i++) if(j->pokemons[i].vida > 0) return 1;
    return 0;
}

int SeleccionarSiguientePokemon(Jugador* j) {
    BorrarPantallaCompleta();
    int cx = GetCentroX();
    while(1) {
        ImprimirCentrado(2, "¡TU POKEMON SE DEBILITO!", 12);
        ImprimirCentrado(3, "Selecciona el siguiente para luchar:", 15);
        for(int i=0; i<j->cantidadPokemons; i++) {
            MoverCursor(cx - 10, 6+i);
            if(j->pokemons[i].vida > 0) CambiarColor(10); else CambiarColor(8);
            printf("%d. %-15s", i+1, j->pokemons[i].nombre);
        }
        MouseState m = obtenerMouseState();
        for(int i=0; i<j->cantidadPokemons; i++) {
            if(mouseEnRango(cx-10, 6+i, cx+10, 6+i, m) && j->pokemons[i].vida > 0) return i;
        }
        Sleep(50);
    }
}

int SeleccionarSiguienteRival(Jugador* j) {
    int vivos[6], n = 0;
    for(int i=0; i<j->cantidadPokemons; i++) if(j->pokemons[i].vida > 0) vivos[n++] = i;
    if (n == 0) return -1;
    return vivos[rand() % n];
}

void IniciarBatalla(Jugador* jugador, Jugador* rival) {
    SetRenderState(2); // MODO BATALLA
    int idxMi = 0;
    int idxRival = 0;
    
    // IMPORTANTE: Sanar a ambos antes de empezar
    ResetearEquipo(jugador);
    ResetearEquipo(rival);
    
    Pokemon* miPk = &jugador->pokemons[idxMi];
    Pokemon* rivalPk = &rival->pokemons[idxRival];
    SetCombatSprites(miPk->id, rivalPk->id);

    for(int i=0; i<3; i++) strcpy(historial[i], ""); 
    DibujarUIEstatica();
    
    while (HayPokemonsVivos(jugador) && HayPokemonsVivos(rival)) {
        Pokemon* miPk = &jugador->pokemons[idxMi];
        Pokemon* rivalPk = &rival->pokemons[idxRival];
        SetCombatSprites(miPk->id, rivalPk->id);
        
        int ctx = GetCentroX();
        MostrarInfoPokemon(rivalPk, ctx - 38, 2);
        MostrarInfoPokemon(miPk, ctx + 2, 10);
        
        // El rectangulo del menu ya se dibuja en DibujarUIEstatica, pero lo aseguramos
        DibujarRectangulo(0, 18, 40, 7);
        MoverCursor(14, 18); printf(" MENU ");
        
        MoverCursor(5, 20); CambiarColor(10); printf("[1] ATACAR");
        MoverCursor(21, 20); CambiarColor(14); printf("[2] MOCHILA");
        MoverCursor(5, 23); CambiarColor(11); printf("[3] POKEMON");
        MoverCursor(21, 23); CambiarColor(12); printf("[4] SALIR");
        CambiarColor(7);

        int elegido = -1;
        while(elegido == -1) {
            MouseState m = obtenerMouseState();
            if (mouseEnRango(5, 20, 15, 20, m)) elegido = 1;
            else if (mouseEnRango(21, 20, 35, 20, m)) elegido = 2;
            else if (mouseEnRango(5, 23, 15, 23, m)) elegido = 3;
            else if (mouseEnRango(21, 23, 35, 23, m)) elegido = 4;
            Sleep(50);
        }
        EsperarSueltaMouse(); // Evitar que el siguiente frame detecte este click

        if (elegido == 4) { // SALIR
            ResetearEquipo(jugador);
            return;
        }

        if (elegido == 1) { // ATACAR
            // MOSTRAR ATAQUES EN EL MISMO LUGAR
            LimpiarArea(1, 19, 38, 5);
            MoverCursor(14, 18); printf(" ATAQUES ");
            CambiarColor(12); MoverCursor(2, 19); printf("< VOLVER"); CambiarColor(7);

            for(int i=0; i<4; i++) {
                if(miPk->movimientos[i].nombre[0] != '\0') {
                    // Calcular efectividad para color
                    float mult = GetMultiplicador((tipoPokemon)miPk->movimientos[i].tipo, rivalPk->tipo[0]);
                    if (rivalPk->tipo[1] != VACIO) mult *= GetMultiplicador((tipoPokemon)miPk->movimientos[i].tipo, rivalPk->tipo[1]);
                    
                    if (mult > 1.0f) CambiarColor(10); // Super efectivo
                    else if (mult < 1.0f) CambiarColor(12); // Poco efectivo
                    else CambiarColor(15); // Normal
                    
                    MoverCursor(5 + (i % 2) * 18, 21 + (i / 2) * 2);
                    printf("[%d] %s", i+1, miPk->movimientos[i].nombre);
                }
            }
            CambiarColor(7);
            
            int seleccionado = -1;
            int v_menu = 0;
            while(seleccionado == -1 && !v_menu) {
                MouseState m2 = obtenerMouseState();
                if(mouseEnRango(2, 19, 10, 19, m2)) v_menu = 1;
                for(int i=0; i<4; i++) {
                    if(miPk->movimientos[i].nombre[0] != '\0') {
                        int x = 5 + (i % 2) * 18;
                        int y = 21 + (i / 2) * 2;
                        if(mouseEnRango(x, y, x + 15, y, m2)) seleccionado = i;
                    }
                }
                Sleep(50);
            }
            EsperarSueltaMouse(); // Evitar ghosting al volver al menu
            if (v_menu) { DibujarUIEstatica(); continue; }

            int numGolpes = miPk->movimientos[seleccionado].golpeMin;
            if (miPk->movimientos[seleccionado].golpeMax > miPk->movimientos[seleccionado].golpeMin) {
                numGolpes += rand() % (miPk->movimientos[seleccionado].golpeMax - miPk->movimientos[seleccionado].golpeMin + 1);
            }

            int danioTotal = 0;
            for (int g = 0; g < numGolpes; g++) {
                int danio = CalcularDanio(miPk->movimientos[seleccionado], miPk, rivalPk);
                rivalPk->vida -= danio;
                danioTotal += danio;
                if (rivalPk->vida < 0) rivalPk->vida = 0;
                if (rivalPk->vida == 0) break;
            }
            
            // Aplicar Buffs
            if (miPk->movimientos[seleccionado].buffStat > 0) {
                int cant = miPk->movimientos[seleccionado].cantBuff;
                if (miPk->movimientos[seleccionado].buffStat == 1) { miPk->atk += (miPk->atk * cant / 10); LogBatalla("¡Ataque subio!"); }
                else if (miPk->movimientos[seleccionado].buffStat == 2) { miPk->def += (miPk->def * cant / 10); LogBatalla("¡Defensa subio!"); }
            }

            float mult = GetMultiplicador((tipoPokemon)miPk->movimientos[seleccionado].tipo, rivalPk->tipo[0]);
            if (rivalPk->tipo[1] != VACIO) mult *= GetMultiplicador((tipoPokemon)miPk->movimientos[seleccionado].tipo, rivalPk->tipo[1]);

            char b[100]; 
            if (numGolpes > 1) sprintf(b, "%s uso %s! (%d veces) -%d", miPk->nombre, miPk->movimientos[seleccionado].nombre, numGolpes, danioTotal);
            else sprintf(b, "%s uso %s! -%d", miPk->nombre, miPk->movimientos[seleccionado].nombre, danioTotal);
            LogBatalla(b);
            
            if (miPk->movimientos[seleccionado].esOHKO) LogBatalla("¡GOLPE FULMINANTE!");
            else if (mult > 1.0f) LogBatalla("¡Es super efectivo!");
            else if (mult < 1.0f && mult > 0) LogBatalla("No es muy efectivo...");

            if (rivalPk->vida <= 0) { 
                rivalPk->vida = 0; 
                rivalPk->estado = DEBILITADO;
                sprintf(b, "¡%s enemigo debilitado!", rivalPk->nombre); LogBatalla(b);
                
                // GANAR EXP
                long expGano = (rivalPk->nivel * 50);
                sprintf(b, "%s gano %ld EXP!", miPk->nombre, expGano); LogBatalla(b);
                int lvlAntes = miPk->nivel;
                GanarExp(miPk, expGano);
                if (miPk->nivel > lvlAntes) {
                    sprintf(b, "¡%s subio al nivel %d!", miPk->nombre, miPk->nivel); LogBatalla(b);
                }

                if (HayPokemonsVivos(rival)) {
                    idxRival = SeleccionarSiguienteRival(rival);
                    sprintf(b, "Rival saco a %s!", rival->pokemons[idxRival].nombre); LogBatalla(b);
                }
            } else {
                // Turno Rival
                int danio = CalcularDanio(rivalPk->movimientos[0], rivalPk, miPk);
                miPk->vida -= danio;
                
                float multR = GetMultiplicador((tipoPokemon)rivalPk->movimientos[0].tipo, miPk->tipo[0]);
                if (miPk->tipo[1] != VACIO) multR *= GetMultiplicador((tipoPokemon)rivalPk->movimientos[0].tipo, miPk->tipo[1]);

                sprintf(b, "Rival uso %s! -%d", rivalPk->movimientos[0].nombre, danio); LogBatalla(b);
                if (multR > 1.0f) LogBatalla("¡Es super efectivo!");
                else if (multR < 1.0f && multR > 0) LogBatalla("No es muy efectivo...");

                if (miPk->vida <= 0) {
                    miPk->vida = 0;
                    miPk->estado = DEBILITADO;
                    sprintf(b, "¡Tu %s se debilito!", miPk->nombre); LogBatalla(b);
                    if (HayPokemonsVivos(jugador)) {
                        idxMi = SeleccionarSiguientePokemon(jugador);
                    }
                }
            }
        }
        DibujarUIEstatica();
        Sleep(100);
    }

    SetRenderState(0); 
    BorrarPantallaCompleta();
    if (!HayPokemonsVivos(jugador)) {
        ImprimirCentrado(10, "HAS PERDIDO EL COMBATE...", 12);
        jugador->stats.combatesSeguidos = 0; // Reset combo
    } else {
        ImprimirCentrado(10, "¡HAS GANADO EL COMBATE!", 10);
        int sumaNiveles = 0;
        int maxLvlRival = 0;
        int tieneFuego = 0;
        for(int i=0; i<rival->cantidadPokemons; i++) {
            sumaNiveles += rival->pokemons[i].nivel;
            if (rival->pokemons[i].nivel > maxLvlRival) maxLvlRival = rival->pokemons[i].nivel;
            if (rival->pokemons[i].tipo[0] == FUEGO || rival->pokemons[i].tipo[1] == FUEGO) tieneFuego = 1;
        }
        
        // Actualizar Estadisticas
        jugador->stats.combatesGanados++;
        jugador->stats.combatesSeguidos++;
        if (maxLvlRival > jugador->stats.maxNivelVencido) jugador->stats.maxNivelVencido = maxLvlRival;
        if (tieneFuego) jugador->stats.vitoriaTipoFuego = 1;
        if (rival->cantidadPokemons == 6) jugador->stats.vitoriaRival6v6 = 1;
        
        // Duelo de Agua: ¿Todo el equipo del jugador es Agua?
        int todoAgua = 1;
        for(int i=0; i<jugador->cantidadPokemons; i++) {
            if(jugador->pokemons[i].tipo[0] != AGUA && jugador->pokemons[i].tipo[1] != AGUA) todoAgua = 0;
        }
        if (todoAgua) jugador->stats.victoriaAguaSoloAgua = 1;

        int recom = sumaNiveles * 25;
        jugador->monedas += recom;
        char b[100]; sprintf(b, "¡Has obtenido $%d por la victoria!", recom);
        ImprimirCentrado(12, b, 14);
    }
    
    ResetearEquipo(jugador);
    ImprimirCentrado(16, "[ VOLVER AL MENU ]", 14);
    while(!mouseEnRango(GetCentroX()-10, 16, GetCentroX()+10, 16, obtenerMouseState())) Sleep(50);
}

void PersonalizarRival(Jugador* rival) {
    BorrarPantallaCompleta();
    int cx = GetCentroX();
    
    // Dibujar elementos estáticos UNA VEZ
    DibujarCajaUTF8(cx - 45, 1, 90, 22, 11);
    ImprimirCentrado(2, "=== EDITOR DE RIVAL ===", 11);
    MoverCursor(cx - 38, 5); CambiarColor(11); printf("EQUIPO RIVAL (Clic para quitar)");
    MoverCursor(cx + 10, 5); CambiarColor(11); printf("BIBLIOTECA (Clic para añadir)");
    
    // Biblioteca Estática
    MoverCursor(cx + 10, 7); printf("1. BULBASAUR  "); ImprimirTipoBadge(PLANTA); printf(" "); ImprimirTipoBadge(VENENO);
    MoverCursor(cx + 10, 8); printf("2. CHARMANDER "); ImprimirTipoBadge(FUEGO);
    MoverCursor(cx + 10, 9); printf("3. SQUIRTLE   "); ImprimirTipoBadge(AGUA);

    while(1) {
        // Redibujar Equipo Rival (Solo zona dinámica)
        for(int i=0; i<6; i++) {
            LimpiarArea(cx - 40, 7 + i, 45, 1); // Limpiar zona de cada fila
            MoverCursor(cx - 40, 7 + i);
            if(i < rival->cantidadPokemons) {
                CambiarColor(15);
                printf("%d. %-12s ", i+1, rival->pokemons[i].nombre);
                ImprimirTipoBadge(rival->pokemons[i].tipo[0]);
                if(rival->pokemons[i].tipo[1] != VACIO) { printf(" "); ImprimirTipoBadge(rival->pokemons[i].tipo[1]); }
            } else {
                CambiarColor(8);
                printf("%d. [ VACIO ]", i+1);
            }
        }

        // Validación y Botón Listo
        LimpiarArea(cx - 30, 15, 60, 1); // Limpiar línea de error
        int btnColor = 10;
        if (rival->cantidadPokemons == 0) {
            ImprimirCentrado(15, "¡ERROR: El rival debe tener al menos 1!", 12);
            btnColor = 8; 
        } 
        ImprimirCentrado(17, "[ LISTO ]", btnColor);

        MouseState m = obtenerMouseState();
        if(mouseEnRango(cx-5, 17, cx+5, 17, m)) {
            if (rival->cantidadPokemons > 0) return;
        }

        // Quitar
        for(int i=0; i<rival->cantidadPokemons; i++) {
            if(mouseEnRango(cx-40, 7+i, cx-5, 7+i, m)) {
                for(int k=i; k<rival->cantidadPokemons-1; k++) rival->pokemons[k] = rival->pokemons[k+1];
                rival->cantidadPokemons--;
                Sleep(200);
            }
        }
        // Añadir
        if(rival->cantidadPokemons < 6) {
            Pokemon p; int add = 0;
            if(mouseEnRango(cx+10, 7, cx+40, 7, m)) { p = crearBulbasaur(); add = 1; }
            else if(mouseEnRango(cx+10, 8, cx+40, 8, m)) { p = crearCharmander(); add = 1; }
            else if(mouseEnRango(cx+10, 9, cx+40, 9, m)) { p = crearSquirtle(); add = 1; }
            if(add) { rival->pokemons[rival->cantidadPokemons++] = p; Sleep(200); }
        }
        Sleep(60);
    }
}

void ConfigurarRivalMenu(Jugador* jugador) {
    static Jugador rival;
    static int rv_init = 0;
    if(!rv_init) {
        strcpy(rival.nombre, "Rival");
        rival.cantidadPokemons = 1;
        rival.pokemons[0] = crearCharmander();
        rv_init = 1;
    }

    BorrarPantallaCompleta();
    int cx = GetCentroX();
    while(1) {
        MoverCursor(0, 0);
        DibujarCajaUTF8(cx - 30, 1, 60, 20, 11);
        ImprimirCentrado(3, "=== CONFIGURACION DE COMBATE ===", 11);
        
        char b[100]; sprintf(b, "Rival tiene %d Pokemon(s)", rival.cantidadPokemons);
        ImprimirCentrado(6, b, 15);

        DibujarBotonCentrado(10, "COMBATIR YA", 10);
        DibujarBotonCentrado(13, "MODIFICAR RIVAL", 11);
        DibujarBotonCentrado(16, "VOLVER", 12);
        
        MouseState mouse = obtenerMouseState();
        if (mouseEnRango(cx-8, 10, cx+8, 10, mouse)) {
            if (rival.cantidadPokemons == 0) {
                ImprimirCentrado(19, "¡ERROR: El rival debe tener al menos 1!", 12);
                Sleep(800); MoverCursor(0, 19); printf("                                         ");
            } else {
                IniciarBatalla(jugador, &rival);
                BorrarPantallaCompleta();
            }
        } else if (mouseEnRango(cx-9, 13, cx+9, 13, mouse)) {
            PersonalizarRival(&rival);
            BorrarPantallaCompleta();
        } else if (mouseEnRango(cx-8, 16, cx+8, 16, mouse)) return;
        Sleep(50);
    }
}
