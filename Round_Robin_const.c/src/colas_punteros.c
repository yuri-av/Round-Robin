#include "colas.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// Definición de las estructuras internas del TAD
struct Nodo
{
    rProceso *datos;
    struct Nodo *siguiente;
};

struct ColaRep
{
    struct Nodo *frente;
    struct Nodo *final;
};

// Crear una cola vacía
Cola c_crear(void)
{
    Cola nueva_cola = (Cola)malloc(sizeof(struct ColaRep));
    if (nueva_cola != NULL)
    {
        nueva_cola->frente = NULL;
        nueva_cola->final = NULL;
    }
    return nueva_cola;
}

// Verificar si la cola está vacía
bool c_es_vacia(Cola cola)
{
    return (cola == NULL || cola->frente == NULL);
}

// Rutina interna que calcula los elementos de la cola
int longitud(Cola cola)
{
    int i = 0;
    struct Nodo *N = (cola != NULL) ? cola->frente : NULL;
    while (N != NULL)
    {
        i++;
        N = N->siguiente;
    }
    return i;
}

// Verificar si la cola alcanzó el tope máximo (si está definido)
bool c_es_llena(Cola cola)
{
    return (longitud(cola) >= TAMANIO_MAXIMO_COLAS);
}

// Insertar un proceso al final de la cola
bool c_encolar(Cola cola, rProceso *proceso)
{
    if (cola == NULL || c_es_llena(cola))
    {
        return false;
    }

    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    if (nuevo_nodo == NULL)
    {
        return false; // Sin memoria RAM disponible
    }

    nuevo_nodo->datos = proceso;
    nuevo_nodo->siguiente = NULL;

    if (c_es_vacia(cola))
    {
        cola->frente = nuevo_nodo;
    }
    else
    {
        cola->final->siguiente = nuevo_nodo;
    }

    cola->final = nuevo_nodo;
    return true;
}

// Extraer el primer proceso del frente
rProceso *c_desencolar(Cola cola)
{
    if (c_es_vacia(cola))
    {
        return NULL;
    }

    struct Nodo *inicio = cola->frente;
    rProceso *proceso = inicio->datos;

    cola->frente = inicio->siguiente;

    // Si la cola se vació, reseteamos el puntero final
    if (cola->frente == NULL)
    {
        cola->final = NULL;
    }

    free(inicio);
    return proceso;
}

// Obtener el proceso del frente sin sacarlo de la cola
rProceso *c_recuperar(Cola cola)
{
    if (c_es_vacia(cola))
    {
        return NULL;
    }
    return cola->frente->datos;
}

// Mostrar los procesos en cola
void c_mostrar(Cola cola)
{
    if (c_es_vacia(cola))
    {
        printf("[ Cola de Listos Vacia ]\n");
        return;
    }

    printf("-----------------------------------------\n");
    printf(" PID | Rafaga Restante | Rafaga Original\n");
    printf("-----------------------------------------\n");

    struct Nodo *actual = cola->frente;
    while (actual != NULL)
    {
        rProceso *p = actual->datos;
        printf(" %-3d | %-15d | %-15d\n", p->pid, p->tiempo_servicio, p->TS_OG);
        actual = actual->siguiente;
    }
    printf("-----------------------------------------\n");
}