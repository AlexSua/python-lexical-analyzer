

#ifndef SISTEMADEENTRADA_H
#define	SISTEMADEENTRADA_H

#define N 120 //tamaño de buffer
char buffer1[N + 1];
char buffer2[N + 1];


//Devuelve un caracter del buffer
char consumir_caracter();

//Inicializa el sistema de entrada
void inicializar();

//Abre el fichero que se introduzca como argumento.
void abrir();

//Cierra el fichero
void cerrar();

//Iguala puntero inicio al puntero delantero.
void empiezaLexema();

//Iguala puntero delantero a puntero inicio.
void retrocesoDelantero();




//indica el buffer en el que esta el puntero delantero
int bufferActivo;

//indica el buffer en el que esta el puntero inicio
int bufferActivoInicio;

//indica que un retroceso ha producido un cambio al buffer anterior por parte del puntero delantero.
int retrocesoCambioBuffer;

//archivo sobre el que se realizará la compilación.
FILE *archivo;

//puntero que apunta al principio de cada lexema
char *punteroInicio;

//puntero que apunta al nombre del archivo
char *nombreArchivo;

//puntero que avanza a medida que se leen caracteres del buffer. En caso de fallo puede volver al puntero inicio.
char *punteroDelantero;
#endif	/* SISTEMADEENTRADA_H */
