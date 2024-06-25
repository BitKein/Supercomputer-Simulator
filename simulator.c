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
    sistema->procesosEjec->procesos = NULL;

    // proceso vacio
    struct proceso *p0 = (struct proceso *)malloc(sizeof(struct proceso));
    p0->cambios = NULL;
    p0->nucleos = 0;
    p0->pid = -1;
    p0->siguiente = NULL;
    // añadir proceso vacio
    anadirAlFinal(sistema->procesosEjec, p0);

    // definir funcion gestion de colas
    void (*gcFunc)(struct colaProcesos *, struct colaEventos *, struct sistema *);
    gcFunc = &fifo;

    // generar la cola de procesos
    struct colaProcesos *colaProcesos = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    colaProcesos->tamanio = 0;
    // proceso de prueba 1
    struct proceso *p1 = (struct proceso *)malloc(sizeof(struct proceso));
    p1->pid = 0;
    p1->nucleos = 8;
    p1->tiempoEjec = 100;
    p1->tiempoDesdeInicioEjec = 0;
    p1->tiempoParaTerminar = p1->tiempoEjec;
    p1->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
    p1->cambios->tamanio = 1;
    p1->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
    p1->cambios->cambios->momentoCambio = 50;
    p1->cambios->cambios->incrementar = 1;
    p1->cambios->cambios->factor = 2;
    p1->cambios->cambios->siguiente = NULL;
    p1->cambios->cambios->procesado = 0;
    p1->siguiente = NULL;

    // proceso de prueba 2
    struct proceso *p2 = (struct proceso *)malloc(sizeof(struct proceso));
    p2->pid = 1;
    p2->nucleos = 8;
    p2->tiempoEjec = 100;
    p2->tiempoDesdeInicioEjec = 0;
    p2->tiempoParaTerminar = p2->tiempoEjec;
    p2->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
    p2->cambios->tamanio = 1;
    p2->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
    p2->cambios->cambios->momentoCambio = 50;
    p2->cambios->cambios->incrementar = 1;
    p2->cambios->cambios->factor = 2;
    p2->cambios->cambios->siguiente = NULL;
    p2->cambios->cambios->procesado = 0;
    p2->siguiente = NULL;

    // proceso de prueba 3
    struct proceso *p3 = (struct proceso *)malloc(sizeof(struct proceso));
    p3->pid = 2;
    p3->nucleos = 8;
    p3->tiempoEjec = 100;
    p3->tiempoDesdeInicioEjec = 0;
    p3->tiempoParaTerminar = p3->tiempoEjec;
    p3->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
    p3->cambios->tamanio = 1;
    p3->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
    p3->cambios->cambios->momentoCambio = 50;
    p3->cambios->cambios->incrementar = 1;
    p3->cambios->cambios->factor = 2;
    p3->cambios->cambios->siguiente = NULL;
    p3->cambios->cambios->procesado = 0;
    p3->siguiente = NULL;

    /*     // proceso de prueba 3
        struct proceso *p4 = (struct proceso *)malloc(sizeof(struct proceso));
        p4->pid = 3;
        p4->nucleos = 4;
        p4->tiempoEjec = 100;
        p4->tiempoDesdeInicioEjec = 0;
        p4->tiempoParaTerminar = p4->tiempoEjec;
        p4->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p4->cambios->tamanio = 1;
        p4->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p4->cambios->cambios->momentoCambio = 50;
        p4->cambios->cambios->incrementar = 1;
        p4->cambios->cambios->factor = 2;
        p4->siguiente = NULL;

        // proceso de prueba 3
        struct proceso *p5 = (struct proceso *)malloc(sizeof(struct proceso));
        p5->pid = 3;
        p5->nucleos = 4;
        p5->tiempoEjec = 100;
        p5->tiempoDesdeInicioEjec = 0;
        p5->tiempoParaTerminar = p5->tiempoEjec;
        p5->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p5->cambios->tamanio = 1;
        p5->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p5->cambios->cambios->momentoCambio = 50;
        p5->cambios->cambios->incrementar = 1;
        p5->cambios->cambios->factor = 2;
        p5->cambios->cambios->siguiente=NULL;
        p5->siguiente = NULL; */

    // añadir procesos a la cola
    anadirAlFinal(colaProcesos, p1);
    anadirAlFinal(colaProcesos, p2);
    anadirAlFinal(colaProcesos, p3);
    // anadirAlFinal(colaProcesos, p4);
    // anadirAlFinal(colaProcesos, p5);

    // generar el array con los eventos
    struct colaEventos *colaEventos = (struct colaEventos *)malloc(sizeof(struct colaEventos));
    colaEventos->tamanio = 1;
    colaEventos->eventos = (struct momento *)malloc(sizeof(struct momento));
    colaEventos->eventos->momento = 0;
    colaEventos->eventos->siguienteMomento = colaEventos->eventos;
    colaEventos->eventos->tiempoDesdeEventoAnterior = 0;
    colaEventos->eventos->numeroEventos = 1;
    colaEventos->eventos->evento = (struct evento *)malloc(sizeof(struct evento));
    colaEventos->eventos->evento->tipo = 0;
    colaEventos->eventos->evento->proceso = p0;
    colaEventos->eventos->evento->siguiente = NULL;

    struct momento *momento = colaEventos->eventos;
    struct evento *evento;
    //  bucle principal, por cada momento de tiempo...
    while (momento != NULL)
    {
        evento = momento->evento;
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

            /* if (evento->tipo != 0)
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

            quitarProceso(sistema->procesosEjec, p); */

            sistema->cantNucleosLibres += p->nucleos;

            switch (evento->tipo)
            {
            case 0:
                quitarProceso(sistema->procesosEjec, p);
                if (p->pid != -1)
                    quitarEventosProceso(colaEventos, p->pid);
                break;
            case 1:
                p->nucleos = p->nucleos * evento->factor;
                quitarProceso(sistema->procesosEjec, p);
                anadirAlFinal(colaProcesos, p);
                quitarEventosProceso(colaEventos, p->pid);
                p->cambios->cambios->procesado = 1;
                break;
            case 2:
                p->nucleos = p->nucleos / evento->factor;
                quitarProceso(sistema->procesosEjec, p);
                anadirAlFinal(colaProcesos, p);
                quitarEventosProceso(colaEventos, p->pid);
                p->cambios->cambios->procesado = 1;
                break;
            default:
                break;
            }

            /* if (evento->proceso->pid != -1)
                quitarEventosProceso(colaEventos, p->pid); */
            evento = evento->siguiente;
        }
        // llamar al GC
        gcFunc(colaProcesos, colaEventos, sistema);

        momento = momento->siguienteMomento;
    }
    printf("La cola se ha procesado en %i segundos.\n", sistema->tiempoTranscurrido);
    return 0;
}