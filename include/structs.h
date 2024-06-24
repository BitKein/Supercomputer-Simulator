
struct objetoCambio
{
    // 1: el cambio YA se ha hecho
    // 0: No se ha hecho el cambio
    int procesado;
    int momentoCambio;
    // 1: Multiplicar el número de nucleos actual por el factor
    // 0: Dividir el número de nucleos actual por el factor
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
    struct proceso *proceso;
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
    struct evento *evento;
    struct momento *siguienteMomento;
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
    int cantTotalNucleos;
    int cantNucleosLibres;
    struct colaProcesos *procesosEjec;
};