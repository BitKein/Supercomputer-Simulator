
struct objetoCambio
{
    int momentoCambio;
    // 0: Multiplicar el número de nucleos actual por el factor
    // 1: Dividir el número de nucleos actual por el factor
    int incrementar;
    int factor;
    struct objetoCambio *siguiente;
};

struct cambiosNucleos
{
    int tamanio;
    struct objetoCambio *cambios;
};

struct proceso
{
    int pid;
    int nucleos;
    int tiempoEjec;
    int tiempoDesdeInicioEjec;
    int tiempoParaTerminar;
    struct cambiosNucleos *cambios;
    struct proceso *siguiente;
};

struct colaProcesos
{
    int tamanio;
    struct proceso *procesos;
};

struct evento
{
    int pid;
    // 0: no hay cambio, un proceso ha terminado
    // 1: aumenta el número de núcleos
    // 2: disminuye el número de núcleos
    int tipo;
    // si tipo es 0, factor es 1
    int factor;
    struct evento *siguiente;
};

struct momento
{
    int momento;
    int tiempoDesdeEventoAnterior;
    int numeroEventos;
    // struct evento *eventos;
    struct evento *siguienteEvento;
};

struct colaEventos
{
    int tamanio;
    struct momento *eventos;
};

struct sistema
{
    // segundos transcurridos desde el principio
    int tiempoTranscurrido;
    int cantNucleos;
    int tamCola;
    struct colaProcesos *procesosEjec;
};