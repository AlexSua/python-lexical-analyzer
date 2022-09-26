# python-lexical-analyzer
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
where:
- **\<arg>**: The name of the python that is going to be analyzed.

Run the analyzer for wilcoxon.py:
```bash
./Compilador wilcoxon.py
```

Clean generated files:
```bash
make clean
```
