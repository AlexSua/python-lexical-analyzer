CC=gcc
CFLAGS=-lm
DEPS = AnalizadorLexico.h GestionDeErrores.h SistemaDeEntrada.h TablaDeSimbolos.h
OBJ = AnalizadorLexico.o GestionDeErrores.o SistemaDeEntrada.o TablaDeSimbolos.o AnalizadorSintactico.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Compilador: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f AnalizadorLexico.o
	rm -f GestionDeErrores.o 
	rm -f SistemaDeEntrada.o
	rm -f TablaDeSimbolos.o
	rm -f AnalizadorSintactico.o
	rm -f Compilador