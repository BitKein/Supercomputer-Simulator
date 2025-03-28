

/* makespan */
int queue_exec_time;
// Puntero a cola de tiempos de ejecucion
int *job_exec_time;

/* waiting time*/
// int queue_wait_time;
//  Puntero a cola de tiempos de espera
int *job_wait_time;

/* total time */
// int queue_total_time;
//  Puntero a cola de tiempos totales
int *job_total_time;

/* utilization */

// En el momento utilization_moment[i] se estaban utilizando utilization_cores[i] nucleos
int *utilization_moment;
int *utilization_cores;
int utilization_size;

int inicEstadisticas();