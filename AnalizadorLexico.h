

#ifndef ANALIZADORLEXICO_H
#define	ANALIZADORLEXICO_H

#define ESTADO_INICIAL 0
#define COMENTARIOS 1
#define CERO 2
#define HEXADECIMAL 3
#define ESTADO_NOFINAL -2

//NOCONSUMIR se reinicia nivelPrioridad a 0, no se consume el caracter leido y se vuelve a ESTADO_INICIAL
#define NOCONSUMIR nivelPrioridad = 0;noConsumir();estado = ESTADO_INICIAL;

//RETROCESO se iguala punteroDelantero a punteroInicio en el sistema de entrada..Tambien se hace lo mismo con la localizacion.
#define RETROCESO nivelPrioridad = 1; retrocesoDelantero(); estado = ESTADO_INICIAL;localizacionDelantero.linea=localizacionInicio.linea;localizacionDelantero.columna=localizacionInicio.columna;

//ISIDENTIFICADOR si el caracter forma parte del identificador.
#define ISIDENTIFICADOR (isalpha(caracter) || caracter == '_' || isdigit(caracter)) 

//ISESPACIO condicion es espacio o es una tabulacion.
#define ISESPACIO (caracter == ' ' || caracter == '\t')

//ISFINDELINEA condicion es salto de linea o final de fichero.
#define ISFINDELINEA (caracter == '\n' || caracter == EOF)
        
//estructura que enlazara con la siguiente linea en la que aparece un componente lexico.
struct linea{
    int linea;
    struct linea* siguienteLinea;
}typedef linea;

//estructura del componente lexico. Se almacena aqui el lexema, su identificador y una lista enlazada de las lineas en las que aparece.
typedef struct {
    char* lexema;
    int identificador;
    struct linea arrayLineas;//esta lista enlazada se creo con el objetivo de guardar las apariciones de las palabras reservadas e identificadores en la tabla de simbolos.
}componenteLexico;


typedef struct{
    int linea;
    int columna;
}localizacion;

//Estas dos variables controlan la posicion en el fichero del caracter que se esta leyendo.
localizacion localizacionInicio;//Estará al comienzo de cada lexema. En caso de error se devuelve esta posición.
localizacion localizacionDelantero;//Avanza a medida que se consumen caracteres.

//estructura para cada componente lexico. Contiene lexema, identificador y lista enlazada de lineas.
componenteLexico compLex;

//Indica la posicion en la que se está almacenando un lexema. En el caso de identacion se usa como contador de espacios.
int pLex;

//Indica que es el inicio de programa. Se usa para inicializar ciertas variables.
int inicio;

//Cada caracter que se va consumiendo del sistema de entrada.
int caracter;

//Indica el estado actual en el que se encuentra el autómata mayor.
int estado;

//esta variable será -2 hasta que se reconoce un comp lexico. En caso de reconocerlo pasará a valer el identificador del comp Lex y se devuelve.
int estadoFinal;

//Al inicializar vale 0. Si se produce retroceso valdra 1.
int nivelPrioridad;

//indica el estado actual en el que se encuentra un automata pequeño encargado de reconocer un componente lexico determinado.
int estadoInterno;

//Controla la cantidad de delimitadores abiertos. En caso de haberlos, y haber salto de linea indica que no se debe devolver identacion
int haydelimitadorabierto;

//funcion que devuelve un componente lexico reconocido de un fichero.
int siguiente_comp_lexico();

#endif	/* ANALIZADORLEXICO_H */

