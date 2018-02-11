
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Definiciones.h"
#include "AnalizadorLexico.h"
#include "SistemaDeEntrada.h"
#include "GestionDeErrores.h"
#include "TablaDeSimbolos.h"

//La funcion no Consumir retrocede una posicion del puntero delantero con el objetivo de leer
//el caracter anterior ya leido de nuevo.Ejemplo de esta situacion es: lexema.
//En este caso se devuelve lexema, no se consume el punto, y se devuelve el punto posteriormente.
static inline void noConsumir() {
    empiezaLexema();
    retrocesoDelantero();
    if (caracter == '\n')//Si no se consume \n se retrocede una linea en la posicion
        localizacionDelantero.linea--;
}

//Devuelve el siguiente caracter del fichero
static inline int siguiente_caracter() {
    caracter = consumir_caracter();
    localizacionDelantero.columna++;//se avanza una posicion en columna
    if (caracter == '\n') {
        localizacionDelantero.linea++;//se avanza una posicion en linea si es \n
        localizacionDelantero.columna = 1;//columna =1 si avanza la linea
    }
    return caracter;
}

static inline void almacenarCaracter() {//almacena un caracter en el lexema de la estructura
    if (compLex.lexema == 0) {//si lexema es nulo reserva, si no, redimensiona
        compLex.lexema = (char*) malloc(sizeof (char)*2);//reserva espacio para 1 caracter y fin de linea
    } else if (pLex < N) {//Tamaño del lexema no puede ser superior a tamaño de buffer N
        compLex.lexema = (char*) realloc(compLex.lexema, (pLex + 2));//redimensiona para 1 caracter más y fin de linea
    } else
        gestionarError(ERROR_BUFFEROVERFLOW, localizacionInicio.columna, localizacionInicio.linea);
    compLex.lexema[pLex] = caracter;
    compLex.lexema[pLex + 1] = '\0';
    pLex++;
}

//Comprueba si el identificador reconocido es una palabra reservada de las precargadas. En caso de no serlo se guarda en la tabla.
static inline int introducirTablaSimbolos() {
    if (obtenerNodoHash(compLex.lexema, &compLex) != 0) {//obtiene la estructura componente lexico del nodo hash a traves del lexema
        compLex.arrayLineas.linea = localizacionDelantero.linea;
        anhadirLinea(compLex);//anhado linea a la lista de lineas en las que aparece el componente lexico obtenido
        return compLex.identificador;
    } else {//si no obtiene el nodo hash, guardo componente en tabla de simbolos como identificador
        compLex.identificador = 300;
        anhadirNodoHash(compLex);
        return IDENTIFICADOR;
    }
}

static inline void InicializarAnalizadorLexico() {//inicializa todos los flags
    estadoInterno = 0;
    estado = ESTADO_INICIAL;
    estadoFinal = ESTADO_NOFINAL;
    nivelPrioridad = 0;
    if (inicio == 0) {//si es inicio del programa inicializo la posicion.
        localizacionDelantero.columna = 1;
        localizacionDelantero.linea = 1;
    }

}

