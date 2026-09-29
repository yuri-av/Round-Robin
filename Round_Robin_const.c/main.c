#include <stdio.h>
#include "./colas/headers/colas.h"
#include "./include/planificador.h"
#include "./include/interfaz.h"

int main()
{
    // 1. Inicializar estructuras
    Cola cola_listos = c_crear();
    Cola cola_finalizados = c_crear();

    // (Opcional) Acá podrías agregar 2 o 3 procesos iniciales a la cola
    // de listos por defecto para que la simulación no empiece vacía.

    int quantum_del_sistema = 3; // El quantum que le quieras dar

    // 2. Arrancar el motor
    iniciar_simulacion(cola_listos, cola_finalizados, quantum_del_sistema);

    // 3. Al terminar (cuando el usuario presione 0)
    // imprimir_estadisticas_finales(cola_finalizados);

    return 0;
}