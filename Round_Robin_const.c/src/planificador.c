#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/proceso.h"
#include "../include/planificador.h"

DWORD WINAPI simuladorRoundRobin(LPVOID arg);
void procesarQuantum(rProceso *p, int quantum);
void calcularTiemposFinales(rProceso *p, int tiempoActual);