#include "include/structs.h"
#include <cstddef>

// funciones de Gestión de Colas

/*
Una simple forma de gestionar las colas:
- Prioridades tipo FIFO
- Si el siguiente proceso en la lista para entrar en ejecución
no entra, se prueba el siguiente, hasta encontrar uno o terminar
la cola.
- Si un proceso en ejecución pasa a necesitar más núcleos, sale
de ejecución y se pone el primero en la cola.
- Si un proceso deja núcleos libres, se busca el primer proceso en
la cola que pueda entrar en ejecución.


*/

int simple(struct colaProcesos *cola, struct sistema *sistema)
{
    struct colaEventos *colaEventos = generarColaEventos(sistema);
    while ()
    {

        actualizarColaEventos
    }
    return 0;
}

/*  FUNCIONES AUXILIARES    */

/* struct colaEventos *generarColaEventos(struct sistema *sistema)
{
    struct proceso *proceso;
    // struct cambiosNucleos *cambios;
    struct objetoCambio *cambio;

    // por cada proceso de la cola
    for (int i = 0; i < colaProcesos->tamanio; i++)
    {
        proceso = (*colaProcesos).procesos + i * sizeof(struct proceso *);
        for (int j = 0; j < proceso->cambios->tamanio; j++)
        {
            if ()
            {
            }
            else
            {
            }
        }
    }
} */

void actualizarColaEventos(struct colaEventos *colaEventos, struct eventos *e)
{
    // bool seguir = TRUE;
    struct eventos *aux = colaEventos->eventos;
    struct eventos *auxAnterior = NULL;
    struct eventos *auxAnteriorAnterior = NULL;
    while (aux->momento < e->momento && aux != NULL)
    {
        e->momento -= aux->momento;
        auxAnteriorAnterior = auxAnterior;
        auxAnterior = aux;
        aux = aux->siguiente;
    }
    e->siguiente = aux;
    auxAnterior = e;
}

/*
añadir proceso al final de la cola de procesos
*/
void añadirAlFinal(struct colaProcesos *cola, struct proceso *p)
{
    if (cola->tamanio == 0)
    {
        cola->procesos = p;
    }
    else
    {
        struct proceso *aux = cola->procesos;
        while (aux->siguiente != NULL)
        {
            aux = aux->siguiente;
        }
        cola->tamanio++;
        aux->siguiente = p;
    }
    return;
}

/*
añadir proceso al principio de la cola de procesos
*/
void añadirAlPrincipio(struct colaProcesos *cola, struct proceso *p)
{
    if (cola->tamanio == 0)
    {
        cola->procesos = p;
        cola->tamanio++;
    }
    else
    {
        struct proceso *aux = cola->procesos;
        cola->procesos = p;
        p->siguiente = aux;
        cola->tamanio++;
    }
    return;
}
