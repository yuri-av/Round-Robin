#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H
#include <windows.h>
#include "proceso.h"
#include "colas.h"
#include "interfaz.h"

// Esto es lo que le pasamos al hilo
typedef struct ContextoSistema
{
    Cola colaListos;
    Cola colaTerminados;
    HANDLE mutex;
    int relojGlobal;
    int flagTerminar; // 0 = Corriendo, 1 = Apagar sistema
} ContextoSistema;

// Signatures
DWORD WINAPI simuladorRoundRobin(LPVOID arg);
void procesarQuantum(rProceso *p, int quantum);
void calcularTiemposFinales(rProceso *p, int tiempoActual);

#endif // PLANIFICADOR_H