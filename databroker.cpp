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
    ofstream outfile(_dataFile);
    outfile.seekp(rrn+1);
    outfile.put('$');
    outfile.close();
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
        number=(unsigned long)stoul(val);
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
    if(rrn>=infile.tellg()){
        _activeReg.assignRRN(-1);
        return false;
    }
    string line;
    infile.seekg(rrn);
    infile >> line;
    infile.close();
    if(line.at(0)!='#'){
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
    for(;i<line.size();i++){
        car+=line.at(i);
    }
    _activeReg.confReg(number, name, car, rrn);
    return true;
}

bool dataBroker::changeData(unsigned number){
    int rrn = _indice->getRRN(number);
    if(rrn == -1){
        return false;
    }
    if(!readRegister(rrn)){
        return false;
    }
    char choice;
    char choice2;
    unsigned num;
    string name;
    string car;
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
        _activeReg.assignNumber(num);
        break;
    case 'P':
    case 'p':
        cout << "Digite o novo nome para alteração: ";
        cin >> name;
        _activeReg.assignName(name);
        break;
    case 'C':
    case 'c':
        cout << "Digite o novo carro para alteração: ";
        cin >> car;
        _activeReg.assignCar(car);
        break;
    default:
        return true;
    }
    removeData(number);
    addData(_activeReg);
    return true;
}

void dataBroker::findData(unsigned number){

}

void dataBroker::shrinkData(){

}

