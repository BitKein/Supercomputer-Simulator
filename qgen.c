
/*

Este programa genera ficheros que contienen colas
de procesos para que sean procesados por el simulador

*/

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define MAX_TEST_LINE 500
#define MAX_NUCLEOS 64
#define MAX_TEJEC 99999
#define MAX_CAMBIOS 4
#define MAX_FACTOR_POW 3
#define MAX_QUEUE_SIZE 100

int main()
{
    printf("Generando testfile...\n");
    int a = 0;
    WIN32_FIND_DATA archivo;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char ruta_completa[MAX_PATH];
    int contador = 0;
    // char *ruta = "C:\\Users\\amq18\\Desktop\\qgen\\tests";
    char *ruta = "C:\\Users\\amq18\\Desktop\\Supercomputer-Simulator\\tests";

    // Construir la ruta para la búsqueda con comodín (*.* para todos los archivos)
    snprintf(ruta_completa, MAX_PATH, "%s\\*.*", ruta);

    // Buscar el primer archivo en el directorio
    hFind = FindFirstFile(ruta_completa, &archivo);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("No se pudo abrir el directorio: %s\n", ruta);
        return -1; // Error al abrir el directorio
    }

    // Recorrer todos los archivos y directorios en el directorio
    do
    {
        // Ignorar directorios (como "." y "..")
        if (!(archivo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            contador++;
        }
    } while (FindNextFile(hFind, &archivo) != 0);

    FindClose(hFind); // Cerrar el handle de búsqueda

    contador++;
    char scontador[10]; // Asegúrate de que haya suficiente espacio para almacenar el número
    sprintf(scontador, "%d", contador);

    // Crear un buffer suficientemente grande para contener el nombre completo del archivo
    char nomfichero[MAX_PATH];
    snprintf(nomfichero, MAX_PATH, "tests/test%s.txt", scontador); // Formatear el nombre del archivo

    FILE *f = fopen(nomfichero, "w"); // Abrir el archivo para escritura
    if (f == NULL)
    {
        printf("No se pudo crear el archivo: %s\n", nomfichero);
        return -1;
    }

    printf("Archivo creado: %s\n", nomfichero);

    // rellenar el fichero

    /*
    estructura de cada linea del fichero:
    pid,nucleos,tEjec,numCambios,*[momentoCambio,incr,factor]
    */

    int sigPid = 0;
    int c, cambios, tejec, nuc, factor, incr, camb_anterior, nuc_actualizado;
    srand(time(0));
    int max_pid = (int)(rand() % MAX_QUEUE_SIZE);
    // char *linea = (char *)malloc(MAX_TEST_LINE * sizeof(char));
    char linea[MAX_TEST_LINE];

    while (sigPid <= max_pid)
    {
        // linea = (char *)malloc(MAX_TEST_LINE * sizeof(char));
        // char linea[MAX_TEST_LINE];
        // sprintf(linea, 0);
        // linea = "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   ";
        for (int k = 0; k < 500; k++)
            linea[k] = 0;
        // añadimos el pid
        sprintf(linea, "%s%d,", linea, sigPid);
        // añadimos numero de nucleos
        nuc = (int)(pow(2.0, (rand() % (int)(log2(MAX_NUCLEOS) + 1))));
        sprintf(linea, "%s%d,", linea, nuc);
        // añadimos tiempo de ejec
        tejec = (int)(rand() % (MAX_TEJEC + 1));
        sprintf(linea, "%s%d,", linea, tejec);
        // añadimos numero de cambios
        cambios = (int)(rand() % (MAX_CAMBIOS + 1));
        if (cambios != 0)
            sprintf(linea, "%s%d,", linea, cambios);
        else
            sprintf(linea, "%s%d", linea, cambios);
        // array para saber los momentos que han salido ya
        // int a[cambios];
        bool seguir = TRUE;
        // int c;

        camb_anterior = 0;

        // actualizar el bucle con lo puesto en el cuaderno

        nuc_actualizado = nuc;
        for (int k = 0; k < cambios; k++)
        {
            /* while (seguir)
            {
                seguir = FALSE;
                c = rand() % tejec;
            }
            a[k] = c; */
            c = rand() % tejec;
            while (c <= camb_anterior)
                c = rand() % tejec;
            camb_anterior = c;
            // añadimos el momento del cambio
            sprintf(linea, "%s%d,", linea, c);
            // añadimos si incrementar o decrementar
            if (nuc_actualizado < 2)
            {

                incr = 1;
                sprintf(linea, "%s%d,", linea, incr);
                factor = (int)(pow(2.0, (rand() % (MAX_FACTOR_POW + 1))));
                // while (nuc_actualizado * factor <= MAX_NUCLEOS)
                while (nuc_actualizado * factor > MAX_NUCLEOS)
                    factor = (int)(pow(2.0, (rand() % (MAX_FACTOR_POW + 1))));
                sprintf(linea, "%s%d", linea, factor);
            }

            else

            {
                seguir = TRUE;
                while (seguir)
                {
                    factor = (int)(pow(2.0, (rand() % (MAX_FACTOR_POW + 1))));
                    while (factor == 1)
                        factor = (int)(pow(2.0, (rand() % (MAX_FACTOR_POW + 1))));
                    incr = (int)(rand() % 2);
                    if ((incr == 1 && nuc_actualizado * factor <= MAX_NUCLEOS) || (incr == 0 && nuc_actualizado / factor >= 1))
                        seguir = FALSE;
                }
                sprintf(linea, "%s%d,", linea, 1);
                sprintf(linea, "%s%d", linea, factor);
            }
            if (k < cambios - 1)
                sprintf(linea, "%s,", linea);
        }
        sprintf(linea, "%s\n", linea);
        fprintf(f, linea);
        printf("Proceso %i generado.\n", sigPid);
        sigPid++;

        // free((void *)linea);
    }
    fflush(f);
    fclose(f); // Cerrar el archivo
    return 0;
}