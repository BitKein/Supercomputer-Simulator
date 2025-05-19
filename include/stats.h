

/* makespan */
float queue_exec_time;
// Puntero a cola de tiempos de ejecucion
float *job_exec_time;

/* waiting time*/
// int queue_wait_time;
//  Puntero a cola de tiempos de espera
float *job_wait_time;

/* total time */
// int queue_total_time;
//  Puntero a cola de tiempos totales
float *job_total_time;

/* utilization */

// En el momento utilization_moment[i] se estaban utilizando utilization_cores[i] nucleos
float *utilization_moment;
int *utilization_cores;
int utilization_size;

int inicEstadisticas();