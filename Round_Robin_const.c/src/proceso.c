#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/proceso.h"
#define SEPARADOR "-----------------------------------"

typedef enum
{
    NUEVO_INTERNO,
    LISTO_INTERNO,
    CORRIENDO_INTERNO,
    TERMINADO_INTERNO
} EstadoProcesoInterno;

const char *obtenerNombreEstado(EstadoProcesoInterno estado)
{
    switch (estado)
    {
    case NUEVO_INTERNO:
        return "NUEVO";
    case LISTO_INTERNO:
        return "LISTO";
    case CORRIENDO_INTERNO:
        return "CORRIENDO";
    case TERMINADO_INTERNO:
        return "TERMINADO";
    default:
        return "DESCONOCIDO";
    }
}
rProceso *crearProceso(int pid, int rafaga, int relojGlobal)
{
    // 1. Reservamos memoria dinámica para la estructura
    rProceso *proceso_nuevo = (rProceso *)malloc(sizeof(rProceso));

    if (proceso_nuevo == NULL)
    {
        return NULL; // Error de memoria RAM insuficiente
    }

    // 2. Asignamos los datos usando la sintaxis de flecha (->)
    proceso_nuevo->pid = pid;
    proceso_nuevo->tiempo_servicio = rafaga;
    proceso_nuevo->TS_OG = rafaga;
    proceso_nuevo->tiempo_llegada = relojGlobal;
    proceso_nuevo->tiempo_retorno = 0;
    proceso_nuevo->tiempo_espera = 0;
    proceso_nuevo->estado = LISTO;

    // 3. Devolvemos el puntero directamente
    return proceso_nuevo;
}

void destruirProceso(rProceso *proceso)
{
    if (proceso != NULL)
    {
        free(proceso);
    }
}