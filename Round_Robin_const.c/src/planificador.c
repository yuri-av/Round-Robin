#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/proceso.h"
#include "../include/planificador.h"

DWORD WINAPI simuladorRoundRobin(LPVOID arg)
{
    // Hay que desempaquetar el contexto
    ContextoSistema *contexto = (ContextoSistema *)arg;

    int quantum = 2; // El tiempo max por ronda

    // 2. Bucle principal del hilo: corre hasta que el usuario decida salir
    while (contexto->flagTerminar == 0)
    {
        // ========================================================
        // FASE 1: ZONA CRÍTICA - Extraer el proceso de la cola
        // ========================================================
        WaitForSingleObject(contexto->mutex, INFINITE); // Bloqueamos la cola

        if (c_es_vacia(contexto->colaListos))
        {
            ReleaseMutex(contexto->mutex); // Liberamos para no trabar el main
            Sleep(50);                     // Pequeña pausa si no hay procesos
            continue;
        }

        // Sacamos el proceso a ejecutar
        rProceso *pActual = c_desencolar(contexto->colaListos);

        ReleaseMutex(contexto->mutex); // ¡Liberamos el candado antes de simular!

        // ========================================================
        // FASE 2: SIMULACIÓN - Usamos la función procesarQuantum
        // ========================================================
        // Calculamos cuánto tiempo va a consumir realmente en esta ronda
        // (Necesario calcularlo acá porque procesarQuantum devuelve void)
        int tiempoConsumido;
        if (pActual->tiempo_servicio >= quantum)
        {
            tiempoConsumido = quantum;
        }
        else
        {
            tiempoConsumido = pActual->tiempo_servicio;
        }

        // Mostramos por interfaz lo que está pasando
        mostrarEjecucion(pActual, quantum, contexto->relojGlobal);

        // Simulamos la CPU (esta función tiene el Sleep adentro)
        procesarQuantum(pActual, quantum);

        // Actualizamos el reloj del sistema con el tiempo consumido
        contexto->relojGlobal += tiempoConsumido;

        // ========================================================
        // FASE 3: ZONA CRÍTICA - Reubicar el proceso
        // ========================================================
        WaitForSingleObject(contexto->mutex, INFINITE); // Volvemos a bloquear

        if (pActual->tiempo_servicio > 0)
        {
            // Aún le queda ráfaga: lo devolvemos a la cola de listos
            pActual->estado = LISTO;
            c_encolar(contexto->colaListos, pActual);
        }
        else
        {
            // La ráfaga llegó a cero: calculamos métricas y va a terminados
            calcularTiemposFinales(pActual, contexto->relojGlobal);
            c_encolar(contexto->colaTerminados, pActual);
        }

        ReleaseMutex(contexto->mutex); // Liberamos el candado
    }

    return 0; // El hilo del planificador termina exitosamente
}
void procesarQuantum(rProceso *p, int quantum)
{
    int tiempoAEjecutar;

    // 1. Determinamos cuánto tiempo va a correr realmente
    if (p->tiempo_servicio >= quantum)
    {
        tiempoAEjecutar = quantum;
    }
    else
    {
        tiempoAEjecutar = p->tiempo_servicio;
    }

    // 2. Marcamos el proceso como en ejecución
    p->estado = EJECUTANDOSE;

    // 3. Simulamos el tiempo en el procesador (multiplicado por 100 o 1000 para que sea visible)
    Sleep(tiempoAEjecutar * 100);

    // 4. Actualizamos la ráfaga restante
    p->tiempo_servicio -= tiempoAEjecutar;
}
void calcularTiemposFinales(rProceso *p, int tiempoActual)
{
    // 1. Calculamos el tiempo total en el sistema
    p->tiempo_retorno = tiempoActual - p->tiempo_llegada;

    // 2. Calculamos el tiempo que pasó esperando en la cola
    p->tiempo_espera = p->tiempo_retorno - p->TS_OG;

    // 3. Lo marcamos como finalizado
    p->estado = FINALIZADO;
}