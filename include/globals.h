// En bf si el siguiente proceso en la lista para entrar en ejecución no entra, se prueban los K siguientes.
#define K 1
// Para permitir o no que los trabajos cambien de numero de cores
#define MALLEABILITY_ON 1

/* #define CORES_SIMULADOR 32 */
// #define CORES_SIMULADOR 165884
#define CORES_SIMULADOR 8

// Para seleccionar el gestor de la cola de trabajos a utilizar
// 1: fifo
// 2: bf
#define GESTOR_COLA 1

// Numero de procesos en la cola de trabajos
int numProcesosCola;

// lista de tamaño numProcesosCola donde procTerminados[i]=1 si el proceso con PID i ha terminado
int *procTerminados;