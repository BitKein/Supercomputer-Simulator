void fifo(struct colaProcesos *colaProcesos, struct colaEventos *colaEventos, struct sistema *sistema);
void actualizarColaEventos(struct colaEventos *colaEventos, struct momento *e);
void anadirAlFinal(struct colaProcesos *cola, struct proceso *p);
void anadirAlPrincipio(struct colaProcesos *cola, struct proceso *p);
int quitarProceso(struct colaProcesos *cola, struct proceso *p);
