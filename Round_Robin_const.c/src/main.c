#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
// El main coordina todo, por lo que incluye los módulos necesarios
#include "../include/planificador.h"
#include "../include/interfaz.h"
#include "../include/proceso.h"
#include "../include/colas.h"

int main(void)
{
    // 1. INICIALIZACIÓN DEL SISTEMA
    ContextoSistema contexto;
    contexto.colaListos = c_crear();
    contexto.colaTerminados = c_crear();
    contexto.relojGlobal = 0;
    contexto.flagTerminar = 0;

    // Creamos el Mutex (FALSE indica que el hilo principal no lo bloquea al crearlo)
    contexto.mutex = CreateMutex(NULL, FALSE, NULL);

    int pidGlobal = 1; // Contador único para los procesos

    // 2. LANZAMIENTO DEL HILO SECUNDARIO
    // Pasamos la dirección de memoria de nuestro contexto (&contexto)
    HANDLE hiloPlanificador = CreateThread(NULL, 0, simuladorRoundRobin, &contexto, 0, NULL);

    if (hiloPlanificador == NULL)
    {
        printf("Error fatal: No se pudo crear el hilo del planificador.\n");
        return 1;
    }

    // 3. BUCLE PRINCIPAL (Interfaz de Usuario)
    printf("====================================================\n");
    printf("   SIMULADOR ROUND ROBIN INICIADO EN SEGUNDO PLANO\n");
    printf("   Presione ENTER en cualquier momento para pausar\n");
    printf("   e ingresar un nuevo proceso al sistema.\n");
    printf("====================================================\n\n");

    while (contexto.flagTerminar == 0)
    {
        // El hilo principal se bloquea pacíficamente aquí sin consumir CPU
        // esperando que el usuario presione Enter.
        getchar();

        // El usuario presionó Enter. Solicitamos el control de la Cola.
        WaitForSingleObject(contexto.mutex, INFINITE);

        // Pedimos los datos (si ingresa ráfaga 0, devuelve NULL)
        rProceso *nuevoProceso = pedirDatosProceso(pidGlobal, contexto.relojGlobal);

        if (nuevoProceso == NULL)
        {
            // Señal de apagado: actualizamos la flag para que el hilo secundario termine
            contexto.flagTerminar = 1;
        }
        else
        {
            // Encolamos el nuevo proceso en la Cola de Listos y preparamos el próximo PID
            c_encolar(contexto.colaListos, nuevoProceso);
            pidGlobal++;
        }

        // ¡Fundamental! Liberamos el candado para que el planificador continúe
        ReleaseMutex(contexto.mutex);

        // Limpiamos el buffer de entrada por si quedó basura del scanf
        fflush(stdin);
    }

    // 4. APAGADO ORDENADO (Clean-up)
    printf("\nApagando sistema, esperando a que el planificador termine su ciclo...\n");

    // Esperamos a que el hilo secundario lea el flagTerminar y finalice su ejecución
    WaitForSingleObject(hiloPlanificador, INFINITE);

    // Mostramos los resultados finales
    imprimirTablaMetricas(contexto.colaTerminados);

    // Liberamos los recursos de Windows
    CloseHandle(hiloPlanificador);
    CloseHandle(contexto.mutex);

    return 0;
}