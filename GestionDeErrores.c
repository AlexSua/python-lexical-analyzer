
#include <stdio.h>
#include <stdlib.h>
#include "GestionDeErrores.h"
#include "TablaDeSimbolos.h"
#include "AnalizadorLexico.h"
#include "SistemaDeEntrada.h"

void gestionarError(int num_err, int col, int linea) {//Toma como argumento el numero de error, columna y linea en el que se produce.
    printf("\nError: %d. ", num_err);
    if (num_err == ERROR_LEXICO) {
        printf("Error lexico en la columna %d de la fila %d\n", col, linea);
        if (compLex.lexema != 0)
            free(compLex.lexema);
        LiberarHash();
    } else if (num_err == ERROR_ABRIR_ARCHIVO) {
        printf("Error abriendo el archivo\n");
        if (compLex.lexema != 0)
            free(compLex.lexema);
        LiberarHash();
    } else if (num_err == ERROR_CERRAR_ARCHIVO) {
        printf("Error cerrando el archivo\n");
    } else if (num_err == ERROR_LECTURA) {
        printf("Error de lectura en el archivo\n");
        if (compLex.lexema != 0)
            free(compLex.lexema);
        LiberarHash();
    } else if (num_err == ERROR_COMILLAS) {
        printf("Error en el uso de las comillas en la columna %d de la fila %d\n", col, linea);
        if (compLex.lexema != 0)
            free(compLex.lexema);
        LiberarHash();
    } else if (num_err == ERROR_BUFFEROVERFLOW) {
        printf("BufferOverflow. Tamano del lexema demasiado grande en la columna %d de la fila %d\n", col, linea);
        if (compLex.lexema != 0)
            free(compLex.lexema);
        LiberarHash();
    } else if (num_err == ERROR_ARGUMENTO) {
        printf("No se ha introducido un argumento con el nombre del archivo\n");
    }

    exit(0);
}
/*
 * 
 */

