#include <stdio.h>
#include <stdbool.h>

typedef struct rProceso
{
    int id;
    int tiempo_proceso;
    bool estado;
} rProceso;

void crearProcesos(rProceso vector[], int cantidad);
void RoundRobin(int Q, rProceso procesos[], int cantidad);

int main()
{
    int cantidad;
    int Q;

    printf("Ingrese la cantidad de procesos que se realizaran: ");
    scanf("%i", &cantidad);

    printf("Ingrese el valor de Q: ");
    scanf("%i", &Q);
    rProceso procesos[cantidad];

    crearProcesos(procesos, cantidad);
    RoundRobin(Q, procesos, cantidad);

    return 0;
}

void crearProcesos(rProceso vector[], int cantidad)
{
    int i;
    int tiempoServicio;
    rProceso proceso;
    for (i = 0; i < cantidad; i++)
    {
        printf("Ingrese el tiempo de servicio del proceso %i: ", i + 1);
        scanf("%i", &tiempoServicio);
        // Definimos tiempo de servicio de proceso n
        proceso.tiempo_proceso = tiempoServicio;
        // Definimos id del servicio
        proceso.id = i + 1;
        // Ponemos estado del proceso (true por default)
        proceso.estado = true;
        // Agregamos al vector que los contiene
        vector[i] = proceso;
    }
    return;
}

void RoundRobin(int Q, rProceso procesos[], int cantidad)
{
    // Necesito estas métricas al final del
    // float TRP, TR, TE, TEP;
    bool hay_mas = true;
    int ronda = 1;
    // Entra en el while para poder crear tantas rondas como sea necesario
    do
    {
        printf("=== RONDA %i ===\n", ronda);
        // Se reinicia con cada ronda para evitar errores
        int finalizados = 0;
        for (int i = 0; i < cantidad; i++)
        {
            // Chequeamos que en una ronda anterior ya haya finalizado
            // (se reinicia con cada ronda para evitar errores)
            if (procesos[i].estado == false)
            {
                finalizados++;
            }
            // Finalizó en esta ronda
            else if (Q >= procesos[i].tiempo_proceso && procesos[i].estado == true)
            {
                printf("Proceso %i finalizado!\n", i + 1);
                procesos[i].tiempo_proceso = 0;
                procesos[i].estado = false;
                finalizados++;
            }
            // Todavia hay espacio para otra ronda para este
            else
            {
                printf("Proceso %i: %i(-%i)\n", i + 1, procesos[i].tiempo_proceso - Q, Q);
                procesos[i].tiempo_proceso -= Q;
            }
        }
        sleep(3);
        // Si finalizados es igual a la cantidad de elementos del vector, no hay mas rondas que hacer
        if (finalizados == cantidad)
        {
            hay_mas = false;
        }
        // Si sigue se aumenta el numero de la ronda
        ronda++;
    } while (hay_mas);
    printf("=== PROCESOS FINALIZADOS ===\n");
    return;
}