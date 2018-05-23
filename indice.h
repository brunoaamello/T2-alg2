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
    vector<pair<unsigned, int> > _indices;
    int _lastPos;
    string _dataFile;
    string _indexFile;
    string _lockFile;
    int getPos(unsigned number);
public:
    Indice(string dataFile, string indexFile = "primario.ndx", string lockFile = "primario.lock");
    ~Indice();
    void retrieveIndices();
    void readIndices();
    void buildFile();
    void addReg(Register reg);
    int getRRN(unsigned number);
    bool removeReg(unsigned number);
    vector<pair<unsigned, int> > getNumberList();
    void clear();
};

#endif // INDICE_H
