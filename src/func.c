#include "../include/structs.h"
#include <stdlib.h>
#include <stdio.h>
#include "../include/func.h"
#include <stdbool.h>

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

    if (colaProcesos->tamanio > 0)
    {
        struct proceso *p = colaProcesos->procesos;
        struct proceso *siguiente = NULL;
        if (colaProcesos->tamanio > 1)
            siguiente = colaProcesos->procesos->siguiente;
        struct momento *m;
        int i = 0;
        while (p != NULL && i < colaProcesos->tamanio)
        {
            if (sistema->cantNucleosLibres == 0)
                break;
            if (p->nucleos <= sistema->cantNucleosLibres)
            {
                quitarProceso(colaProcesos, p);
                // añadir a la lista de procesos en ejecucion
                anadirAlFinal(sistema->procesosEjec, p);
                // actualizar los nucleos libres
                sistema->cantNucleosLibres -= p->nucleos;
                // añadir el evento en el que termina el proceso
                m = (struct momento *)malloc(sizeof(struct momento));

                m->momento = p->tiempoParaTerminar;
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
                    if (cambio->procesado == 0)
                    {
                        m = (struct momento *)malloc(sizeof(struct momento));
                        m->momento = cambio->momentoCambio;
                        m->evento = (struct evento *)malloc(sizeof(struct evento));
                        m->evento->proceso = p;
                        m->siguienteMomento = NULL;
                        m->evento->siguiente = NULL;
                        m->evento->factor = cambio->factor;
                        switch (cambio->incrementar)
                        {
                        case 1:
                            m->evento->tipo = 1;
                            break;
                        case 0:
                            m->evento->tipo = 2;
                            break;
                        default:
                            break;
                        }
                        actualizarColaEventos(colaEventos, m);
                    }
                    cambio = cambio->siguiente;
                }
            }
            else
            {
                i++;
            }
            p = siguiente;
            if (colaProcesos->tamanio > 1 && siguiente != NULL)
            {
                siguiente = siguiente->siguiente;
            }
            else
            {
                siguiente = NULL;
            }
        }
    }
    return;
}

/*
- Si el siguiente proceso en la lista para entrar en ejecución
no entra, se prueban los K siguientes.
- Si un proceso en ejecución pasa a necesitar más o menos núcleos, sale
de ejecución y se pone el primero en la cola.
*/

