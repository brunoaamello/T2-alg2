#include "indice.h"

Indice::Indice(){
    _indices.clear();
    _lastPos=0;
    ifstream lock("primario.lock");
    if(lock.good()){                               //exited unsafely in the past
        cout << "Indice corrompido." << endl;
        cout << "Reconstruindo índice";
        ifstream infile("dados.txt");
        if(!infile.good()){
            infile.close();
            char choice;
            cout << endl << "ERRO FATAL ARQUIVO DE DADOS NÃO ENCONTRADO" << endl;
            cout << "Deseja continuar a execução sem os dados?(s/n)" << endl;
            cin >> choice;
            if(choice != 's'){
                remove("primario.lock");
                exit(-1);
            }
        }else{
            infile.close();
            cout << ".";
            retrieveIndices();
            cout << "." << endl;
        }
    }else{                                          // last exit was safe
        cout << "Lendo Indice.";
        ofstream{ "primario.lock" };
        cout << ".";
        readIndices();
        cout << "." << endl;
    }
}

void Indice::retrieveIndices(){
    ifstream infile("dados.txt");
    string line, val;
    int number;
    unsigned rrn;
    rrn = infile.tellg();
    while(infile >> line){
        for(int i=1;line.at(i)!='|';i++){
            val+=line.at(i);
        }
        number = stoi(val);
        val.clear();
        _indices.insert(_indices.end(), make_pair(number, rrn));
        line.clear();
        rrn = infile.tellg();
    }
    sort(_indices.begin(), _indices.end());
}

void Indice::readIndices(){
    ifstream infile("primario.ndx");
    string line, val;
    unsigned i;
    int number;
    unsigned rrn;
    while(infile >> line){
        val.clear();
        for(i=0;line.at(i)!='|';i++){
            val+=line.at(i);
        }
        number = stoi(val);
        val.clear();
        for(;i<line.size();i++){
            val+=line.at(i);
        }
        rrn = stoul(val);
        line.clear();
        _indices.insert(_indices.end(), make_pair(number, rrn));
    }
    sort(_indices.begin(), _indices.end());
}

void Indice::buildFile(){
    remove("primario.ndx");
    ofstream outfile("primario.ndx");
    for(auto it=_indices.begin();it!=_indices.end();it++){
        outfile << it->first << '|' << it->second << endl;
    }
    outfile.close();
}

void Indice::addReg(Register reg){
    _indices.insert(_indices.end(), make_pair(reg.getNumber(), reg.getRRN()));
    sort(_indices.begin(), _indices.end());
}

int Indice::getPos(unsigned number){
    if(_indices[_lastPos].first==number){
        return _lastPos;
    }
    int lower = 0;
    int higher = _indices.size()-1;
    int res;
    while(higher!=lower && lower<higher){
        if(binary_search(_indices.begin()+lower, _indices.begin()+higher/2, number, comp_pair_un())){
            higher/=2;
        }else{
            lower=higher/2;
        }
    }
    if(_indices[lower].first==number){
        res=lower;
    }else if(_indices[higher].first==number){
        res=higher;
    }else{
        res=-1;
    }
    _lastPos=res;
    return res;
}

int Indice::getRRN(unsigned number){
    int pos = getPos(number);
    if(pos == -1){
        return -1;
    }else{
        return _indices[pos].second;
    }
}

bool Indice::removeReg(unsigned number){
    int pos = getPos(number);
    if(pos == -1){
        return false;
    }else{
        _indices.erase(_indices.begin()+pos);
        return true;
    }
}


