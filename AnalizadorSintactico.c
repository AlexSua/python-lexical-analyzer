

#include <stdio.h>
#include <stdlib.h>
#include "AnalizadorLexico.h"
#include "TablaDeSimbolos.h"
#include "GestionDeErrores.h"
#include "SistemaDeEntrada.h"

int main(int argc, char** argv) {
    if(argv[1]==NULL)
        gestionarError(ERROR_ARGUMENTO,0,0);
    nombreArchivo=argv[1];//Se obtiene el nombre del archivo como argumento
    int a;
    crearTablaHash();
    while (a != EOF) {
        a = siguiente_comp_lexico();
        printf("        Linea:%2d........"
                "Identificador:%3d:...... %s \n", 
                compLex.arrayLineas.linea, 
                a, compLex.lexema);
    }
    imprimirTablaHash();
    LiberarHash();
    return (EXIT_SUCCESS);
}

