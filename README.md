# Python Lexical Analyzer
Python lexical analyzer written in C. It recognizes the Python subset existent in the wilcoxon.py file.

## Steps to run the program:

Build the project:
```bash
make
```
Format:
```bash
./Compilador <arg>
```     

Run the analyzer:
```bash
./Compilador wilcoxon.py
```

where:
- **\<arg>**: The name of the python that is going to be analyzed.

Clean generated files:
```bash
make clean
```

In case you want to use GCC directly, use gcc *.c -lm since the program uses the maths library for the intToArray(); function of the AnalizadorLexico.c
