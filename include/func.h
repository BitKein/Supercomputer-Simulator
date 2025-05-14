void fifo(struct colaProcesos *colaProcesos, struct colaEventos *colaEventos, struct sistema *sistema);
void bf(struct colaProcesos *colaProcesos, struct colaEventos *colaEventos, struct sistema *sistema, int k);
void actualizarColaEventos(struct colaEventos *colaEventos, struct momento *e);
void anadirAlFinal(struct colaProcesos *cola, struct proceso *p);
void anadirAlPrincipio(struct colaProcesos *cola, struct proceso *p);
int quitarProceso(struct colaProcesos *cola, struct proceso *p);
struct momento *siguienteEvento(struct colaEventos *colaEventos);
