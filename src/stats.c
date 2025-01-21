
#include "../include/stats.h"
#include "../include/globals.h"
#include "../include/structs.h"
#include <stddef.h>

// para inicializar estructuras de datos relacionadas con las estadísticas
int inicEstadisticas()
{
    // makespan
    queue_exec_time = (int *)malloc(sizeof(int));
    queue_exec_time = 0;
    job_exec_time = (int **)malloc(numProcesosCola * sizeof(int *));
    for (int i = 0; i < numProcesosCola; i++)
    {
        job_exec_time[i] = (int *)malloc(sizeof(int));
        job_exec_time[i] = 0;
    }

    // waiting time en cola
    queue_wait_time = (int *)malloc(sizeof(int));
    *queue_wait_time = 0;

    // waiting time de cada job
    job_wait_time = (int **)malloc(numProcesosCola * sizeof(int *));
    for (int i = 0; i < numProcesosCola; i++)
    {
        job_wait_time[i] = (int *)malloc(sizeof(int));
        job_wait_time[i] = 0;
    }

    // total time

    job_total_time = (int **)malloc(numProcesosCola * sizeof(int *));
    for (int i = 0; i < numProcesosCola; i++)
    {
        job_total_time[i] = (int *)malloc(sizeof(int));
        job_total_time[i] = 0;
    }

    // utilization
    utilization_size = (int *)malloc(sizeof(int));
    *utilization_size = 0;

    return 0;
}

// para actualizar o recopilar las estadísticas
int actEstadisticas(struct colaProcesos *cola, int t)
{

    if (cola->tamanio == 0)
        return 1;

    int *pidProcesosEjec = (int *)malloc(cola->tamanio * sizeof(int));
    int j = 0;
    struct proceso *pr = cola->procesos;
    while (pr != NULL)
    {
        pidProcesosEjec[j] = pr->pid;
        j++;
        pr = pr->siguiente;
    }
    if (j != cola->tamanio)
        return 2;

    j = 0;
    for (int i = 0; i < numProcesosCola; i++)
    {
        if (!procTerminados[i])
        {
            job_total_time[i] += t;
        }

        if (i == pidProcesosEjec[j])
        {
            job_exec_time[i] += t;
            j++;
        }

        else
        {
            if (!procTerminados[i])
            {
                job_wait_time[i] += t;
            }
        }
    }
}

int utilization(int nucleos_en_uso)
{

    return 0;
}

/* int runStats()
{
    // waiting time
} */

// makespan: tiempo de ejec basicamente, tanto de la cola como de cada trabajo
// devuelve el Tejec de la cola + una lista de Tejec de cada proceso
/* int makespan()
{
    // en principio solo devolverá el Tejec de la cola
    queue_exec_time += ;
} */

// waiting time: tiempo de espera, tanto de la cola como de cada proceso
// devuelve el Wtime de la cola + una lista de Wtime de cada proceso
int *waitingTime()
{
}

// total time: Tejec + Wtime, tanto de la cola como de cada proceso
//  devuelve el Ttime de la cola + una lista de Ttime de cada proceso
int *totalTime()
{
}

int utilization()
{
}