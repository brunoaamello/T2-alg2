#include "databroker.h"

dataBroker::dataBroker(){
    _indice=nullptr;
}

void dataBroker::assignIndice(Indice* ind){
    _indice=ind;
}

void dataBroker::addData(Register reg){
    _activeReg=reg;
    ofstream outfile("dados.txt");
    outfile.seekp(ios::end);
    _activeReg.assignRRN(outfile.tellp());
    outfile << _activeReg.toStrReg() << endl;
    outfile.close();
    _indice->addReg(_activeReg);
}

void dataBroker::removeData(unsigned number){
    if(number == _activeReg.getNumber() && _activeReg.getRRN() != -1){
        _indice->removeReg(number);

        _activeReg.assignRRN(-1);
    }else{
        if(_indice->removeReg(number)){
            cout << "Registro não encontrado." << endl;
        }else{

        }
    }
}

bool dataBroker::changeData(unsigned number){

}

void dataBroker::findData(unsigned number){

}

void dataBroker::shrinkData(){

}

