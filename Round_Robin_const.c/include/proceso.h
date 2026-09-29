#ifndef PROCESO_H
#define PROCESO_H

#include "tipo_elemento.h" // Necesario para empaquetar el proceso

typedef enum
{
    LISTO,
    EJECUTANDOSE,
    FINALIZADO
} EstadoProceso;

typedef struct rProceso
{
    int pid;
    int tiempo_llegada;
    int tiempo_servicio;
    int TS_OG;
    int tiempo_retorno;
    int tiempo_espera;
    EstadoProceso estado;
} rProceso;

// Signatures
rProceso *crearProceso(int pid, int rafaga, int relojGlobal);
void destruirProceso(rProceso *proceso);

#endif // PROCESO_H