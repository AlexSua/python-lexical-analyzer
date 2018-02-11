#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Definiciones.h"
#include "TablaDeSimbolos.h"

//Array de nodos que forman la tabla hash
struct nodoHash* HashNode;

//Calcula la clave para acceder a cada nodo HASH a traves del lexema del componente lexico.
inline static int funcionHash(char *key) {
    int tam, clave, i;
    tam = strlen(key);
    clave = 0;
    for (i = tam - 1; i >= 0; i = i - 1)
        clave = (clave * 256 + (int) key[i]) % NHASH;
    return clave;
}

//Se hace la reserva de memoria para cada nodo de la tabla HASH y se precarga con las palabras reservadas.
void crearTablaHash() {
    int i;
    HashNode = (struct nodoHash*) malloc(NHASH * sizeof (struct nodoHash));
    for (i = 0; i < NHASH; i++) {
        HashNode[i].siguienteNodo = NULL;
        HashNode[i].datos.lexema = NULL;
    }
    precargarTablaHash();
}

void anhadirNodoHash(componenteLexico value) {
    int clave;
    int tam = strlen(value.lexema) + 1;
    clave = funcionHash(value.lexema);//se aplica la función hash para localizar el sitio donde se va a almacenar el elemento dentro de la tabla hash
    struct nodoHash *nodo;
    if (HashNode[clave].datos.lexema == NULL) {//En caso de que sea NULL se añade normalmente en la tabla HASH.
        HashNode[clave].datos.lexema = (char*) malloc(sizeof (char)*tam);
        strcpy(HashNode[clave].datos.lexema, value.lexema);
        HashNode[clave].datos.identificador = value.identificador;
        HashNode[clave].datos.arrayLineas.linea = value.arrayLineas.linea;
    } else {//En caso de colision se usa una lista enlazada, el primer nodo es reemplazado por el nodo que contiene el componente lexico acabado de leer. Y este ultimo apuntará como siguiente elemento de la lista al nodo que antes estaba de primero.
        nodo = (struct nodoHash*) malloc(sizeof (struct nodoHash));
        nodo->siguienteNodo = (struct nodoHash*) malloc(sizeof (struct nodoHash));
        memcpy(nodo->siguienteNodo, &HashNode[clave], sizeof (struct nodoHash));
        nodo->datos.lexema = (char*) malloc(sizeof (char)*tam);
        strcpy(nodo->datos.lexema, value.lexema);
        nodo->datos.identificador = value.identificador;
        nodo->datos.arrayLineas.linea = value.arrayLineas.linea;
        HashNode[clave] = *nodo;
    }
}

int obtenerNodoHash(char *key, componenteLexico *clIn) {
    int clave;
    int i = 0;
    int tam = strlen(key) + 1;
    clave = funcionHash(key);//Se aplica la función HASH para localizar el nodo con el componente lexico con el lexema que se esta buscando.
    struct nodoHash *nodoActual;
    nodoActual = &HashNode[clave];
    while (nodoActual != NULL) {//Se busca en la lista enlazada que hay en el nodo.
        if (HashNode[clave].datos.lexema != NULL) {
            if (strcmp(nodoActual->datos.lexema, key) == 0) {
                clIn->identificador = nodoActual->datos.identificador;
                clIn->lexema = (char*) malloc(sizeof (char)*tam);
                strcpy(clIn->lexema, nodoActual->datos.lexema);
                clIn->arrayLineas.linea = nodoActual->datos.arrayLineas.linea;
                i = 1;
                break;//si lo encuentra se almacena una estructura componenteLexico en la direccion apuntada por el argumento.
            }
        }
        nodoActual = nodoActual->siguienteNodo;//Se desplaza en la lista enlazada.
    }
    return i;//Si lo encuentra devuelve 1 en caso contrario devuelve 0
}

void LiberarHash() {
    int i = 0;
    for (i = 0; i < NHASH; i++) {
        struct nodoHash *nodoSiguiente;
        struct nodoHash *nodoActual;
        struct linea *lineaSiguiente;
        nodoActual = &HashNode[i];
        while (nodoActual->siguienteNodo != NULL) {//Libera cada NodoSiguiente de cada elemento de la lista enlazada.
            nodoSiguiente = nodoActual->siguienteNodo->siguienteNodo;
            while (nodoActual->datos.arrayLineas.siguienteLinea != NULL) {//libera las lineas en las que aparece ese componente lexico.
                lineaSiguiente = nodoActual->datos.arrayLineas.siguienteLinea->siguienteLinea;
                free(nodoActual->datos.arrayLineas.siguienteLinea);
                nodoActual->datos.arrayLineas.siguienteLinea = lineaSiguiente;
            }
            free(nodoActual->siguienteNodo->datos.lexema);//libera el lexema del componente lexico de cada nodo.
            free(nodoActual->siguienteNodo);//libera nodoSIguiente de cada nodo
            nodoActual->siguienteNodo = nodoSiguiente;
        }
    }
    free(HashNode);//Libera el array de nodos HASH.
}

