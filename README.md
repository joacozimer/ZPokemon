# ZPokemon - Experiencia Táctica en C
ZPokemon es un RPG de combates tácticos desarrollado íntegramente en C, diseñado para ofrecer una experiencia ligera pero profunda mediante el uso de la librería Raylib.

ZPokemon es un RPG de combates tácticos desarrollado íntegramente en C, diseñado para ofrecer una experiencia ligera pero profunda en la terminal o mediante renderizado Win32/Android.

## 📜 Historial de Versiones

### 🚀 Versión 1.0.3 (Actual) - "Graphical Gold Edition"
Esta actualización marca la cúspide de estabilidad de ZPokemon, consolidando el motor gráfico y el despliegue de recursos.

*   **Pantalla de Carga Dinámica**: Sistema de retroalimentación en tiempo real con barra de progreso y descripción del asset actual. Adiós a las pantallas blancas al inicio.
*   **Reparación Automática de Assets (GDI+ Bridge)**: Sistema inteligente integrado en el compilador que detecta y corrige formatos de imagen incompatibles antes de la generación del ejecutable.
*   **Portabilidad Extrema**: Todos los recursos están embebidos. El ejecutable de 1.0.3 es 100% autónomo y optimizado para Windows.
*   **Interfaz Gráfica Refinada**: 
    *   Escalado dinámico de botones y textos según la resolución.
    *   Nuevos fondos temáticos para Intro, Carga y Menú.
*   **Estabilidad Superior**: Corrección de fallos críticos de memoria al cargar recursos y optimización de decodificación.

---

### 🚀 Versión 1.0.2 - "El Desafío de la Torre"
Esta actualización se centró en añadir contenido de juego desafiante y pulir la presentación visual.

*   **Modo Torre**: Nuevo modo de juego tipo "gauntlet" con 5 niveles de dificultad creciente y recompensas en gemas al vencer al jefe.
*   **IA Dinámica**: Los entrenadores de la torre poseen movimientos aleatorios de todo el registro, aumentando la variedad táctica.
*   **Pulido Visual**: Centrado matemático de la pantalla de introducción (Logo ASCII y textos de bienvenida).
*   **Estabilidad**: Resolución de errores de compilación y optimización de dependencias internas.

    *   Soporte para ataques múltiples (de 2 a 5 golpes).
    *   Sistema de Buffs/Aumentos de estadísticas en tiempo real.
    *   Movimientos fulminantes (OHKO) como Guillotina.
*   **Base de Datos**: 18 movimientos totales y registro centralizado de datos.
*   **Sistema de Guardado**: Nuevo formato .zpk (PartidaActual.zpk) con gestión de "Nueva Partida" y "Continuar".
*   **Mejoras de UI**: Menú de configuración dedicado y limpieza de la tienda.
*   **DevOps**: Inclusión de .gitignore y optimización de compilación manual.

🛡️ Versión 1.0.0 - "Legado"
La base original del proyecto antes de la expansión.

*   Menú principal básico con gestión de equipo y PC.
*   3 Starters clásicos: Bulbasaur, Charmander y Squirtle.
*   Sistema de combate por turnos con multiplicadores de tipo básicos.
*   Guardado en formato PokemonSave.dat.
*   Tienda básica con pociones y super balas.

⚠️ Problemas Conocidos (Issues)
*   Soporte Android: La generación de APK y la estabilidad en dispositivos Android aún no se ha resuelto completamente. El proyecto es funcional principalmente en Windows (Win32).
*   Mecánicas de Objetos: Aunque los objetos se listan en la Pokedex, su uso activo durante el combate está en desarrollo.

🗺️ Hoja de Roadmap (Hoja de Ruta)
Próximas funcionalidades a implementar:

1.  **Sistema de Objetos**: Uso de Pociones, Antídotos y otros consumibles durante la batalla.
2.  **Evoluciones**: Implementación del sistema de evolución por nivel para los Starters.
3.  **Más Pokémon**: Expansión de la Pokédex a los 151 originales.
4.  **Modo Aventura**: Sistema de mapas básico para exploración y encuentro con entrenadores.
5.  **IA Rival Avanzada**: Mejora de la lógica del rival para que use cambios de Pokémon y mejores ataques.
