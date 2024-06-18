#include "../include/structs.h"
// #include <cstddef>
#include <stdlib.h>
#include <stdio.h>
#include "../include/func.h"

// funciones de Gestión de Colas

/*
Una simple forma de gestionar las colas:
- Si el siguiente proceso en la lista para entrar en ejecución
no entra, se prueba el siguiente, hasta encontrar uno o terminar
la cola.
- Si un proceso en ejecución pasa a necesitar más o menos núcleos, sale
de ejecución y se pone el ultimo en la cola.
*/

void fifo(struct colaProcesos *colaProcesos, struct colaEventos *colaEventos, struct sistema *sistema)
{

    struct proceso *p = colaProcesos->procesos;
    struct proceso *siguiente = NULL;
    if (colaProcesos->tamanio > 1)
        siguiente = colaProcesos->procesos->siguiente;
    struct momento *m;
    while (p != NULL)
    {
        quitarProceso(colaProcesos, p);
        if (sistema->cantNucleosLibres == 0)
            break;
        if (p->nucleos < sistema->cantNucleosLibres)
        {
            // añadir a la lista de procesos en ejecucion
            anadirAlFinal(sistema->procesosEjec, p);
            // actualizar los nucleos libres
            sistema->cantNucleosLibres -= p->nucleos;
            // añadir el evento en el que termina el proceso
            m = (struct momento *)malloc(sizeof(struct momento));

            m->momento = p->tiempoEjec;
            m->numeroEventos = 1;
            m->evento = (struct evento *)malloc(sizeof(struct evento));
            m->evento->proceso = p;
            m->evento->tipo = 0;
            m->siguienteMomento = NULL;
            m->evento->siguiente = NULL;

            actualizarColaEventos(colaEventos, m);
            struct objetoCambio *cambio = p->cambios->cambios;
            while (cambio != NULL)
            {
                m = (struct momento *)malloc(sizeof(struct momento));
                m->momento = cambio->momentoCambio;
                m->evento = (struct evento *)malloc(sizeof(struct evento));
                m->evento->proceso = p;
                m->siguienteMomento = NULL;
                m->evento->siguiente = NULL;
                switch (cambio->incrementar)
                {
                case 1:
                    m->evento->tipo = 1;
                    m->evento->factor = cambio->factor;
                    break;
                case 0:
                    m->evento->tipo = 2;
                    m->evento->factor = cambio->factor;
                    break;
                default:
                    break;
                }
                actualizarColaEventos(colaEventos, m);
                cambio = cambio->siguiente;
            }
        }
        // quitarProceso(colaProcesos, p);
        p = siguiente;
        if (colaProcesos->tamanio > 1)
        {
            siguiente = siguiente->siguiente;
        }
        else
        {
            siguiente = NULL;
        }
    }
    return;
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

void actualizarColaEventos(struct colaEventos *colaEventos, struct momento *e)
{
    if (colaEventos->tamanio == 0)
    {
        colaEventos->eventos = e;
    }
    else
    {

        struct momento *aux = colaEventos->eventos;
        struct momento *auxAnterior = NULL;
        // struct momento *auxAnteriorAnterior = NULL;
        while (aux != NULL)
        {
            if (aux->momento > e->momento)
            {
                aux->momento -= e->momento;
                break;
            }
            e->momento -= aux->momento;
            // auxAnteriorAnterior = auxAnterior;
            auxAnterior = aux;
            aux = aux->siguienteMomento;
        }
        e->siguienteMomento = aux;
        auxAnterior->siguienteMomento = e;
    }
    colaEventos->tamanio++;
}

/*
añadir proceso al final de la cola de procesos
*/
void anadirAlFinal(struct colaProcesos *cola, struct proceso *p)
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

        aux->siguiente = p;
        // aux->siguiente->siguiente = NULL;
    }
    cola->tamanio++;
    return;
}

/*
añadir proceso al principio de la cola de procesos
*/
void anadirAlPrincipio(struct colaProcesos *cola, struct proceso *p)
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

int quitarProceso(struct colaProcesos *cola, struct proceso *p)
{
    if (cola->tamanio == 0)
    {
        return 1;
    }
    else if (cola->tamanio == 1)
    {
        cola->procesos = NULL;
        cola->tamanio = 0;
        return 0;
    }
    else if (cola->procesos == p)
    {
        cola->procesos = cola->procesos->siguiente;
        cola->tamanio--;
        p->siguiente = NULL;
        return 0;
    }
    else
    {
        struct proceso *aux = cola->procesos->siguiente->siguiente;
        struct proceso *auxAnterior = cola->procesos->siguiente;
        while (aux != NULL)
        {
            if (aux == p)
            {
                // quitar proceso
                auxAnterior->siguiente = aux->siguiente;
                aux->siguiente = NULL;
                cola->tamanio--;
                return 0;
            }
        }
    }
    return 2;
}

int quitarEvento(struct colaEventos *colaEventos, struct evento *evento)
{
    if (colaEventos->tamanio == 0)
    {
        return 1;
    }
    else if (colaEventos->eventos->evento == evento)
    {
        colaEventos->eventos = colaEventos->eventos->siguienteMomento;
        colaEventos->tamanio--;
        return 0;
    }
    else
    {
        struct momento *aux = colaEventos->eventos->siguienteMomento;
        struct momento *auxAnterior = colaEventos->eventos;
        while (aux != NULL)
        {
            if (aux->evento == evento)
            {
                // quitar evento
                auxAnterior->siguienteMomento = aux->siguienteMomento;
                aux->siguienteMomento = NULL;
                colaEventos->tamanio--;
                return 0;
            }
        }
    }
    return 2;
}