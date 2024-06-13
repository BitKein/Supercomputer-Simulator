#include <stdio.h>
#include <stdlib.h>
#include "include/structs.h"
#include "include/func.h"

int main()
{

    // init supercompu
    struct sistema *sistema = (struct sistema *)malloc(sizeof(struct sistema));
    sistema->tiempoTranscurrido = 0;
    sistema->cantTotalNucleos = 32;
    sistema->cantNucleosLibres = sistema->cantTotalNucleos;
    sistema->procesosEjec = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    sistema->procesosEjec->tamanio = 0;

    // proceso vacio
    struct proceso *p0 = (struct proceso *)malloc(sizeof(struct proceso));
    // añadir proceso vacio
    anadirAlFinal(sistema->procesosEjec, p0);

    // definir funcion gestion de colas
    void (*gcFunc)(struct colaProcesos *, struct colaEventos *, struct sistema *);
    gcFunc = &fifo;

    // generar la cola de procesos
    struct colaProcesos *colaProcesos = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    colaProcesos->tamanio = 0;
    struct proceso *p1 = (struct proceso *)malloc(sizeof(struct proceso));
    p1->pid = 0;
    p1->nucleos = 4;
    p1->tiempoEjec = 100;
    p1->tiempoDesdeInicioEjec = 0;
    p1->tiempoParaTerminar = p1->tiempoEjec;
    p1->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
    p1->cambios->tamanio = 1;
    p1->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
    p1->cambios->cambios->momentoCambio = 50;
    p1->cambios->cambios->incrementar = 1;
    p1->cambios->cambios->factor = 2;
    p1->siguiente = NULL;

    // añadir procesos a la cola
    anadirAlFinal(colaProcesos, p1);

    // generar el array con los eventos
    struct colaEventos *colaEventos = (struct colaEventos *)malloc(sizeof(struct colaEventos));
    colaEventos->tamanio = 1;
    colaEventos->eventos = (struct momento *)malloc(sizeof(struct momento));
    colaEventos->eventos->momento = 0;
    colaEventos->eventos->tiempoDesdeEventoAnterior = 0;
    colaEventos->eventos->numeroEventos = 1;
    colaEventos->eventos->evento = (struct evento *)malloc(sizeof(struct evento));
    colaEventos->eventos->evento->tipo = 0;
    colaEventos->eventos->evento->proceso = (struct proceso *)malloc(sizeof(struct proceso));

    struct momento *momento = colaEventos->eventos;
    struct evento *dfdg = colaEventos->eventos->evento;
    struct evento *evento = momento->evento;
    // bucle principal, por cada momento de tiempo...
    while (momento != NULL)
    {
        // actualizar tiempo que ha transcurrido
        sistema->tiempoTranscurrido += momento->momento;
        // actualizar tiempos cola procesos
        struct proceso *proceso = sistema->procesosEjec->procesos;
        while (proceso != NULL)
        {
            proceso->tiempoDesdeInicioEjec += momento->momento;
            proceso->tiempoParaTerminar -= momento->momento;
            proceso = proceso->siguiente;
        }

        // por cada evento...
        while (evento != NULL)
        {
            struct proceso *p = evento->proceso;
            if (evento->tipo != 0)
            {
                if (evento->tipo == 1)
                {
                    // cambiar los nucleos que necesita
                    p->nucleos = p->nucleos * evento->factor;
                }
                if (evento->tipo == 2)
                {
                    // cambiar los nucleos que necesita
                    p->nucleos = p->nucleos / evento->factor;
                }
                anadirAlFinal(colaProcesos, p);
            }

            quitarProceso(sistema->procesosEjec, p);

            evento = evento->siguiente;
        }
        // llamar al GC
        gcFunc(colaProcesos, colaEventos, sistema);

        momento = momento->siguienteMomento;
    }

    return 0;
}