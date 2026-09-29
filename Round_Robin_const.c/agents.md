# Contexto del Proyecto: Simulador de Planificador Round Robin

Este proyecto es una simulación en C de un planificador de procesos de Sistema Operativo utilizando el algoritmo Round Robin con soporte para prioridades. Su característica principal es que se ejecuta de manera constante mediante un bucle infinito guiado por "ticks" de reloj, permitiendo la inserción asíncrona de nuevos procesos en tiempo de ejecución sin bloquear el simulador (implementado específicamente para Windows usando `<conio.h>` y `<windows.h>`).

## Arquitectura y Responsabilidades de los Archivos

El sistema está estrictamente modularizado. Basado en la estructura del espacio de trabajo[cite: 1], las responsabilidades son las siguientes:

*   **`main.c`**: Es el orquestador del sistema. Contiene el bucle infinito (el reloj del CPU). Su única responsabilidad es consultar si hay entrada del usuario, llamar al planificador para ejecutar un ciclo, e invocar el retardo de tiempo (`Sleep`). No contiene lógica de Round Robin.
*   **`planificador.c` / `include/planificador.h`**: Contiene la lógica pura del motor Round Robin. Decide qué proceso entra a la CPU, descuenta el quantum, descuenta el tiempo de servicio y gestiona los cambios de contexto (mover procesos entre las colas de Listos y Finalizados).
*   **`interfaz.c` / `include/interfaz.h`**: Maneja exclusivamente la entrada y salida (I/O). Encapsula las funciones no bloqueantes (`_kbhit()`, `_getch()`) para leer el teclado en Windows, e incluye las funciones para imprimir el estado del sistema por consola.
*   **`proceso.c` / `include/proceso.h`**: Define el dominio del proceso (estructura `rProceso` y estados). Gestiona la creación, empaquetado y destrucción de las entidades de proceso.
*   **`colas/` (TAD Cola)**: Contiene los headers (`colas.h`) y las implementaciones (`colas_arreglos_circular.c`, `colas_punteros.c`, etc.) de la estructura de datos Cola[cite: 1]. Es un TAD genérico que no conoce el concepto de "proceso".
*   **`tipoElemento/` (TAD TipoElemento)**: Contiene la definición (`tipo_elemento.h`) y lógica (`tipo_elemento.c`) para empaquetar datos genéricos usando un `int clave` y un `void *valor`[cite: 1]. Sirve de puente entre la Cola y los Procesos.

## Cómo se comunican los módulos

1.  El motor arranca en `main.c`, inicializando las colas (TAD).
2.  En cada iteración (tick de 1 segundo), `main.c` invoca a `verificar_input_asincrono()` en `interfaz.c`.
3.  Si hay input para crear un proceso, `interfaz.c` recolecta los datos, usa `proceso.c` para instanciar el `rProceso`, lo envuelve con `tipoElemento`, y lo encola usando el TAD `colas`.
4.  Luego, `main.c` llama a `ejecutar_ciclo_planificador()` en `planificador.c`, pasándole la CPU actual, las colas y el estado del quantum.
5.  `planificador.c` manipula los punteros a proceso y mueve elementos en las colas según las reglas de Round Robin, imprimiendo los cambios de estado.
6.  `main.c` pausa la ejecución (`Sleep`) y repite el ciclo.

## Reglas Estrictas de Modificación (Qué NO tocar)

*   **No modificar los TADs base (`colas/` y `tipoElemento/`):** Estas estructuras ya están programadas y probadas. Todo elemento que ingrese a la cola debe estar envuelto en un `TipoElemento`.
*   **No introducir bloqueos en `main.c` o `planificador.c`:** El sistema debe seguir corriendo de manera continua. Prohibido usar `scanf` o `getchar` de forma síncrona en el bucle principal. Toda lectura de teclado debe pasar por el chequeo previo de `_kbhit()`.
*   **No mezclar responsabilidades:** `planificador.c` no debe leer del teclado. `main.c` no debe descontar quantum ni acceder directamente a `tiempo_servicio`.
*   **Inclusiones:** Respetar los include guards y las rutas relativas. Evitar dependencias circulares (ej. no incluir `planificador.h` dentro de `proceso.h`). Si es necesario compilar, tener en cuenta la ruta de los headers de los TADs (ej: `./tipoElemento/headers/tipo_elemento.h`[cite: 1]).