void bf(struct colaProcesos *colaProcesos, struct colaEventos *colaEventos, struct sistema *sistema, int K)
{

    if (colaProcesos->tamanio > 0)
    {
        struct proceso *p = colaProcesos->procesos;
        struct proceso *siguiente = NULL;
        if (colaProcesos->tamanio > 1)
            siguiente = colaProcesos->procesos->siguiente;
        struct momento *m;
        int i = 0;
        while (p != NULL && i < K)
        {
            if (sistema->cantNucleosLibres == 0)
                break;
            if (p->nucleos <= sistema->cantNucleosLibres)
            {
                quitarProceso(colaProcesos, p);
                // añadir a la lista de procesos en ejecucion
                anadirAlFinal(sistema->procesosEjec, p);
                // actualizar los nucleos libres
                sistema->cantNucleosLibres -= p->nucleos;
                // añadir el evento en el que termina el proceso
                m = (struct momento *)malloc(sizeof(struct momento));

                m->momento = p->tiempoParaTerminar;
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
                    if (cambio->procesado == 0)
                    {
                        m = (struct momento *)malloc(sizeof(struct momento));
                        m->momento = cambio->momentoCambio;
                        m->evento = (struct evento *)malloc(sizeof(struct evento));
                        m->evento->proceso = p;
                        m->siguienteMomento = NULL;
                        m->evento->siguiente = NULL;
                        m->evento->factor = cambio->factor;
                        switch (cambio->incrementar)
                        {
                        case 1:
                            m->evento->tipo = 1;
                            break;
                        case 0:
                            m->evento->tipo = 2;
                            break;
                        default:
                            break;
                        }
                        actualizarColaEventos(colaEventos, m);
                    }
                    cambio = cambio->siguiente;
                }
            }
            else
            {
                i++;
            }
            p = siguiente;
            if (colaProcesos->tamanio > 1 && siguiente != NULL)
            {
                siguiente = siguiente->siguiente;
            }
            else
            {
                siguiente = NULL;
            }
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

// FUNCIONA PERFECTAMENTE
void actualizarColaEventos(struct colaEventos *colaEventos, struct momento *e)
{
    if (colaEventos->tamanio == 0)
    {
        colaEventos->eventos = e;
        colaEventos->tamanio++;
        colaEventos->eventos->siguienteMomento = NULL;
        return;
    }
    else if (colaEventos->tamanio == 1)
    {
        if (colaEventos->eventos->momento > e->momento)
        {
            colaEventos->eventos->momento -= e->momento;
            e->siguienteMomento = colaEventos->eventos;
            colaEventos->eventos = e;
        }
        else
        {
            e->momento -= colaEventos->eventos->momento;
            colaEventos->eventos->siguienteMomento = e;
        }
        colaEventos->tamanio++;
        return;
    }
    else
    {
        if (colaEventos->eventos->momento > e->momento)
        {
            colaEventos->eventos->momento -= e->momento;
            e->siguienteMomento = colaEventos->eventos;
            colaEventos->eventos = e;
        }
        else
        {

            struct momento *aux = colaEventos->eventos;
            struct momento *auxAnterior = NULL;
            while (aux != NULL)
            {
                if (aux->momento > e->momento)
                {
                    aux->momento -= e->momento;
                    break;
                }
                e->momento -= aux->momento;
                auxAnterior = aux;
                aux = aux->siguienteMomento;
            }
            e->siguienteMomento = aux;
            auxAnterior->siguienteMomento = e;
        }
    }
    colaEventos->tamanio++;
}

/*
añadir proceso al final de la cola de procesos
*/

// FUNCIONA PERFECTAMENTE
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

// FUNCIONA PERFECTAMENTE
int quitarProceso(struct colaProcesos *cola, struct proceso *p)
{
    if (cola->tamanio == 0)
    {
        return 1;
    }
    else if (cola->tamanio == 1)
    {
        if (cola->procesos->pid == p->pid)
        {
            cola->procesos = NULL;
            cola->tamanio = 0;
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else if (cola->procesos->pid == p->pid)
    {
        cola->procesos = cola->procesos->siguiente;
        cola->tamanio--;
        p->siguiente = NULL;
        return 0;
    }
    else
    {
        struct proceso *aux = cola->procesos->siguiente;
        struct proceso *auxAnterior = cola->procesos;
        while (aux != NULL)
        {
            if (aux->pid == p->pid)
            {
                // quitar proceso
                auxAnterior->siguiente = aux->siguiente;
                aux->siguiente = NULL;
                cola->tamanio--;
                return 0;
            }
            auxAnterior = aux;
            aux = aux->siguiente;
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

// FUNCIONA PERFECTAMENTE
int quitarEventosProceso(struct colaEventos *colaEventos, int pid)
{
    if (colaEventos->tamanio == 0)
        return 1;
    else if (colaEventos->tamanio == 1)
    {
        if (colaEventos->eventos->evento->proceso->pid == pid)
        {
            colaEventos->eventos = NULL;
            colaEventos->tamanio = 0;
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        while (colaEventos->eventos->evento->proceso->pid == pid)
        {
            if (colaEventos->eventos->siguienteMomento != NULL)
                colaEventos->eventos->siguienteMomento->momento += colaEventos->eventos->momento;
            colaEventos->eventos = colaEventos->eventos->siguienteMomento;
            colaEventos->tamanio--;
            if (colaEventos->eventos == NULL)
                return 0;
        }

        struct momento *momento = colaEventos->eventos->siguienteMomento;
        struct momento *momentoAnterior = colaEventos->eventos;
        while (momento != NULL)
        {
            if (momento->evento->proceso->pid == pid)
            {
                momentoAnterior->siguienteMomento = momento->siguienteMomento;
                if (momentoAnterior->siguienteMomento != NULL)
                    momentoAnterior->siguienteMomento->momento += momento->momento;
                colaEventos->tamanio--;
            }
            else
            {
                momentoAnterior = momento;
            }
            momento = momento->siguienteMomento;
        }
    }
    return 0;
}

// FUNCIONA PERFECTAMENTE
struct momento *siguienteEvento(struct colaEventos *colaEventos)
{
    if (colaEventos->tamanio == 0)
        return NULL;
    struct momento *primero = colaEventos->eventos;
    colaEventos->eventos = colaEventos->eventos->siguienteMomento;
    colaEventos->tamanio--;
    return primero;
}

void marcarSigCambio(struct proceso *p)
{
    bool seguir = 1;
    struct objetoCambio *c = p->cambios->cambios;
    while (seguir && c != NULL)
    {
        if (c->procesado == 0)
        {
            c->procesado = 1;
            seguir = 0;
        }
        else
        {
            c = c->siguiente;
        }
    }
    return;
}

// AQUI DEBE DE ESTAR EL ERROR
float actualizarTiempoParaTerminar(float tEjec, int viejoN, int nuevoN)
{
    return tEjec * viejoN / nuevoN;
}