static inline void EstadoInicial() {
    free(compLex.lexema);//se libera el lexema de la estructura
    compLex.lexema = 0;
    pLex = 0;//se reinicia la posicion del lexema
    inicio = 1;//comenzo la ejecucion del analizador lexico
    empiezaLexema();//puntero inicio del sistema de entrada se iguala a delantero
    compLex.arrayLineas.linea = localizacionDelantero.linea;//guarda linea del componente lexico en la estructura
    localizacionInicio.linea = localizacionDelantero.linea;
    localizacionInicio.columna = localizacionDelantero.columna;//localizacion inicio se situa al principio del lexema
    if (caracter == ' ' || caracter == '\t');
    else if (isalpha(caracter) || caracter == '_') {
        almacenarCaracter();//se almacena el caracter que se acaba de leer
        estado = IDENTIFICADOR;//Se iguala el estado de un autómata "mayor" a identificador,se almacena y posteriormente un switch case reconocerá el estado y se ejecutará la función que contiene los estados que reconocen ese componente lexico.
    } else if (isdigit(caracter) && caracter != '0') {
        almacenarCaracter();
        estado = ENTERO;//Lo mismo que el anterior y lo mismo para el resto de else if.
    } else if (caracter == '.') {
        almacenarCaracter();
        estado = DECIMAL;
    } else if (caracter == '=') {
        almacenarCaracter();
        estado = IGUAL;
    } else if (caracter == '#') {
        estado = COMENTARIOS;
    } else if (caracter == '\'') {
        almacenarCaracter();
        estado = STRING;
    } else if (caracter == '\"') {
        almacenarCaracter();
        switch (nivelPrioridad) {
            case 0://si se lee " se prueba si es comentario multilinea. Si no lo es...
                estado = COMENTARIO_MULTILINEA;
                break;
            case 1://... nivel de prioridad 1 y se prueba si es un STRING.
                estadoInterno = 1;
                estado = STRING;
                nivelPrioridad = 0;//se reinicia el nivel de prioridad.
                break;
        }
    } else if (caracter == '0') {
        almacenarCaracter();
        estado = CERO;
    } else if (caracter == '\n') {
        estado = IDENTACION;
    } else if (caracter == '-') {
        almacenarCaracter();
        estado = ASIGNACION_RESTA;
    } else if (caracter == '+') {
        almacenarCaracter();
        estado = ASIGNACION_SUMA;
    } else if (caracter == '<') {
        almacenarCaracter();
        estado = MENOR_IGUAL;
    } else if (caracter == '>') {
        almacenarCaracter();
        estado = MAYOR_IGUAL;
    } else if (caracter == '*') {
        almacenarCaracter();
        estado = POTENCIA;
    } else if (ispunct(caracter)
            && caracter != '$'//python no acepta ni $ ni ? por lo tanto no entra en la condicion.
            && caracter != '?') {
        if (caracter == '(' ||//se abre un delimitador de estos (, [, {...
                caracter == '[' ||
                caracter == '{')
            haydelimitadorabierto++;//...entonces se suma 1 al flag.
        if (caracter == ')'
                || caracter == ']'
                || caracter == '}')//se cierra delimitador...
            haydelimitadorabierto--;//... se resta al flag
        almacenarCaracter();
        estadoFinal = caracter;//estado final es el propio caracter y posteriormente se devuelve al sintactico.
    } else if (caracter == EOF) {
        estadoFinal = EOF;//estado final es EOF y se devuelve al sintactico
    } else {
        gestionarError(ERROR_LEXICO, localizacionInicio.columna, localizacionInicio.linea);//si no se entra en ningun if error.
    }
}

static inline void Identificador() {//funcion que contiene todas las comprobaciones para verificar si es un identificador
    if (ISESPACIO) {
        estadoFinal = introducirTablaSimbolos();
    } else if (ISIDENTIFICADOR) {//si el caracter pertenece al lexema de un componente lexico identificador se almacena y se mantiene en el estado.
        almacenarCaracter();
        estado = IDENTIFICADOR;
    } else {
        NOCONSUMIR
        estadoFinal = introducirTablaSimbolos();//se comprueba si es palabra reservada y se devuelve el identificador del componente.
    }
}

static inline void Entero() {//funcion que verifica si lexema es un entero.
    if (ISESPACIO) {
        estadoFinal = ENTERO;
    } else if (isdigit(caracter)) {
        almacenarCaracter();
        estado = ENTERO;
    } else if (caracter == '.') {//si despues de un entero hay punto es decimal.
        almacenarCaracter();
        estadoInterno = 1;//estado interno del automata decimal que indica que se leyo un numero y no un punto. Ir a decimal para ver con detalle.
        estado = DECIMAL;
    } else if (caracter == 'e'
            && nivelPrioridad == 0) {//si el automata que verifica la notacion cientifica falla, nivelPrioridad=1 y no se contempla este else if la siguiente vez
        almacenarCaracter();
        estado = NOTACION_CIENTIFICA;
    } else {
        NOCONSUMIR
        estadoFinal = ENTERO;
    }
}

