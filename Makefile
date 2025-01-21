#Antes de entregar quita la opcion de -O0 que es para quitar
# la optimizacion y que el debugger funcione bien

simulator: simulator.c func.o stats.o
	gcc -g -Wall -O0 -o simulator simulator.c func.o stats.o

func.o: ./src/func.c ./include/func.h ./include/globals.h
	gcc -g -Wall -O0 -c -o func.o ./src/func.c

stats.o: ./src/stats.c ./include/stats.h ./include/globals.h
	gcc -g -Wall -O0 -c -o stats.o ./src/stats.c

.PHONY: clean
clean:
	rm -rf *.o