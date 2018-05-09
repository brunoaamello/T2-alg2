#ifndef DATABROKER_H
#define DATABROKER_H
#include <fstream>
#include "indice.h"

class dataBroker{
private:
    Indice* _indice;
    Register _activeReg;
public:
    dataBroker();
    void assignIndice(Indice* ind);
    void addData(Register reg);
    void removeData(unsigned number);
    bool changeData(unsigned number);
    void findData(unsigned number);
    void shrinkData();
};

#endif // DATABROKER_H
