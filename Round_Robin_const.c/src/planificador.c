#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "proceso.h"
#include "planificador.h"

DWORD WINAPI simuladorRoundRobin(LPVOID arg);
void procesarQuantum(rProceso *p, int quantum);
void calcularTiemposFinales(rProceso *p, int tiempoActual);