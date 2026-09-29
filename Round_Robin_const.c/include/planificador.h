#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H
#include <windows.h>
#include "proceso.h"
#include "colas.h"
#include "interfaz.h"

// Signatures
DWORD WINAPI simuladorRoundRobin(LPVOID arg);
void procesarQuantum(rProceso *p, int quantum);
void calcularTiemposFinales(rProceso *p, int tiempoActual);

#endif // PLANIFICADOR_H