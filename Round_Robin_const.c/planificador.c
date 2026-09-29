#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "./include/planificador.h"
#include "./include/interfaz.h"

// Variables globales para el estado del procesador simulado
rProceso *proceso_en_ejecucion = NULL;
int quantum_restante = 0;
int reloj_global = 0; // Tiempo total transcurrido

// ---------------------------------------------------------
// FUNCIONES MODULARES (Definiciones locales)
// ---------------------------------------------------------

/* FASE 1: Gestiona el teclado sin bloquear el programa.
 * Retorna 0 si el usuario presiona la tecla de salida, 1 si sigue corriendo. */
int procesar_interrupciones_teclado(Cola cola_listos)
{
    if (_kbhit())
    {
        char tecla = _getch();

        if (tecla == '0')
        {
            return 0; // Señal para apagar el simulador
        }
        else if (tecla == 'a' || tecla == 'A')
        {
            // Entramos en modo ingreso.
            // Acá el tiempo del simulador se "congela" lógicamente mientras el usuario tipea.
            printf("\n--- INTERRUPCION: INGRESO DE PROCESO ---\n");

            // Llamamos a tu función de interfaz para pedir datos por scanf
            ingresar_proceso_dinamico(cola_listos);

            printf("--- REANUDANDO SIMULACION ---\n\n");
        }
    }
    return 1; // Sigue corriendo
}

/* FASE 2: La lógica pura del Round Robin en este milisegundo/tick */
void ejecutar_ciclo_cpu(Cola cola_listos, Cola cola_finalizados, int quantum_base)
{
    // Caso A: La CPU está vacía. Intentamos traer un proceso de la cola de listos.
    if (proceso_en_ejecucion == NULL)
    {
        if (!c_es_vacia(cola_listos))
        {
            TipoElemento te = c_desencolar(cola_listos);
            proceso_en_ejecucion = (rProceso *)te->valor;
            proceso_en_ejecucion->estado = EJECUTANDOSE;
            quantum_restante = quantum_base; // Reiniciamos el quantum para el nuevo proceso

            printf("[Tiempo: %d] CPU: Ejecutando PID %d (TS: %d, Quantum: %d)\n",
                   reloj_global, proceso_en_ejecucion->pid,
                   proceso_en_ejecucion->tiempo_servicio, quantum_restante);
        }
        else
        {
            // No hay procesos para ejecutar. Hacemos un tick "idle" (ocioso).
            // (Opcional: podés imprimir algo, pero llenaría la pantalla).
            return;
        }
    }

    // Caso B: Hay un proceso ejecutándose. Le descontamos tiempo.
    if (proceso_en_ejecucion != NULL)
    {
        proceso_en_ejecucion->tiempo_servicio--;
        quantum_restante--;
        reloj_global++; // Avanzamos el reloj de la CPU

        // Chequeo 1: ¿El proceso terminó su trabajo?
        if (proceso_en_ejecucion->tiempo_servicio <= 0)
        {
            proceso_en_ejecucion->estado = FINALIZADO;
            proceso_en_ejecucion->tiempo_retorno = reloj_global; // Anotamos en qué momento terminó

            printf("[Tiempo: %d] CPU: PID %d FINALIZADO.\n", reloj_global, proceso_en_ejecucion->pid);

            // Lo guardamos en la cola de finalizados y liberamos la CPU
            TipoElemento te = empaquetar_proceso(proceso_en_ejecucion, 0);
            c_encolar(cola_finalizados, te);
            proceso_en_ejecucion = NULL;
        }
        // Chequeo 2: ¿Se le acabó el quantum pero aún le falta tiempo? (Cambio de contexto)
        else if (quantum_restante == 0)
        {
            proceso_en_ejecucion->estado = LISTO;

            printf("[Tiempo: %d] CPU: PID %d AGOTO QUANTUM. Volviendo a cola de listos...\n",
                   reloj_global, proceso_en_ejecucion->pid);

            // Lo volvemos a encolar en los listos y liberamos la CPU
            TipoElemento te = empaquetar_proceso(proceso_en_ejecucion, 0 /* Aca iria tu prioridad */);
            c_encolar(cola_listos, te);
            proceso_en_ejecucion = NULL;
        }
    }
}

// ---------------------------------------------------------
// EL MOTOR (Bucle principal a llamar desde el main)
// ---------------------------------------------------------

void iniciar_simulacion(Cola cola_listos, Cola cola_finalizados, int quantum_base)
{
    int simulador_corriendo = 1;

    printf("==================================================\n");
    printf("   INICIANDO SISTEMA OPERATIVO (ROUND ROBIN)\n");
    printf("==================================================\n");
    printf("- Quantum base: %d\n", quantum_base);
    printf("- Presione 'A' para encolar un proceso nuevo.\n");
    printf("- Presione '0' para apagar el sistema.\n\n");

    // EL BUCLE INFINITO
    while (simulador_corriendo)
    {

        // 1. Verificar si el usuario tocó algo
        simulador_corriendo = procesar_interrupciones_teclado(cola_listos);

        // Si el usuario apretó '0', simulador_corriendo ahora es 0, y saldrá del while.
        if (!simulador_corriendo)
            break;

        // 2. Ejecutar la lógica de planificación
        ejecutar_ciclo_cpu(cola_listos, cola_finalizados, quantum_base);

        // 3. Simular la velocidad del hardware (Delay)
        // 100 ms = 1 tick de reloj. Si lo querés más rápido o más lento, cambiás esto.
        Sleep(100);
    }

    printf("\n==================================================\n");
    printf("   SISTEMA APAGADO. GENERANDO ESTADISTICAS...\n");
    printf("==================================================\n");
}