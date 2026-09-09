#include <stdio.h>
#include <stdbool.h>

typedef struct rProceso
{
    int pid;             // Process ID
    int tiempo_servicio; // Tiempo de Servicio o TS (se restará en el proceso)
    int TS_OG;           // TS original
    int tiempo_retorno;  // Tiempo reloj que se necesitó para finalizarlo junto con otros procesos
    int tiempo_espera;   // TR - TS
    bool estado;         // En proceso: True, Finalizado: False
} rProceso;

void crearProcesos(rProceso vector[], int cantidad);
void RoundRobin(int Q, rProceso procesos[], int cantidad);
void metricasProcesos(rProceso procesos[], int cantidad);
int scheduler(int proceso_actual, rProceso procesos[], int i);

void mostrarMenu();
void logicaMenu();

int main()
{
    logicaMenu();
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
        proceso.tiempo_servicio = tiempoServicio;
        proceso.TS_OG = tiempoServicio;
        // Definimos id del servicio
        proceso.pid = i + 1;
        // Ponemos estado del proceso (true por default)
        proceso.estado = true;
        // Agregamos al vector que los contiene
        vector[i] = proceso;
    }
    return;
}

void RoundRobin(int Q, rProceso procesos[], int cantidad)
{
    bool hay_mas = true;
    int ronda = 1;
    int pant = -1;
    // Tiempo total transcurrido
    int reloj = 0;
    // Entra en el while para poder crear tantas rondas como sea necesario
    do
    {
        printf("=== RONDA %i ===\n", ronda);
        // Se reinicia con cada ronda para evitar errores
        int finalizados = 0;
        for (int i = 0; i < cantidad; i++)
        {
            int proceso_actual = i;
            // Chequeamos que en una ronda anterior ya haya finalizado
            if (procesos[i].estado == false)
            {
                finalizados++;
            }
            // Finalizó en esta ronda
            else if (Q >= procesos[i].tiempo_servicio && procesos[proceso_actual].estado == true)
            {
                // Chequeamo si ya habia pasado por este proceso en la ronda anterior
                pant = scheduler(pant, procesos, proceso_actual);
                int ts_restante = procesos[proceso_actual].tiempo_servicio;
                // Le sumamos al reloj el Q que se utilizó para este proceso
                reloj += ts_restante;
                procesos[proceso_actual].tiempo_retorno = reloj;
                printf("Proceso %i finalizado en: %i ciclos\n",
                       procesos[proceso_actual].pid,
                       procesos[proceso_actual].tiempo_retorno);
                // Si el TS es menor a Q, entonces se hace TS - TS = 0
                procesos[proceso_actual].tiempo_servicio -= ts_restante;
                // Cambiamos el estado del proceso a finalizado
                procesos[proceso_actual].estado = false;
                // Se calcula el TE del proceso (TR - TS Original)
                procesos[proceso_actual].tiempo_espera = procesos[i].tiempo_retorno - procesos[proceso_actual].TS_OG;
                finalizados++;
            }
            // Todavia hay espacio para otra ronda para este
            else
            {
                // Chequeamo si ya habia pasado por este proceso en la ronda anterior
                pant = scheduler(pant, procesos, proceso_actual);

                printf("Proceso %i: %i(-%i)\n", i + 1,
                       procesos[i].tiempo_servicio - Q, Q);
                procesos[i].tiempo_servicio -= Q;
                reloj += Q;
            }
        }
        // Tiempo de espera de 2 segundos
        sleep(2);
        // Si finalizados es igual a la cantidad de elementos del vector, no hay mas rondas que hacer
        if (finalizados == cantidad)
        {
            hay_mas = false;
        }
        // Si sigue se aumenta el numero de la ronda
        ronda++;
    } while (hay_mas);
    printf("=== PROCESOS FINALIZADOS ===\n\n");
    return;
}

void metricasProcesos(rProceso procesos[], int cantidad)
{
    float TRP = 0, TEP = 0;
    for (int i = 0; i < cantidad; i++)
    {
        TRP += procesos[i].tiempo_retorno;
        TEP += procesos[i].tiempo_espera;
    }
    TRP = TRP / cantidad;
    TEP = TEP / cantidad;
    printf("===== TRP =====\n     %.2f    \n", TRP);
    printf("===============\n\n");
    printf("===== TEP =====\n     %.2f    \n", TEP);
    printf("===============\n\n");
    return;
}

int scheduler(int pant, rProceso procesos[], int proceso_actual)
{
    int nuevo_proceso = pant;
    if (pant != procesos[proceso_actual].pid)
    {
        nuevo_proceso = procesos[proceso_actual].pid;
        printf("El Scheduler cambia al proceso %i\n", nuevo_proceso);
    }
    return nuevo_proceso;
}

// FUNCIONES DE MENU

void mostrarMenu()
{
    printf("\n----------MENU----------\n");
    printf("Agregar procesos (1)\n");
    printf("Ejecutar Round Robin (2)\n");
    printf("Salir (0)\n");
    printf("------------------------\n");
}

void logicaMenu()
{
    int opcion = -1;
    int cantidad = 0;
    int Q;
    rProceso procesos[100];

    while (opcion != 0)
    {
        mostrarMenu();
        printf("Ingresar opcion: ");
        scanf("%i", &opcion);
        switch (opcion)
        {
        case 1:
            printf("Ingrese la cantidad de procesos que se realizaran: ");
            scanf("%i", &cantidad);
            while (cantidad <= 0 || cantidad > 100)
            {
                printf("Valor numerico no valido, ingresar un entero mayor a 0 y menor a 100: ");
                scanf("%i", &cantidad);
            }
            printf("Ingrese el valor de Q: ");
            scanf("%i", &Q);
            while (Q <= 0)
            {
                printf("Q debe valer mas que 0, ingrese nuevamente: ");
                scanf("%i", &Q);
            }

            crearProcesos(procesos, cantidad);
            printf("Procesos creados con exito...\n");
            // printf("%i",opcion);
            break;
        case 2:
            if (cantidad < 1)
            {
                printf("Primero debe agregar procesos para ejecutar el Round Robin\n");
            }
            else
            {
                RoundRobin(Q, procesos, cantidad);
                metricasProcesos(procesos, cantidad);
            }
            break;
        case 0:
            printf("\nFin del programa...\n");
            break;
        default:
            printf("La opcion no es valida\n");
            break;
        }
    }
}
