#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/structs.h"
#include "include/func.h"
#include "include/stats.h"
#include "include/globals.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Uso: %s <nombre_del_archivo>\n", argv[0]);
        return 1;
    }

    // init supercompu
    struct sistema *sistema = (struct sistema *)malloc(sizeof(struct sistema));
    sistema->tiempoTranscurrido = 0.0;
    sistema->cantTotalNucleos = CORES_SIMULADOR;
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
        proceso->tiempoEjec = atof(token);
        proceso->tiempoDesdeInicioEjec = 0.0;
        proceso->tiempoParaTerminar = proceso->tiempoEjec;
        proceso->cambios = (struct cambiosNucleos *)malloc(sizeof(struct cambiosNucleos));
        token = strtok(NULL, delimiters);
        proceso->cambios->tamanio = atoi(token);
        if (!MALLEABILITY_ON)
        {
            proceso->cambios->tamanio = 0;
            proceso->cambios->cambios = NULL;
        }
        else if (proceso->cambios->tamanio == 0)
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
                aux->momentoCambio = atof(token);
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
        anadirAlFinal(colaProcesos, proceso);

        token = strtok(NULL, delimiters);
    }

    numProcesosCola = colaProcesos->tamanio;
    procTerminados = (int *)malloc(numProcesosCola * sizeof(int));
    for (int i = 0; i < numProcesosCola; i++)
        procTerminados[i] = 0;

    // inicializar memoria estadisticas
    inicEstadisticas();

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

    switch (GESTOR_COLA)
    {
    case 1:
        gcFunc = &fifo;
        break;
    case 2:
        gcFunc = &bf;
        break;
    }

    // generar el array con los eventos
    struct colaEventos *colaEventos = (struct colaEventos *)malloc(sizeof(struct colaEventos));
    colaEventos->tamanio = 1;
    colaEventos->eventos = (struct momento *)malloc(sizeof(struct momento));
    colaEventos->eventos->momento = 0.0;
    // colaEventos->eventos->siguienteMomento = colaEventos->eventos;
    colaEventos->eventos->siguienteMomento = NULL;
    colaEventos->eventos->tiempoDesdeEventoAnterior = 0.0;
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

        // actualizar stats
        // actEstadisticas(sistema->procesosEjec, momento->momento);

        // inicio actualizar estadisticas
        /* if (sistema->procesosEjec->tamanio == 0)
            return 1; */

        int *pidProcesosEjec = (int *)malloc(sistema->procesosEjec->tamanio * sizeof(int));
        int j = 0;
        struct proceso *pr = sistema->procesosEjec->procesos;
        while (pr != NULL)
        {
            pidProcesosEjec[j] = pr->pid;
            j++;
            pr = pr->siguiente;
        }
        /* if (j != sistema->procesosEjec->tamanio)
            return 2; */

        j = 0;
        for (int f = 0; f < numProcesosCola; f++)
        // for (int i = 0; i < cola->tamanio; i++)
        {
            if (!procTerminados[f])
            {
                job_total_time[f] += momento->momento;
            }

            // if (i == pidProcesosEjec[j])

            bool enEjec = false;
            for (int k = 0; k < sistema->procesosEjec->tamanio; k++)
            {
                if (f == pidProcesosEjec[k])
                {
                    job_exec_time[f] += momento->momento;
                    enEjec = true;
                    break;
                }
            }

            /* if (i == pidProcesosEjec[j])
            {
                job_exec_time[i] += t;
                j++;
            } */

            if (!enEjec && !procTerminados[f])
            {
                job_wait_time[f] += momento->momento;
            }
        }
        // final actualizar estadisticas

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
                procTerminados[p->pid] = 1;
                break;
            case 1:
                // p->tiempoParaTerminar = actualizarTiempoParaTerminar(p->tiempoParaTerminar, p->nucleos, p->nucleos * evento->factor);
                p->tiempoParaTerminar = p->tiempoParaTerminar * p->nucleos / (p->nucleos * evento->factor);
                p->nucleos = p->nucleos * evento->factor;
                quitarProceso(sistema->procesosEjec, p);
                switch (GESTOR_COLA)
                {
                case 1:
                    // fifo
                    anadirAlFinal(colaProcesos, p);
                    break;
                case 2:
                    // bf
                    anadirAlPrincipio(colaProcesos, p);
                    break;
                }
                // anadirAlPrincipio(colaProcesos, p);
                //  anadirAlFinal(colaProcesos, p);
                quitarEventosProceso(colaEventos, p->pid);
                marcarSigCambio(p);
                // p->cambios->cambios->procesado = 1;
                break;
            case 2:
                // p->tiempoParaTerminar = actualizarTiempoParaTerminar(p->tiempoParaTerminar, p->nucleos, (p->nucleos / evento->factor));
                p->tiempoParaTerminar = p->tiempoParaTerminar * p->nucleos / (p->nucleos / evento->factor);
                p->nucleos = p->nucleos / evento->factor;
                quitarProceso(sistema->procesosEjec, p);
                switch (GESTOR_COLA)
                {
                case 1:
                    // fifo
                    anadirAlFinal(colaProcesos, p);
                    break;
                case 2:
                    // bf
                    anadirAlPrincipio(colaProcesos, p);
                    break;
                }
                // anadirAlPrincipio(colaProcesos, p);
                //  anadirAlFinal(colaProcesos, p);
                quitarEventosProceso(colaEventos, p->pid);
                marcarSigCambio(p);
                // p->cambios->cambios->procesado = 1;
                break;
            default:
                break;
            }

            evento = evento->siguiente;
        }

        // utilization(momento->momento, sistema->cantTotalNucleos - sistema->cantNucleosLibres);

        // llamar al GC
        gcFunc(colaProcesos, colaEventos, sistema);

        // utilization(sistema->tiempoTranscurrido, sistema->cantTotalNucleos - sistema->cantNucleosLibres);
        // inicio utilizatio
        utilization_size++;
        utilization_moment = (int *)realloc(utilization_moment, utilization_size * sizeof(int));
        utilization_moment[utilization_size - 1] = sistema->tiempoTranscurrido;
        utilization_cores = (int *)realloc(utilization_cores, utilization_size * sizeof(int));
        utilization_cores[utilization_size - 1] = sistema->cantTotalNucleos - sistema->cantNucleosLibres;

        // final utilization

        momento = siguienteEvento(colaEventos);
        // momento = momento->siguienteMomento;
        i++;
    }
    /* int dias = sistema->tiempoTranscurrido / (60 * 60 * 24);
    int horas = sistema->tiempoTranscurrido / (60 * 60) - dias * 24;
    int minutos = sistema->tiempoTranscurrido / 60 - dias * 60 * 24 - horas * 60;
    int segundos = sistema->tiempoTranscurrido - dias * 60 * 60 * 24 - horas * 60 * 60 - minutos * 60;
    printf("La cola se ha procesado en %i dias, %i horas, %i minutos y %i segundos. (%i seg en total)\n", dias, horas, minutos, segundos, sistema->tiempoTranscurrido); */
    printf(" -- EXECUTION STATS -- \n");
    printf("Queue makespan: %f\n", sistema->tiempoTranscurrido);
    // printf("Queue waiting time: %d\n", queue_wait_time);
    printf("Job waiting time (ordered by PID): ");
    for (int i = 0; i < numProcesosCola; i++)
    {
        printf("%f ", job_wait_time[i]);
    }
    printf("\n");
    // printf("Queue total time: %d\n", queue_total_time);
    printf("Job total time (ordered by PID): ");
    for (int i = 0; i < numProcesosCola; i++)
    {
        printf("%f ", job_total_time[i]);
    }
    printf("\n");

    printf("Utilization moments: ");
    for (int i = 0; i < utilization_size; i++)
        printf("%f,", utilization_moment[i]);

    printf("\n");

    printf("Utilization cores: ");
    for (int i = 0; i < utilization_size; i++)
        printf("%i,", utilization_cores[i]);

    printf("\n");
    queue_exec_time = sistema->tiempoTranscurrido;
    // utilization

    fclose(archivo);
    return 0;
}
