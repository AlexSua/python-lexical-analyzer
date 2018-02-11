
#ifndef GESTIONDEERRORES_H
#define	GESTIONDEERRORES_H

//Macros para errores.
#define ERROR_ABRIR_ARCHIVO 500
#define ERROR_CERRAR_ARCHIVO 501
#define ERROR_LEXICO 502
#define ERROR_LECTURA 503
#define ERROR_COMILLAS 504
#define ERROR_BUFFEROVERFLOW 505
#define ERROR_ARGUMENTO 506

//en caso de error se llama a esta funcion que imprimirá el error junto con su código y el lugar donde se produce
void gestionarError(int num_err,int col,int linea);

#endif	/* GESTIONDEERRORES_H */