static inline void Decimal() {
    switch (estadoInterno) {//automata que verifica si es decimal. Estado interno se corresponde con el estado actual de este automata.
        case 0://estado de haber leido un punto
            if (isdigit(caracter)) {
                almacenarCaracter();
                estadoInterno++;
            } else {
                NOCONSUMIR;//el siguiente caracter no se consume.
                estadoFinal = '.';//si solo se lee un punto se devuelve un punto al sintactico.
            }
            break;
        case 1://habiendo leido un numero
            if (ISESPACIO) {
                estadoFinal = DECIMAL;
            } else if (isdigit(caracter)) {
                almacenarCaracter();
            } else if (caracter == 'e' && nivelPrioridad == 0) {//lo mismo que para enteros.
                almacenarCaracter();
                estadoInterno = 0;
                estado = NOTACION_CIENTIFICA;
            } else {
                NOCONSUMIR
                estadoFinal = DECIMAL;
            }
            break;
    }
}

static inline void Cero() {
    if (ISESPACIO) {//si es un espacio o un tabulador
        estadoFinal = ENTERO;
    } else if ((caracter == 'x'//si se lee una x se va al automata que reconoce hexadecimales.
            || caracter == 'X')
            && nivelPrioridad == 0) {
        almacenarCaracter();
        estado = HEXADECIMAL;
    } else if (caracter == 'e' && nivelPrioridad == 0) {//explicado en la funcion entero
        almacenarCaracter();
        estado = NOTACION_CIENTIFICA;
    } else if (caracter == '.') {
        almacenarCaracter();
        estadoInterno = 1;
        estado = DECIMAL;
    } else {
        NOCONSUMIR
        estadoFinal = ENTERO;
    }
}

static inline void AsignacionSuma() {//Reconoce el componente +=
    if (ISESPACIO) {//si es un espacio se devuelve el mas que se leyo en el estado inicial
        estadoFinal = '+';
    } else if (caracter == '=') {//si el siguiente caracter es un = se devuelve +=
        almacenarCaracter();
        estadoFinal = ASIGNACION_SUMA;
    } else {//en caso de que siguiente caracter sea cualquier cosa distinto de espacio o =, se devuelve + y no se consume el caracter actual.
        NOCONSUMIR
        estadoFinal = '+';
    }
}

static inline void AsignacionResta() {
    if (ISESPACIO) {//Ya comentado en asignacion suma
        estadoFinal = '-';
    } else if (caracter == '=') {
        almacenarCaracter();
        estadoFinal = ASIGNACION_RESTA;
    } else {
        NOCONSUMIR
        estadoFinal = '-';
    }

}

static inline void MenorIgual() {
    if (ISESPACIO) {//Ya comentado en asignacion suma
        estadoFinal = '<';
    } else if (caracter == '=') {
        almacenarCaracter();
        estadoFinal = MENOR_IGUAL;
    } else {
        NOCONSUMIR
        estadoFinal = '<';
    }

}

static inline void MayorIgual() {
    if (ISESPACIO) {//Ya comentado en asignacion suma
        estadoFinal = '>';
    } else if (caracter == '=') {
        almacenarCaracter();
        estadoFinal = MAYOR_IGUAL;
    } else {
        NOCONSUMIR
        estadoFinal = '>';
    }

}

static inline void Comentario() {
    if (caracter == '\n') {//cuando se encuentra \n se vuelve al estado inicial. Mientras sigue en estado comentario se saltan los caracteres.
        noConsumir();
        estado = ESTADO_INICIAL;
    }
    empiezaLexema();//se adelanta el puntero inicio para que no se quede un buffer atras.

}

