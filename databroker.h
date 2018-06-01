/*  Trabalho 2 - Algoritmos e Estruturas de Dados II
 *  Bruno Arantes de Achilles Mello
 *  9866490
 */

#ifndef DATABROKER_H
#define DATABROKER_H
#include <fstream>
#include <stdio.h>
#include "indice.h"

//Essa classe é responsável por gerir a entrada e saída dos dados, fazendo interface com a classe Indice e lendo/escrevendo o arquivo de dados.
//This class is responsible for managing the data input and output, interfacing with the Indice class and reading/writing to the data file.

class dataBroker{
private:
    Indice* _indice;                //Indice do arquivo de dados - Data file Indice
    Register _activeReg;            //Registro ativo para propósitos de caching - Active register for caching purposes
    string _dataFile;               //Nome do arquivo de dados - Name of the data file
    void removeFromData(int rrn);
    bool readInitial();
public:
    dataBroker(string dataFile = "dados.txt");
    ~dataBroker();
    bool addData(Register reg);
    bool removeData(unsigned number);
    bool readRegister(int rrn);
    bool changeData(unsigned number);
    void findData();
    void shrinkData();
};

#endif // DATABROKER_H
