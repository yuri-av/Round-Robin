#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#define SEPARADOR "-----------------------------------"

typedef enum
{
    NUEVO,
    LISTO,
    CORRIENDO,
    TERMINADO
} EstadoProceso;

const char *obtenerNombreEstado(EstadoProceso estado)
{
    switch (estado)
    {
    case NUEVO: return "NUEVO";
    case LISTO: return "LISTO";
    case CORRIENDO: return "CORRIENDO";
    case TERMINADO: return "TERMINADO";
    default: return "DESCONOCIDO";
    }
}

typedef struct rProceso
{
    int pid;
    int tiempo_servicio;
    int TS_OG;
    int tiempo_retorno;
    int tiempo_espera;
    EstadoProceso estado;
} rProceso;

// Prototipos actualizados
void crearProcesos(rProceso vector[], int cantidad);
void RoundRobin(int Q, rProceso procesos[], int cantidad);
void metricasProcesos(rProceso procesos[], int cantidad);
int scheduler(int proceso_actual, rProceso procesos[], int i);
void setEstado(rProceso procesos[], int proceso_actual, EstadoProceso nuevo_estado);

void mostrarMenu();
void logicaMenu();

int main()
{
    logicaMenu();
    return 0;
}

void crearProcesos(rProceso vector[], int cantidad)
{
    for (int i = 0; i < cantidad; i++)
    {
        rProceso proceso;
        printf("Ingrese el tiempo de servicio del proceso %i: ", i + 1);
        scanf("%i", &proceso.tiempo_servicio);
        
        proceso.TS_OG = proceso.tiempo_servicio;
        proceso.pid = i + 1;
        proceso.estado = NUEVO;
        
        vector[i] = proceso;
    }
}

void RoundRobin(int Q, rProceso procesos[], int cantidad)
{
    bool hay_mas = true;
    int ronda = 1;
    int pant = -1;
    int reloj = 0;

    do
    {
        printf("\n=== RONDA %i ===\n", ronda);
        printf("%s\n", SEPARADOR);

        int finalizados = 0;

        for (int i = 0; i < cantidad; i++)
        {
            if (procesos[i].estado == TERMINADO)
            {
                finalizados++;
                continue; // Salta los procesos que ya terminaron
            }

            // Si es la primera vez que se ejecuta, pasa de NUEVO a LISTO
            if (procesos[i].estado == NUEVO)
            {
                setEstado(procesos, i, LISTO);
            }

            // El scheduler interviene y el proceso pasa a CORRIENDO
            pant = scheduler(pant, procesos, i);
            setEstado(procesos, i, CORRIENDO);

            if (Q >= procesos[i].tiempo_servicio)
            {
                // Finaliza en esta ronda
                int ts_restante = procesos[i].tiempo_servicio;
                reloj += ts_restante;
                procesos[i].tiempo_retorno = reloj;
                
                printf("Proceso %i finalizado en: %i ciclos de reloj\n", procesos[i].pid, procesos[i].tiempo_retorno);

                procesos[i].tiempo_servicio = 0;
                setEstado(procesos, i, TERMINADO);

                procesos[i].tiempo_espera = procesos[i].tiempo_retorno - procesos[i].TS_OG;
                finalizados++;
            }
            else
            {
                // Todavia requiere más ciclos, no finaliza
                printf("Proceso %i: %i(-%i)\n", procesos[i].pid, procesos[i].tiempo_servicio, Q);
                procesos[i].tiempo_servicio -= Q;
                reloj += Q;
                
                // Como se acabó su quantum y no terminó, vuelve a la cola de LISTOS
                setEstado(procesos, i, LISTO);
            }
            
            printf("%s\n", SEPARADOR);
            sleep(1);
        }
        
        sleep(2);
        
        if (finalizados == cantidad)
        {
            hay_mas = false;
        }
        ronda++;
        
    } while (hay_mas);
    
    printf("\n=== PROCESOS FINALIZADOS ===\n\n");
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
    printf("===== T. Retorno Promedio (TRP) =====\n    %.2f    \n", TRP);
    printf("=====================================\n\n");
    printf("===== T. Espera Promedio (TEP) ======\n    %.2f    \n", TEP);
    printf("=====================================\n\n");
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

// Reescritura de cambioEstado para hacerla determinista
void setEstado(rProceso procesos[], int proceso_actual, EstadoProceso nuevo_estado)
{
    if (procesos[proceso_actual].estado != nuevo_estado)
    {
        procesos[proceso_actual].estado = nuevo_estado;
        printf("Estado proceso %i: %s\n", procesos[proceso_actual].pid, obtenerNombreEstado(nuevo_estado));
    }
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
            printf("Ingrese el valor de Q (Quantum): ");
            scanf("%i", &Q);
            while (Q <= 0)
            {
                printf("Q debe valer mas que 0, ingrese nuevamente: ");
                scanf("%i", &Q);
            }

            crearProcesos(procesos, cantidad);
            printf("Procesos creados con exito...\n");
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