static inline void NotacionCientifica() {//si mientras se lee un numero o un decimal se lee una e se viene a esta función.
    switch (estadoInterno) {
        case 0://estado que reconoce el signo - y el +
            if (caracter == '-' || caracter == '+') {
                almacenarCaracter();
                estadoInterno++;
            } else if (isdigit(caracter)) {
                almacenarCaracter();
                estadoInterno = 2;
            } else {
                RETROCESO;
            }
            break;
        case 1://estado que reconoce 1 digito
            if (isdigit(caracter)) {
                almacenarCaracter();
                estadoInterno++;
            } else {
                RETROCESO;
            }
            break;
        case 2://estado que reconoce n digitos
            if (isdigit(caracter)) {
                almacenarCaracter();
            } else if (ISESPACIO) {
                estadoFinal = NOTACION_CIENTIFICA;
            } else {
                NOCONSUMIR
                estadoFinal = NOTACION_CIENTIFICA;
            }
            break;
    }
}

static inline void String() {//cadenas de texto
    switch (estadoInterno) {
        case 0://estado al que se viene cuando se detecta un string con comilla simple.
            if (caracter == '\'') {//almacena todos los caracteres hasta que encuentra una comilla simple.
                almacenarCaracter();
                estadoFinal = STRING;
            } else if (ISFINDELINEA) {
                gestionarError(ERROR_COMILLAS, localizacionInicio.columna, localizacionInicio.linea);
            } else
                almacenarCaracter();
            break;
        case 1://estado al que se viene cuando se detecta un string con comilla doble.
            if (caracter == '\"') {//lo mismo que la simple.
                almacenarCaracter();
                estadoFinal = STRING;
            } else if (ISFINDELINEA) {
                gestionarError(ERROR_COMILLAS, localizacionInicio.columna, localizacionInicio.linea);
            } else
                almacenarCaracter();
            break;
    }
}

static inline void ComentarioMultilinea() {
    switch (estadoInterno) {
        case 0://estado despues de haber leido una comilla doble. La primera se lee en el estado inicial.
            if (caracter == '\"') {
                almacenarCaracter();
                estadoInterno++;//si lee una comilla avanza al estado en el que se leyeron 2 comillas
            } else if (ISFINDELINEA) {
                noConsumir();//si es fin de linea, no se consume y se va al estado STRING.
                estadoInterno = 1;
                estado = STRING;
            } else {
                almacenarCaracter();
                estadoInterno = 1;//Si es otro caracter se va al estadoInterno 1 de STRING que se corresponde con es STRING que termina con comillas dobles. 
                estado = STRING;
            }
            break;
        case 1://estado despues de haber leido dos comillas dobles
            if (caracter == '\"') {
                almacenarCaracter();
                estadoInterno++;
            } else {
                RETROCESO;//nivelPrioridad=1 y retrocedemos el puntero delantero a punteroInicio
            }
            break;
        case 2:
            if (caracter == '\"') {//si se lee una comilla se avanza al estado en el que se leyo una comilla
                almacenarCaracter();
                estadoInterno++;
            } else if (caracter == EOF) {//en caso de encontrar EOF se retrocede y se prueba con el automata STRING
                RETROCESO
            } else if (caracter == '\n') {
                almacenarCaracter();
            } else
                almacenarCaracter();
            break;
        case 3:
            if (caracter == '\"') {
                almacenarCaracter();
                estadoInterno++;
            } else
                estadoInterno = 2;//en caso de que siguiente caracter no sea una comilla se vuelve al estadoInterno 2 y se siguen almacenando caracteres.
            break;
        case 4:
            if (caracter == '\"') {
                almacenarCaracter();
                estadoFinal = COMENTARIO_MULTILINEA;//si se lee una tercera comilla se devuelve el componente lexico comentario multilinea
            } else
                estadoInterno = 2;//si no se retrocede al estadoInterno 2
            break;
    }
}

static inline void Igual() {
    if (ISESPACIO) {//similar a asignacion suma ya comentado
        estadoFinal = '=';
    } else if (caracter == '=') {
        almacenarCaracter();
        estadoFinal = IGUAL;
    } else {
        NOCONSUMIR
        estadoFinal = '=';
    }

}

static inline void Potencia() {
    if (ISESPACIO) {//lo mismo que el anterior.
        estadoFinal = '*';
    } else if (caracter == '*') {
        almacenarCaracter();
        estadoFinal = POTENCIA;
    } else {
        NOCONSUMIR
        estadoFinal = '*';
    }
}


