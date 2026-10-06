#include <stdio.h>
#include <unistd.h>
#include <conio.h> // Funciones nativas de consola en Windows
#include "../include/interfaz.h"
#include "../include/proceso.h"

rProceso *pedirDatosProceso(int pidActual, int relojGlobal)
{
    int rafaga; // Tiempo de servicio del nuevo proceso

    printf("\n-------------------------------------\n");
    printf("  INGRESAR NUEVO PROCESO (PID: %d)\n", pidActual);
    printf("-------------------------------------\n");
    printf("Ingrese rafaga/tiempo de servicio (0 para salir): ");
    scanf("%d", &rafaga);

    if (rafaga <= 0)
    {
        return NULL; // Señal de finalización del programa
    }
    // Llamada con la firma exacta que ya tenés en proceso.h
    return crearProceso(pidActual, rafaga, relojGlobal);
}

void mostrarEjecucion(rProceso *p, int quantum, int relojGlobal)
{
    printf("[Reloj: %d ms] -> Ejecutando PID: %d | Rafaga restante: %d ms | Quantum: %d ms\n",
           relojGlobal, p->pid, p->tiempo_servicio, quantum);
}

void imprimirTablaMetricas(Cola terminados)
{
    int p_terminados = longitud(terminados);
    float TR = 0; // Tiempo retorno
    float TE = 0; // Tiempo espera
    if (c_es_vacia(terminados))
    {
        printf("\nNo se ejecutaron procesos en la simulacion.\n");
        return;
    }
    printf("\n=======================================================================\n");
    printf("                      TABLA FINAL DE METRICAS                          \n");
    printf("=======================================================================\n");
    printf(" PID | T. Llegada | T. Servicio (OG) | T. Retorno | T. Espera\n");
    printf("-----------------------------------------------------------------------\n");
    while (!c_es_vacia(terminados))
    {
        // Sacamos el proceso de la cola listos
        rProceso *proceso = c_desencolar(terminados);
        // Mostramos las metricas individuales
        printf(" %-3d | %-10d | %-16d | %-10d | %-8d\n",
               proceso->pid,
               proceso->tiempo_llegada,
               proceso->TS_OG,
               proceso->tiempo_retorno,
               proceso->tiempo_espera);
        // Sumamos a las metricas
        TR += proceso->tiempo_retorno;
        TE += proceso->tiempo_espera;
        // Liberamos memoria
        destruirProceso(proceso);
    }
    printf("-----------------------------------------------------------------------\n");
    printf("Tiempo Promedio de Espera:  %.2f ms\n", (float)TE / p_terminados);
    printf("Tiempo Promedio de Retorno: %.2f ms\n", (float)TR / p_terminados);
    printf("=======================================================================\n");
}