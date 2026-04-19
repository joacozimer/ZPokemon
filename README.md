# ZPokemon - Experiencia Táctica en C

ZPokemon es un RPG de combates tácticos desarrollado íntegramente en C, diseñado para ofrecer una experiencia ligera pero profunda en la terminal o mediante renderizado Win32/Android.

## 📜 Historial de Versiones

### 🚀 Versión 1.0.2 (Actual) - "El Desafío de la Torre"
Esta actualización se centra en añadir contenido de juego desafiante y pulir la presentación visual.
- **Modo Torre**: Nuevo modo de juego tipo "gauntlet" con 5 niveles de dificultad creciente y recompensas en gemas al vencer al jefe.
- **IA Dinámica**: Los entrenadores de la torre poseen movimientos aleatorios de todo el registro, aumentando la variedad táctica.
- **Pulido Visual**: Centrado matemático de la pantalla de introducción (Logo ASCII y textos de bienvenida).
- **Estabilidad**: Resolución de errores de compilación y optimización de dependencias internas.

### 🚀 Versión 1.0.1 - "La Gran Expansión"
Esta versión marca el inicio de **ZPokemon** como entidad propia, mejorando la jugabilidad y la organización del proyecto.
- **Identidad**: Renombramiento oficial del proyecto a ZPokemon.
- **Cinemática**: Nueva introducción con arte ASCII y efectos de texto.
- **Z-Pokedex**: Implementación de un sistema de enciclopedia para consultar Pokemons, Movimientos y Objetos.
- **Motor de Batalla Pro**:
    - Soporte para **ataques múltiples** (de 2 a 5 golpes).
    - Sistema de **Buffs/Aumentos de estadísticas** en tiempo real.
    - Movimientos fulminantes (OHKO) como Guillotina.
- **Base de Datos**: 18 movimientos totales y registro centralizado de datos.
- **Sistema de Guardado**: Nuevo formato `.zpk` (PartidaActual.zpk) con gestión de "Nueva Partida" y "Continuar".
- **Mejoras de UI**: Menú de configuración dedicado y limpieza de la tienda.
- **DevOps**: Inclusión de `.gitignore` y optimización de compilación manual.

### 🛡️ Versión 1.0.0 - "Legado"
La base original del proyecto antes de la expansión.
- Menú principal básico con gestión de equipo y PC.
- 3 Starters clásicos: Bulbasaur, Charmander y Squirtle.
- Sistema de combate por turnos con multiplicadores de tipo básicos.
- Guardado en formato `PokemonSave.dat`.
- Tienda básica con pociones y super balas.

---

## ⚠️ Problemas Conocidos (Issues)
- **Soporte Android**: La generación de APK y la estabilidad en dispositivos Android aún no se ha resuelto completamente. El proyecto es funcional principalmente en Windows (Win32).
- **Mecánicas de Objetos**: Aunque los objetos se listan en la Pokedex, su uso activo durante el combate está en desarrollo.

---

## 🗺️ Hoja de Ruta (Roadmap)
Próximas funcionalidades a implementar:
- **Sistema de Objetos**: Uso de Pociones, Antídotos y otros consumibles durante la batalla.
- **Evoluciones**: Implementación del sistema de evolución por nivel para los Starters.
- **Más Pokémon**: Expansión de la Pokédex a los 151 originales.
- **Modo Aventura**: Sistema de mapas básico para exploración y encuentro con entrenadores.
- **IA Rival Avanzada**: Mejora de la lógica del rival para que use cambios de Pokémon y mejores ataques.

---

Desarrollado con ❤️ por Joaco.