void intToArray(int number) {
    int i, n = 1;
    if (number != 0)
        n = log10(number) + 1;//calcula número de digitos que tiene el número para posteriormente reservar tamaño en el lexema y almacenar el número.
    compLex.lexema = (char*) malloc(sizeof (char)*(n + 1));
    compLex.lexema[n] = '\0';
    for (i = 0; i < n; ++i, number /= 10) {
        compLex.lexema[(n - 1) - i] = (char) '0' + number % 10;//for inverso para empezar por la derecha.
    }
}


static inline void Hexadecimal() {//En vez de devolver un lexema con el hexadecimal, se hace la conversion con strtol y se devuelve un entero.
    switch (estadoInterno) {
        case 0://se ha leido 0x
            if (isxdigit(caracter)) {//si se lee un hexadecimal se pasa al estadointerno 1
                almacenarCaracter();
                estadoInterno++;
            } else {
                RETROCESO;
            }
            break;
        case 1://se ha leido 0x y un numero hexadecimal
            if (isxdigit(caracter)) {//si el siguiente es hexadecimal se sigue almacenando
                almacenarCaracter();
            } else {//si no se devuelve.
                if (!ISESPACIO)noConsumir();
                int number = (int) strtol(compLex.lexema, NULL, 0);//transforma numero en hexadecimal a entero.
                free(compLex.lexema);//se hace un free del numero en formato hexadecimal en el lexema.
                intToArray(number);//se guarda numero entero obtenido del hexadecimal en el lexema.
                estadoFinal = ENTERO;
            }
            break;
    }
}

static inline void Identacion() {
    if (caracter == ' ') pLex++;//se aumenta el contador de numero de espacios.
    else {
        NOCONSUMIR
        if (pLex != 0 && caracter != '#' && haydelimitadorabierto == 0) {//en caso de encontrar un caracter diferente de # y de no haber ningun delimitador abierto....
            intToArray(pLex);//.... se almacena el numero de espacios leidos en el lexema
            estadoFinal = IDENTACION;//y se devuelve el componente identacion.
        }
    }
}

int siguiente_comp_lexico() {
    InicializarAnalizadorLexico();
    while (estado != EOF) {//mientras el estado sea distinto de final de fichero.
        siguiente_caracter();
        switch (estado) {//Automata externo en donde cada uno de los case(estados) contiene una función que contiene estados mas pequeños que reconocen un componente lexico.
            case ESTADO_INICIAL:EstadoInicial();
                break;
            case IDENTIFICADOR:Identificador();//Ejemplo:Este estado "mayor" identificador contiene los estados "mas pequeños" que llevan al estado final Identificador.
                break;
            case ENTERO:Entero();//como en el caso anterior para cada uno de los componentes lexicos.
                break;
            case DECIMAL:Decimal();
                break;
            case IGUAL:Igual();
                break;
            case COMENTARIOS:Comentario();
                break;
            case ASIGNACION_SUMA:AsignacionSuma();
                break;
            case ASIGNACION_RESTA:AsignacionResta();
                break;
            case MENOR_IGUAL:MenorIgual();
                break;
            case MAYOR_IGUAL:MayorIgual();
                break;
            case COMENTARIO_MULTILINEA:ComentarioMultilinea();
                break;
            case CERO:Cero();
                break;
            case HEXADECIMAL:Hexadecimal();
                break;
            case STRING:String();
                break;
            case POTENCIA:Potencia();
                break;
            case NOTACION_CIENTIFICA:NotacionCientifica();
                break;
            case IDENTACION:Identacion();
                break;
        }
        if (estadoFinal != ESTADO_NOFINAL)//estado no_final==-2. Si se llega a un estado final se cambia por el identificador del componente y se devuelve.
            return estadoFinal;//al ejecutar de nuevo siguiente_componente esta variable se reinicia de nuevo a -2
    }
    free(compLex.lexema);//si se llega a un EOF se libera el lexema de la estructura.
}