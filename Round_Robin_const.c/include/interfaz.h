#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "colas.h"

// Signatures
rProceso *pedirDatosProceso(int pidActual, int relojGlobal);
void mostrarEjecucion(rProceso *p, int quantum, int relojGlobal);
void imprimirTablaMetricas(Cola terminados);
#endif // INTERFAZ_H
