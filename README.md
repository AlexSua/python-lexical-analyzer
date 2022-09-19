# Python Lexical Analyzer
Python lexical analyzer written in C. It recognizes the Python subset existent in the wilcoxon.py file.

## Steps to run the program:

```bash
make
```

```bash
./Compilador <arg>
```     

```bash
./Compilador wilcoxon.py
```

where:
- **argumento**: The name of the python that is going to be analyzed.

make clean To clean generated files

In case you want to use GCC directly, use gcc *.c -lm since the program uses the maths library for the intToArray(); function of the AnalizadorLexico.c
