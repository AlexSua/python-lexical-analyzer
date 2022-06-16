# Python Lexical Analyzer
Python lexical analyzer written in C

## Steps to run the program:

make

./Compilador argumento
             
             -argumento: The name of the file to run the program with. 
              In this case wilcoxon.py

make clean To clean generated files

In case you want to use GCC directly, use gcc *.c -lm since the program uses the maths library for the intToArray(); function of the AnalizadorLexico.c
