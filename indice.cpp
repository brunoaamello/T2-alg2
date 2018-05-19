#include "indice.h"

Indice::Indice(string dataFile, string indexFile, string lockFile){
    _dataFile=dataFile;
    _indexFile=indexFile;
    _lockFile=lockFile;
    _indices.clear();
    _lastPos=0;
    ifstream lock(_lockFile);
    if(lock.good()){                               //exited unsafely in the past
        cout << "Indice corrompido." << endl;
        cout << "Reconstruindo índice";
        ifstream infile(_dataFile);
        if(!infile.good()){
            infile.close();
            char choice;
            cout << endl << "ERRO FATAL ARQUIVO DE DADOS NÃO ENCONTRADO" << endl;
            cout << "Deseja continuar a execução sem os dados?(s/n)" << endl;
            cin >> choice;
            if(choice != 's'){
                cout << "Encerrando o programa." << endl;
                remove(_lockFile.c_str());
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
        ofstream{ _lockFile };
        cout << ".";
        readIndices();
        cout << "." << endl;
    }
}

Indice::~Indice(){
    buildFile();
}

void Indice::retrieveIndices(){
    ifstream infile(_dataFile);
    string line, val;
    int number;
    unsigned rrn;
    rrn = infile.tellg();
    getline(infile, line);
    while(line.size()>3){
        if(line.at(1)=='$'){
            getline(infile, line);
            continue;
        }
        for(int i=1;line.at(i)!='|';i++){
            val+=line.at(i);
        }
        number = stoi(val);
        val.clear();
        _indices.insert(_indices.end(), make_pair(number, rrn));
        line.clear();
        rrn = infile.tellg();
        getline(infile, line);
    }
    infile.close();
    sort(_indices.begin(), _indices.end());
}

void Indice::readIndices(){
    ifstream infile(_indexFile);
    string line, val;
    unsigned i;
    unsigned number;
    int rrn;
    while(infile >> line){
        val.clear();
        for(i=0;line.at(i)!='|';i++){
            val+=line.at(i);
        }
        i++;
        number = (unsigned int)stoul(val);
        val.clear();
        for(;i<line.size();i++){
            val+=line.at(i);
        }
        rrn = stoi(val);
        line.clear();
        _indices.insert(_indices.end(), make_pair(number, rrn));
    }
    sort(_indices.begin(), _indices.end());
}

void Indice::buildFile(){
    remove(_indexFile.c_str());
    ofstream{ _indexFile };
    ofstream outfile(_indexFile);
    for(auto it=_indices.begin();it!=_indices.end();it++){
        outfile << it->first << '|' << it->second << endl;
    }
    outfile.close();
    remove(_lockFile.c_str());
}

void Indice::addReg(Register reg){
    _indices.insert(_indices.end(), make_pair(reg.getNumber(), reg.getRRN()));
    sort(_indices.begin(), _indices.end());
}

int Indice::getPos(unsigned number){
    if((unsigned)_lastPos<_indices.size()){
        if(_indices[_lastPos].first==number){
            return _lastPos;
        }
    }
    int lower = 0;
    int higher = _indices.size()-1;
    int res;
    int mid;
    while(higher!=lower && lower<higher){
        mid=(3*lower + higher)/2;
        if(binary_search(_indices.begin()+lower, _indices.begin()+higher, number, comp_pair_un())){
            higher=mid-1;
        }else{
            lower=mid+1;
        }
    }
    if((unsigned)lower < _indices.size() && (unsigned)higher < _indices.size()){
        if(_indices[lower].first==number){
            res=lower;
        }else if(_indices[higher].first==number){
            res=higher;
        }else{
            res=-1;
        }
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


