#ifndef DATABROKER_H
#define DATABROKER_H
#include <fstream>
#include "indice.h"

class dataBroker{
private:
    Indice* _indice;
    Register _activeReg;
    void removeFromData(int rrn);
    bool readInitial();
public:
    dataBroker();
    bool addData(Register reg);
    bool removeData(unsigned number);
    bool readRegister(int rrn);
    bool changeData(unsigned number);
    void findData(unsigned number);
    void shrinkData();
};

#endif // DATABROKER_H
