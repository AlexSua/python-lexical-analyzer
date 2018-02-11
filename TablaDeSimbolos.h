
#ifndef TABLADESIMBOLOS_H
#define	TABLADESIMBOLOS_H
#include "AnalizadorLexico.h"


//Tamanho de la tabla hash
#define NHASH 50

//Cada uno de los nodos de la tabla HASH. Cada nodo contendrá una lista enlazada para el caso en el que haya colision.

struct nodoHash {
    componenteLexico datos; //estructura del componente lexico.
    struct nodoHash* siguienteNodo; //Puntero que apunta al siguiente elemento de la lista enlazada.
};

//Reserva memoria para cada nodo de la tabla HASH.
void crearTablaHash();

//Añade un nodo a la tabla HASH. Cada nodo contendrá un componente Lexico. Este será recibido por argumento.
void anhadirNodoHash(componenteLexico value);

//Obtiene un componente lexco de la tabla hash a traves del lexema. El componente lexico se almacenará en la direccion recibida como argumento.
int obtenerNodoHash(char *key, componenteLexico *clIn);

//Libera la tabla hash al completo junto con todas las estructuras dinamicas que contiene cada nodo.
void LiberarHash();

//Carga la tabla hash con las palabras reservadas del lenguaje a compilar. Python en este caso.
void precargarTablaHash();

//Imprime cada componente lexico almacenado en la tabla HASH. Su lexema,identificador y lineas en las que aparece.
void imprimirTablaHash();

/*En caso de existir el componente lexico introducido por argumento, se añade una nueva linea a la lista enlazada de lineas de ese componente lexico
 * y la funcion devuelve 1. En caso de no existir, la funcion devuelve 0.
 **/
int anhadirLinea(componenteLexico value);

#endif	/* TABLADESIMBOLOS_H */

