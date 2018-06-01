# T2-alg2
github link:
https://github.com/brunoaamello/T2-alg2

####################################################################################################################################
-PT/BR

Segundo trabalho da disciplina Algoritmos e Estruturas de Dados II. Primeiro semestre de 2018.
O Makefile nem sempre funciona, se está usando Fedore e usa o QtCreator ele deve funcionar.
Se este não for o caso compile da seguinte maneira:

g++ -Wall *.cpp *.h -o t2

O programa como está não intercepta comandos como "Ctrl+C", apenas trata situações em que pode ter tido perda de informação.
O script "cleanFiles.sh" deleta os arquivos "dados.txt", "primario.ndx" e "primario.lock" gerados pelo programa, se estiverem presentes.

####################################################################################################################################
-EN/US

Second assignment of the Algorithms and Data Structures II subject. 1st semester of 2018.
Makefile does not always work, if you have Fedora running and uses QtCreator it will probably work.
Otherwise, compile this way:

g++ -Wall *.cpp *.h -o t2

The program as is does not catches "Ctrl+C" commands, only handles situations in which could have occurred data loss.
The script "cleanFiles.sh" deletes the files "dados.txt", "primario.ndx" and "primario.lock" created by the program, if present.
