#ifndef COLAS_H
#define COLAS_H

#include <stdbool.h>
#include "proceso.h"

#define TAMANIO_MAXIMO_COLAS 100 // O el límite que gustes

typedef struct ColaRep *Cola;

Cola c_crear(void);
bool c_es_vacia(Cola cola);
bool c_es_llena(Cola cola);
bool c_encolar(Cola cola, rProceso *proceso);
rProceso *c_desencolar(Cola cola);
rProceso *c_recuperar(Cola cola);
void c_mostrar(Cola cola);
int longitud(Cola cola);

#endif