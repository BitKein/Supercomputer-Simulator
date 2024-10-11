#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/structs.h"
#include "include/func.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Uso: %s <nombre_del_archivo>\n", argv[0]);
        return 1;
    }

    // init supercompu
    struct sistema *sistema = (struct sistema *)malloc(sizeof(struct sistema));
    sistema->tiempoTranscurrido = 0;
    sistema->cantTotalNucleos = 32;
    sistema->cantNucleosLibres = sistema->cantTotalNucleos;
    sistema->procesosEjec = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    sistema->procesosEjec->tamanio = 0;
    sistema->procesosEjec->procesos = NULL;

    FILE *archivo;
    const char delimiters[] = ",";
    char linea[100]; // Array para almacenar una línea del archivo
    char *token;     // Puntero para almacenar cada token
    archivo = fopen(argv[1], "r");

    if (archivo == NULL)
    { // Verifica si el archivo se abrió correctamente
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    struct colaProcesos *colaProcesos = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    colaProcesos->tamanio = 0;
    colaProcesos->procesos = NULL;
    struct proceso *proceso;
    int p;
    struct objetoCambio *aux;

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        // generar el objeto proceso
        proceso = (struct proceso *)malloc(sizeof(struct proceso));
        token = strtok(linea, delimiters);
        proceso->pid = atoi(token);
        token = strtok(NULL, delimiters);
        proceso->nucleos = atoi(token);
        token = strtok(NULL, delimiters);
        proceso->tiempoEjec = atoi(token);
        proceso->tiempoDesdeInicioEjec = 0;
        proceso->tiempoParaTerminar = proceso->tiempoEjec;
        proceso->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        token = strtok(NULL, delimiters);
        proceso->cambios->tamanio = atoi(token);
        // proceso->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        // p = 0;
        // aux = proceso->cambios->cambios;
        if (proceso->cambios->tamanio == 0)
        {
            proceso->cambios->cambios = NULL;
        }
        else
        {
            proceso->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
            p = 0;
            aux = proceso->cambios->cambios;
            while (p < proceso->cambios->tamanio)
            {
                token = strtok(NULL, delimiters);
                aux->momentoCambio = atoi(token);
                token = strtok(NULL, delimiters);
                aux->incrementar = atoi(token);
                token = strtok(NULL, delimiters);
                aux->factor = atoi(token);
                aux->procesado = 0;

                if (p + 1 == proceso->cambios->tamanio)
                    aux->siguiente = NULL;
                else
                {
                    aux->siguiente = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
                    aux = aux->siguiente;
                }
                p++;
            }
        }

        proceso->siguiente = NULL;
        /* proceso->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        token = strtok(NULL, ",");
        proceso->cambios->cambios->momentoCambio = *token;
        token = strtok(NULL, ",");
        proceso->cambios->cambios->incrementar = *token;
        token = strtok(NULL, ",");
        proceso->cambios->cambios->factor = *token;
        proceso->cambios->cambios->procesado = 0;
        proceso->cambios->cambios->siguiente = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        */
        anadirAlFinal(colaProcesos, proceso);

        token = strtok(NULL, delimiters);
    }

    // proceso vacio
    struct proceso *p0 = (struct proceso *)malloc(sizeof(struct proceso));
    p0->cambios = NULL;
    p0->nucleos = 0;
    p0->pid = -1;
    p0->siguiente = NULL;
    p0->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
    p0->cambios->tamanio = 0;
    p0->cambios->cambios = NULL;
    // añadir proceso vacio
    anadirAlFinal(sistema->procesosEjec, p0);

    // definir funcion gestion de colas
    void (*gcFunc)(struct colaProcesos *, struct colaEventos *, struct sistema *);
    gcFunc = &fifo;

    // generar la cola de procesos
    /* struct colaProcesos *colaProcesos = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
    colaProcesos->tamanio = 0;
    colaProcesos->procesos = NULL; */
    // proceso de prueba 1
    /*     struct proceso *p1 = (struct proceso *)malloc(sizeof(struct proceso));
        p1->pid = 0;
        p1->nucleos = 32;
        p1->tiempoEjec = 150;
        p1->tiempoDesdeInicioEjec = 0;
        p1->tiempoParaTerminar = p1->tiempoEjec;
        p1->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p1->cambios->tamanio = 2;
        p1->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p1->cambios->cambios->momentoCambio = 50;
        p1->cambios->cambios->incrementar = 0;
        p1->cambios->cambios->factor = 2;
        p1->cambios->cambios->procesado = 0;
        p1->cambios->cambios->siguiente = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p1->cambios->cambios->siguiente->momentoCambio = 100;
        p1->cambios->cambios->siguiente->incrementar = 1;
        p1->cambios->cambios->siguiente->factor = 2;
        p1->cambios->cambios->siguiente->siguiente = NULL;
        p1->cambios->cambios->siguiente->procesado = 0;
        p1->siguiente = NULL;

        // proceso de prueba 2
        struct proceso *p2 = (struct proceso *)malloc(sizeof(struct proceso));
        p2->pid = 1;
        p2->nucleos = 32;
        p2->tiempoEjec = 150;
        p2->tiempoDesdeInicioEjec = 0;
        p2->tiempoParaTerminar = p2->tiempoEjec;
        p2->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p2->cambios->tamanio = 2;
        p2->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p2->cambios->cambios->momentoCambio = 50;
        p2->cambios->cambios->incrementar = 0;
        p2->cambios->cambios->factor = 2;
        p2->cambios->cambios->procesado = 0;
        p2->cambios->cambios->siguiente = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p2->cambios->cambios->siguiente->momentoCambio = 100;
        p2->cambios->cambios->siguiente->incrementar = 1;
        p2->cambios->cambios->siguiente->factor = 2;
        p2->cambios->cambios->siguiente->siguiente = NULL;
        p2->cambios->cambios->siguiente->procesado = 0;
        p2->siguiente = NULL;

        // proceso de prueba 3
        struct proceso *p3 = (struct proceso *)malloc(sizeof(struct proceso));
        p3->pid = 2;
        p3->nucleos = 32;
        p3->tiempoEjec = 150;
        p3->tiempoDesdeInicioEjec = 0;
        p3->tiempoParaTerminar = p3->tiempoEjec;
        p3->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p3->cambios->tamanio = 2;
        p3->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p3->cambios->cambios->momentoCambio = 50;
        p3->cambios->cambios->incrementar = 0;
        p3->cambios->cambios->factor = 2;
        p3->cambios->cambios->procesado = 0;
        p3->cambios->cambios->siguiente = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p3->cambios->cambios->siguiente->momentoCambio = 100;
        p3->cambios->cambios->siguiente->incrementar = 1;
        p3->cambios->cambios->siguiente->factor = 2;
        p3->cambios->cambios->siguiente->siguiente = NULL;
        p3->cambios->cambios->siguiente->procesado = 0;
        p3->siguiente = NULL;

        struct proceso *p4 = (struct proceso *)malloc(sizeof(struct proceso));
        p4->pid = 3;
        p4->nucleos = 32;
        p4->tiempoEjec = 100;
        p4->tiempoDesdeInicioEjec = 0;
        p4->tiempoParaTerminar = p4->tiempoEjec;
        p4->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p4->cambios->tamanio = 1;
        p4->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p4->cambios->cambios->momentoCambio = 50;
        p4->cambios->cambios->incrementar = 0;
        p4->cambios->cambios->factor = 2;
        p4->cambios->cambios->siguiente = NULL;
        p4->cambios->cambios->procesado = 0;
        p4->siguiente = NULL;

        struct proceso *p5 = (struct proceso *)malloc(sizeof(struct proceso));
        p5->pid = 4;
        p5->nucleos = 8;
        p5->tiempoEjec = 100;
        p5->tiempoDesdeInicioEjec = 0;
        p5->tiempoParaTerminar = p5->tiempoEjec;
        p5->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        p5->cambios->tamanio = 1;
        p5->cambios->cambios = (struct objetoCambio *)malloc(sizeof(struct objetoCambio));
        p5->cambios->cambios->momentoCambio = 50;
        p5->cambios->cambios->incrementar = 0;
        p5->cambios->cambios->factor = 2;
        p5->cambios->cambios->siguiente = NULL;
        p5->cambios->cambios->procesado = 0;
        p5->siguiente = NULL;

        // añadir procesos a la cola
        anadirAlFinal(colaProcesos, p1);
        anadirAlFinal(colaProcesos, p2);
        anadirAlFinal(colaProcesos, p3);
        // anadirAlFinal(colaProcesos, p4);
        //  anadirAlFinal(colaProcesos, p5); */

    // generar el array con los eventos
    struct colaEventos *colaEventos = (struct colaEventos *)malloc(sizeof(struct colaEventos));
    colaEventos->tamanio = 1;
    colaEventos->eventos = (struct momento *)malloc(sizeof(struct momento));
    colaEventos->eventos->momento = 0;
    // colaEventos->eventos->siguienteMomento = colaEventos->eventos;
    colaEventos->eventos->siguienteMomento = NULL;
    colaEventos->eventos->tiempoDesdeEventoAnterior = 0;
    colaEventos->eventos->numeroEventos = 1;
    colaEventos->eventos->evento = (struct evento *)malloc(sizeof(struct evento));
    colaEventos->eventos->evento->tipo = 0;
    colaEventos->eventos->evento->proceso = p0;
    colaEventos->eventos->evento->siguiente = NULL;
    // colaEventos->eventos->siguienteMomento = NULL;

    struct momento *momento = siguienteEvento(colaEventos);
    struct evento *evento;
    //  bucle principal, por cada momento de tiempo...
    int i = 0;
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
            struct objetoCambio *c = proceso->cambios->cambios;
            while (c != NULL)
            {
                if (c->procesado == 0)
                {
                    c->momentoCambio -= momento->momento;
                }
                c = c->siguiente;
            }
            proceso = proceso->siguiente;
        }

        // por cada evento...
        while (evento != NULL)
        {
            struct proceso *p = evento->proceso;

            sistema->cantNucleosLibres += p->nucleos;

            switch (evento->tipo)
            {
            case 0:
                quitarProceso(sistema->procesosEjec, p);
                quitarEventosProceso(colaEventos, p->pid);
                break;
            case 1:
                p->nucleos = p->nucleos * evento->factor;
                quitarProceso(sistema->procesosEjec, p);
                anadirAlFinal(colaProcesos, p);
                quitarEventosProceso(colaEventos, p->pid);
                marcarSigCambio(p);
                // p->cambios->cambios->procesado = 1;
                break;
            case 2:
                p->nucleos = p->nucleos / evento->factor;
                quitarProceso(sistema->procesosEjec, p);
                anadirAlFinal(colaProcesos, p);
                quitarEventosProceso(colaEventos, p->pid);
                marcarSigCambio(p);
                // p->cambios->cambios->procesado = 1;
                break;
            default:
                break;
            }

            evento = evento->siguiente;
        }
        // llamar al GC
        gcFunc(colaProcesos, colaEventos, sistema);

        momento = siguienteEvento(colaEventos);
        // momento = momento->siguienteMomento;
        i++;
    }
    printf("La cola se ha procesado en %i segundos.\n", sistema->tiempoTranscurrido);
    fclose(archivo);
    return 0;
}

