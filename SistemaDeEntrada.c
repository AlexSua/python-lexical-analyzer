#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SistemaDeEntrada.h"
#include "GestionDeErrores.h"

void CargaBuffer1() {
    memset(buffer1, '\0', N); //Rellena el buffer con \0
    fread(buffer1, sizeof (char), N, archivo); //Lee N caracteres del archivo abierto.
    buffer1[N] = EOF; //Iguala la ultima posicion del buffer a EOF.
    bufferActivo = 1; //Modifica la variable bufferActivo para indicar que el puntero delantero se encuentrara ahora en este buffer recien cargado.
    if (feof(archivo)) {
        cerrar(archivo); //En caso de encontrar fin de linea cerrar archivo
    }

}

void CargaBuffer2() {//Lo mismo que en el caso anterior
    memset(buffer2, '\0', N);
    fread(buffer2, sizeof (char), N, archivo);
    buffer2[N] = EOF;
    bufferActivo = 2;
    if (feof(archivo)) {
        cerrar(archivo);
    }
}

//Inicializa el sistema de entrada.

void inicializar() {
    abrir(); //Abre el fichero
    CargaBuffer1(); //Carga el primer buffer
    bufferActivoInicio = 1; //Indica que el buffer en el que se encuentra el puntero de inicio es 1
    punteroInicio = buffer1; //El puntero inicio apunta al primer elemento del buffer 1
    punteroDelantero = buffer1; //Puntero delantero apunta al primer elemento del buffer 1
    retrocesoCambioBuffer = 0; //Inicializa la variable retroceso
}

void retrocesoDelantero() {//Iguala puntero delantero a puntero inicio
    punteroDelantero = punteroInicio;
    if (bufferActivo != bufferActivoInicio) {//En caso de que el puntero delantero y el inicio se encuentre en distintos buffers....
        bufferActivo = bufferActivoInicio;
        retrocesoCambioBuffer = 1; //...se modifica esta variable, con el objetivo de no volver a cargar el buffer ya cargado.
    }
}

char consumir_caracter() {//Devuelve un caracter del buffer.
    char aux;
    if (archivo == NULL) {
        inicializar();
    }
    aux = *punteroDelantero; //aux: Variable que devolvera el caracter. Se usa porque antes de devolver un caracter, el punteroDelantero va a avanzar una posicion.
    if (aux == EOF) {//En caso de encontrar un EOF significa que es fin de buffer. Por lo que se produce la carga de...
        if (bufferActivo == 1) {//...buffer2 si se encuentra activo buffer1 
            if (retrocesoCambioBuffer != 1) {
                CargaBuffer2();
            } else retrocesoCambioBuffer = 0; //En caso de encontrar retrocesoCambioBuffer no se vuelve a cargar el buffer y se reinicia la variable.
            punteroDelantero = buffer2; //El puntero delantero se iguala a la primera posicion del buffer recien cargado.
            aux = *punteroDelantero;
        } else {//...buffer1 si se encuentra activo buffer2
            if (retrocesoCambioBuffer != 1) {
                CargaBuffer1();
            } else retrocesoCambioBuffer = 0;
            punteroDelantero = buffer1;
            aux = *punteroDelantero;
        }
    }
    if (aux == 0) {//En caso de encontrar 0, significa que hay fin de fichero por lo que se devuelve un EOF
        punteroDelantero = punteroDelantero + 1;
        return EOF;
    }
    punteroDelantero = punteroDelantero + 1;
    return aux; //En caso contrario se devuelve el caracter

}

void empiezaLexema() {//Se iguala puntero inicio al puntero delantero-1
    punteroInicio = punteroDelantero - 1;
    bufferActivoInicio = bufferActivo;
}

void abrir() {//abre el archivo
    archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        gestionarError(ERROR_ABRIR_ARCHIVO, 0, 0);
    }
}

void cerrar() {//cierra el archivo
    fclose(archivo);
}