int anhadirLinea(componenteLexico value) {
    int clave;
    int i = 0;
    struct linea* aux;
    clave = funcionHash(value.lexema);
    struct nodoHash *nodoActual;
    nodoActual = &HashNode[clave];
    while (nodoActual != NULL) {
        if (HashNode[clave].datos.lexema != NULL) {
            if (strcmp(nodoActual->datos.lexema, value.lexema) == 0) {//Busca el componente lexico en tabla HASH. Si lo encuentra anhade una linea en la lista de lineas en las que aparece cada componente lexico.
                if (nodoActual->datos.arrayLineas.linea == 0) {
                    nodoActual->datos.arrayLineas.linea = value.arrayLineas.linea;
                } else {
                    aux = (linea*) malloc(sizeof (struct linea));
                    aux->linea = nodoActual->datos.arrayLineas.linea;
                    aux->siguienteLinea = nodoActual->datos.arrayLineas.siguienteLinea;
                    nodoActual->datos.arrayLineas.siguienteLinea = aux;
                    nodoActual->datos.arrayLineas.linea = value.arrayLineas.linea;
                    i = 1;
                    break;
                }
            }
        }
        nodoActual = nodoActual->siguienteNodo;
    }
    return i;
}

void imprimirTablaHash() {
    int i = 0;
    printf("\n        Tabla de Simbolos\n");
    printf("        -----------------\n\n");
    for (i = 0; i < NHASH; i++) {
        struct nodoHash *nodoActual;
        struct linea *lineaActual;
        nodoActual = &HashNode[i];
        while (nodoActual != NULL) {//Recorre todos los nodos de la tabla hash,dentro de los nodos, la lista enlazada que contiene cada nodo, y dentro de cada nodo, la lista enlazada de lineas en las que aparece el componente lexico que contiene cada nodo.
            if (nodoActual->datos.lexema != NULL) {
                printf("        Identificador:%3d  Lexema:%15s  Lineas:|",
                        nodoActual->datos.identificador,
                        nodoActual->datos.lexema);
                lineaActual = &nodoActual->datos.arrayLineas;
                while (lineaActual != NULL) {//Recorre la lista enlazada de lineas en las que aparece cada componente lexico de cada nodo HASH
                    printf("%2d|", lineaActual->linea);
                    lineaActual = lineaActual->siguienteLinea;
                }
                printf("\n");
                nodoActual = nodoActual->siguienteNodo;
            } else break;
        }
    }
}

//Funcion encargada de almacenar cada una de las palabras reservadas en la tabla HASH.
void anhadirPalabraReservada(char* lex, int identificador) {
    componenteLexico aux;
    aux.lexema = (char*) malloc(sizeof (char)*strlen(lex));
    strcpy(aux.lexema, lex);
    aux.identificador = identificador;
    aux.arrayLineas.linea = 0;
    anhadirNodoHash(aux);
    free(aux.lexema);
}

void precargarTablaHash() {
    anhadirPalabraReservada("and", RESER_AND);
    anhadirPalabraReservada("break", RESER_BREAK);
    anhadirPalabraReservada("def", RESER_DEF);
    anhadirPalabraReservada("elif", RESER_ELIF);
    anhadirPalabraReservada("else", RESER_ELSE);
    anhadirPalabraReservada("for", RESER_FOR);
    anhadirPalabraReservada("if", RESER_IF);
    anhadirPalabraReservada("import", RESER_IMPORT);
    anhadirPalabraReservada("in", RESER_IN);
    anhadirPalabraReservada("is", RESER_IS);
    anhadirPalabraReservada("not", RESER_NOT);
    anhadirPalabraReservada("or", RESER_OR);
    anhadirPalabraReservada("print", RESER_PRINT);
    anhadirPalabraReservada("return", RESER_RETURN);
    anhadirPalabraReservada("while", RESER_WHILE);
}