/* int main()
{
    struct colaEventos *colaEventos = (struct colaEventos *)malloc(sizeof(struct colaEventos));
    colaEventos->eventos = NULL;
    colaEventos->tamanio = 0;
    struct momento *e1 = (struct momento *)malloc(sizeof(struct momento));
    e1->momento = 40;
    e1->evento = (struct evento *)malloc(sizeof(struct evento));
    e1->evento->proceso = (struct proceso *)malloc(sizeof(struct proceso));
    e1->evento->proceso->pid = 1;
    e1->siguienteMomento = NULL;
    actualizarColaEventos(colaEventos, e1);

    struct momento *e2 = (struct momento *)malloc(sizeof(struct momento));
    e2->momento = 60;
    e2->evento = (struct evento *)malloc(sizeof(struct evento));
    e2->evento->proceso = (struct proceso *)malloc(sizeof(struct proceso));
    e2->evento->proceso->pid = 2;
    e2->siguienteMomento = NULL;
    actualizarColaEventos(colaEventos, e2);

    struct momento *e3 = (struct momento *)malloc(sizeof(struct momento));
    e3->momento = 80;
    e3->evento = (struct evento *)malloc(sizeof(struct evento));
    e3->evento->proceso = (struct proceso *)malloc(sizeof(struct proceso));
    e3->evento->proceso->pid = 3;
    e3->siguienteMomento = NULL;
    actualizarColaEventos(colaEventos, e3);

    struct momento *e4 = (struct momento *)malloc(sizeof(struct momento));
    e4->momento = 130;
    e4->evento = (struct evento *)malloc(sizeof(struct evento));
    e4->evento->proceso = (struct proceso *)malloc(sizeof(struct proceso));
    e4->evento->proceso->pid = 1;
    e4->siguienteMomento = NULL;
    actualizarColaEventos(colaEventos, e4);

    struct momento *e5 = (struct momento *)malloc(sizeof(struct momento));
    e5->momento = 140;
    e5->evento = (struct evento *)malloc(sizeof(struct evento));
    e5->evento->proceso = (struct proceso *)malloc(sizeof(struct proceso));
    e5->evento->proceso->pid = 2;
    e5->siguienteMomento = NULL;
    actualizarColaEventos(colaEventos, e5);

    struct momento *aux = colaEventos->eventos;
    while (aux != NULL)
    {
        printf(" -> %i(%i)", aux->momento, aux->evento->proceso->pid);
        aux = aux->siguienteMomento;
    }
    printf("\n");

    int pid = 2;
    printf("Quitando procesos con pid %i.\n", pid);

    quitarEventosProceso(colaEventos, pid);

    aux = colaEventos->eventos;
    while (aux != NULL)
    {
        printf(" -> %i(%i)", aux->momento, aux->evento->proceso->pid);
        aux = aux->siguienteMomento;
    }
    printf("\n");

    /* struct colaProcesos *colaProcesos = (struct colaProcesos *)malloc(sizeof(struct colaProcesos));
colaProcesos->procesos = NULL;
colaProcesos->tamanio = 0;

struct proceso *p = (struct proceso *)malloc(sizeof(struct proceso));
p->pid = 69;
p->siguiente = NULL;
anadirAlFinal(colaProcesos, p);

struct proceso *p2 = (struct proceso *)malloc(sizeof(struct proceso));
p2->pid = 70;
p2->siguiente = NULL;
anadirAlFinal(colaProcesos, p2);

quitarProceso(colaProcesos, p2);

struct proceso *aux = colaProcesos->procesos;
while (aux != NULL)
{
    printf(" -> %i", aux->pid);
    aux = aux->siguiente;
} */
// return 0;
//} */