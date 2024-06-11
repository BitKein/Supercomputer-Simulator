#include <stdio.h>
#include <stdlib.h>
#include "include/structs.h"
#include "include/func.h"

int main()
{

    // init supercompu
    struct sistema *sistema = (struct sistema *)malloc(sizeof(struct sistema));
    sistema->tiempoTranscurrido = 0;
    sistema->cantNucleos = 32;
    sistema->tamCola = 0;
    sistema->procesosEjec = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));

    // definir funcion gestion de colas
    int *(*gcFunc)(struct colaProcesos *, struct sistema *);
    gcFunc = &simple;

    int tamCola = 5;
    // generar la cola de procesos
    struct colaProcesos *colaProcesos = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    colaProcesos->tamanio = tamCola;
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
    añadirAlFinal(colaProcesos, p1);

    // generar el array con los eventos
    struct colaEventos *colaEventos = (struct colaEventos *)malloc(sizeof(struct colaEventos));
    struct momento *momento = colaEventos->eventos;
    struct evento *evento = momento->siguienteEvento;
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
            // sacar proceso de la máquina
            struct proceso *p = sistema->procesosEjec->procesos;

            if (evento->tipo == 1)
            {
                // cambiar los nucleos que necesita
                p->nucleos = p->nucleos * evento->factor;
            }
            else if (evento->tipo == 2)
            {
                // cambiar los nucleos que necesita
                p->nucleos = p->nucleos / evento->factor;
            }
            // meter al principio
            añadirAlFinal(colaProcesos, p);
            evento = evento->siguiente;
        }
        // llamar al GC
        gcFunc(colaProcesos, sistema);

        momento = momento->siguiente;
    }

    return 0;
}