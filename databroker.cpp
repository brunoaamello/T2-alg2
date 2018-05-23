#include "databroker.h"

dataBroker::dataBroker(string dataFile){
    _dataFile=dataFile;
    _indice=new Indice(_dataFile);
    ifstream file(_dataFile);
    if(!file.good()){
        cout << "Criando arquivo e lendo entrada inicial.";
        ofstream{ _dataFile };
        cout << ".";
        if(readInitial()){
            cout << "." << endl << "Fim da leitura." << endl;
        }else{
            char option;
            cout << endl << "Erro na leitura do arquivo de início. Deseja continuar a execução mesmo assim(s/n)?" << endl;
            cin >> option;
            if(option!='s'&&option!='S'){
                cout << "Encerrando o programa." << endl;
                exit(-1);
            }
        }
    }else{
        cout << "Inicialização completa com sucesso." << endl;
    }
}

dataBroker::~dataBroker(){
    delete _indice;
}

void dataBroker::removeFromData(int rrn){
    FILE* outfile = fopen(_dataFile.c_str(), "r+");
    fseek(outfile, rrn+1, SEEK_SET);
    fputc('$', outfile);
    fclose(outfile);
}

bool dataBroker::readInitial(){
    ifstream infile("TabelaInicial.txt");
    if(!infile.good()){
        return false;
    }
    Register reg;
    string line;
    string name;
    string car;
    string val;
    unsigned number;
    int size, count;
    unsigned i;
    getline(infile, line);
    for(i=1;i<line.size();i++){
        if(line.at(i-1)=='=' && line.at(i)==' '){
            i++;
            break;
        }
    }
    for(;i<line.size();i++){
        val+=line.at(i);
    }
    size = stoi(val);
    getline(infile, line);
    getline(infile, line);
    getline(infile, line);
    count=0;
    while(count < size){
        val.clear();
        name.clear();
        car.clear();
        for(i=1;i<line.size();i++){
            if(line.at(i)=='|' && val.size()>0){
                i++;
                break;
            }else if(line.at(i)!=' '){
                val+=line.at(i);
            }
        }
        number=(unsigned int)stoul(val);
        for(;i<line.size();i++){
            if(line.at(i)=='|' && name.size()>0){
                i++;
                break;
            }else if((line.at(i-1)>'A' && line.at(i-1)<'z' && line.at(i-1)!=' ' && line.at(i+1)!=' ')||(line.at(i)>'A' && line.at(i)<'z')){
                name+=line.at(i);
            }
        }
        for(;i<line.size()-1;i++){
            if(line.at(i)=='|' && car.size()>0){
                i++;
                break;
            }else if((line.at(i-1)>'A' && line.at(i-1)<'z' && line.at(i-1)!=' ' && line.at(i+1)!=' ')||(line.at(i)>'A' && line.at(i)<'z')){
                car+=line.at(i);
            }
        }
        reg.confReg(number, name, car);
        if(!addData(reg)){
            cout << "Erro na escrita do registro." << endl;
        }
        getline(infile, line);
        count++;
    }
    infile.close();
    return true;
}

bool dataBroker::addData(Register reg){
    int rrn = _indice->getRRN(reg.getNumber());
    if(rrn!=-1){
        return false;
    }
    _activeReg=reg;
    ofstream outfile(_dataFile, ios::app);
    _activeReg.assignRRN(outfile.tellp());
    outfile << _activeReg.toStrReg() << endl;
    outfile.close();
    _indice->addReg(_activeReg);
    return true;
}

bool dataBroker::removeData(unsigned number){
    if(number == _activeReg.getNumber() && _activeReg.getRRN() != -1){
        _indice->removeReg(number);
        removeFromData(_activeReg.getRRN());
        _activeReg.assignRRN(-1);
    }else if(_indice->getRRN(number)!=-1){
        readRegister(_indice->getRRN(number));
        _indice->removeReg(number);
        removeFromData(_activeReg.getRRN());
        _activeReg.assignRRN(-1);
    }else{
        return false;
    }
    return true;
}

bool dataBroker::readRegister(int rrn){
    ifstream infile(_dataFile);
    string line;
    infile.seekg(rrn);
    if(rrn!=infile.tellg()){
        _activeReg.assignRRN(-1);
        cout << "telg not rrn" << endl;
        return false;
    }
    getline(infile, line);
    infile.close();
    if(line.at(0)!='#'){
        cout << "Erro #" << endl;
        return false;
    }
    unsigned i;
    unsigned number;
    string name;
    string car;
    for(i=1;line.at(i)!='|';i++){
        name+=line.at(i);
    }
    i++;
    number=(unsigned int)stoul(name);
    name.clear();
    for(;line.at(i)!='|';i++){
        name+=line.at(i);
    }
    i++;
    for(;i<line.size() && line.at(i)!='|';i++){
        car+=line.at(i);
    }
    _activeReg.confReg(number, name, car, rrn);
    return true;
}

bool dataBroker::changeData(unsigned number){
    int rrn = _indice->getRRN(number);
    if(rrn == -1){
        cout << "rrn -1" << endl;
        return false;
    }
    if(!readRegister(rrn)){
        cout << "Cannot read register" << endl;
        return false;
    }
    char choice;
    char choice2;
    unsigned num = _activeReg.getNumber();
    string name = _activeReg.getName();
    string car = _activeReg.getCar();
    cout << "Registo encontrado: " << endl << _activeReg.getOutStr();
    cout << "Qual dos dados deseja alterar?" << endl << "N - Número" << endl << "P - Nome" << endl << "C - Carro" << endl << "Para sair digite qualquer outra tecla." << endl;
    cin >> choice;
    switch(choice){
    case 'N':
    case 'n':
        cout << "Digite o novo número para alteração: ";
        cin >> num;
        while(_indice->getRRN(num)!=-1){
            cout << "Numero já utilizado, deseja digitar outro número?(s/n)" << endl;
            cin >> choice2;
            switch(choice2){
            case 'S':
            case 's':
                cout << "Digite o novo número: ";
                cin >> num;
                break;
            default:
                return true;
            }
        }
        break;
    case 'P':
    case 'p':
        cout << "Digite o novo nome para alteração: ";
        getline(cin, name);
        break;
    case 'C':
    case 'c':
        cout << "Digite o novo carro para alteração: ";
        getline(cin, car);
        break;
    default:
        return true;
    }
    removeData(number);
    _activeReg.assignNumber(num);
    _activeReg.assignName(name);
    _activeReg.assignCar(car);
    addData(_activeReg);
    return true;
}

void dataBroker::findData(unsigned number){

}

void dataBroker::shrinkData(){
    vector<pair<unsigned, int> > numbers = _indice->getNumberList();
    _indice->clear();
    ofstream outfile("dados.temp", ofstream::app);
    int rrn;
    for(auto it=numbers.begin();it!=numbers.end();it++){
        rrn=outfile.tellp();
        readRegister(it->second);
        outfile << _activeReg.toStrReg() << endl;
        _activeReg.assignRRN(rrn);
        _indice->addReg(_activeReg);
    }
    outfile.close();
    remove(_dataFile.c_str());
    rename("dados.temp", _dataFile.c_str());
}

