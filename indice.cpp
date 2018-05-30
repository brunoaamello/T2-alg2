#include "indice.h"

Indice::Indice(string dataFile, string indexFile, string lockFile){
    _dataFile=dataFile;
    _indexFile=indexFile;
    _lockFile=lockFile;
    _indices.clear();
    _lastPos=-1;
    ifstream lock(_lockFile);
    if(lock.good()){
        //Se a última saída do programa foi insegura - If the last exit was unsafe
        cout << "Indice corrompido." << endl;
        cout << "Reconstruindo índice";
        ifstream infile(_dataFile);
        if(!infile.good()){
            //Se não tem um arquivo de dados - If there is no data file
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
            //Se tem um arquivo de dados, reconstrói o índice - Else, rebuilds the indice
            infile.close();
            cout << ".";
            retrieveIndices();
            cout << "." << endl;
        }
    }else{
        //Se a última saída foi segura, lê o arquivo de índices - If last exit was safe reads the indice file
        cout << "Lendo Indice.";
        ofstream{ _lockFile };
        cout << ".";
        if(!readIndices()){
            //Se houve um problema lendo o arquivo de dados, reconstrói o indice - If there was a problem reading the indice file, rebuilds it
            retrieveIndices();
        }
        cout << "." << endl;
    }
}

Indice::~Indice(){
    buildFile();
}

//Função que reconstrói o índice a partir do arquivo de dados
//Function that rebuilds the indice with the data file
void Indice::retrieveIndices(){
    ifstream infile(_dataFile);
    string line, val;
    int number;
    unsigned rrn;
    //Lê e faz uma separação parcial - Reads and makes a partial parsing
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
    //Ordena as entradas do índice - Sorts the indices entries
    sort(_indices.begin(), _indices.end());
}

//Lê as entradas no arquivo de índice
//Read the entries in the indice file
bool Indice::readIndices(){
    ifstream infile(_indexFile);
    if(!infile.good()){
        return false;
    }
    string line, val;
    unsigned i;
    unsigned number;
    int rrn;
    //Leitura e separação dos pares (número,rrn) - Read snd parsing of pairs (number, rrn)
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
        //Inserção no índice - Insertion in the indice
        _indices.insert(_indices.end(), make_pair(number, rrn));
    }
    //Ordena as entradas do índice - Sorts the indices entries
    sort(_indices.begin(), _indices.end());
    return true;
}

//Função que constrói o arquivo de índices a partir das entradas no índice
//Function that builds the indice file from the indice entries
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

//Insere uma entrada no índice e ordena
//Inserts an entry in the indice and sorts
void Indice::addReg(Register reg){
    _indices.insert(_indices.end(), make_pair(reg.getNumber(), reg.getRRN()));
    sort(_indices.begin(), _indices.end());
}

//Acha a posição de um número no vetor de pares (número, rrn)
//Finds the position of a number in the vector (number, rrn)
int Indice::getPos(unsigned number){
    //Checagem de cache - Chache check
    if((unsigned)_lastPos<_indices.size() && _lastPos != -1){
        if(_indices[_lastPos].first==number){
            return _lastPos;
        }
    }
    //Busca binária - Binary search
    int lower = 0;
    int higher = _indices.size()-1;
    int mid;
    while(lower<=higher && higher<(int)_indices.size()){
        mid=(lower + higher)/2;
        if(_indices[mid].first==number){
            return mid;
        }
        if(_indices[lower].first==number){
            return lower;
        }
        if(_indices[higher].first==number){
            return higher;
        }
        if(_indices[mid].first>number){
            higher=mid-1;
        }else if(_indices[mid].first<number){
            lower=mid+1;
        }
    }
    //Não encontrado - Not found
    return -1;
}

//Busca o rrn de um dado número
//Searches for the rrn of a given number
int Indice::getRRN(unsigned number){
    int pos = getPos(number);
    if(pos == -1){
        return -1;
    }else{
        return _indices[pos].second;
    }
}

//Remove um par (número,rrn) do índice a partir de um número
//Removes a pair (number,rrn) from the indice with the number
bool Indice::removeReg(unsigned number){
    int pos = getPos(number);
    if(pos == -1){
        return false;
    }else{
        _indices.erase(_indices.begin()+pos);
        return true;
    }
}

//Retorna a lista de pares (número,rrn)
//Returns the pair list of (number,rrn)
vector<pair<unsigned, int> > Indice::getNumberList(){
    return _indices;
}

//Limpa a lista de pares
//Clears the pair list
void Indice::clear(){
    _indices.clear();
}
