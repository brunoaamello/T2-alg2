/*  Trabalho 2 - Algoritmos e Estruturas de Dados II
 *  Bruno Arantes de Achilles Mello
 *  9866490
 */

#ifndef INDICE_H
#define INDICE_H
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "register.h"

using namespace std;

class Indice{
private:
    //Lista de par (número,rrn) - Pair list (number,rrn)
    vector<pair<unsigned, int> > _indices;
    //Última posição acessada, para caching - Last valid requested position, for caching
    int _lastPos;
    //Nomes dos arquivos - File names
    string _dataFile;
    string _indexFile;
    string _lockFile;
    int getPos(unsigned number);
public:
    Indice(string dataFile, string indexFile = "primario.ndx", string lockFile = "primario.lock");
    ~Indice();
    void retrieveIndices();
    bool readIndices();
    void buildFile();
    void addReg(Register reg);
    int getRRN(unsigned number);
    bool removeReg(unsigned number);
    vector<pair<unsigned, int> > getNumberList();
    void clear();
};

#endif // INDICE_H
