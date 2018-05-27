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
        return false;
    }
    getline(infile, line);
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
    for(;i<line.size() && line.at(i)!='|';i++){
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
        cout << "Cannot read register" << endl;
        return false;
    }
    char choice;
    char choice2;
    unsigned num = _activeReg.getNumber();
    string name = _activeReg.getName();
    string car = _activeReg.getCar();
    cout << "Registo encontrado: " << endl << "\e[1m" << _activeReg.getOutStr() << "\e[0m";
    cout << "Qual dos dados deseja alterar?" << endl << "\e[1mN\e[0m - Número" << endl << "\e[1mP\e[0m - Nome" << endl << "\e[1mC\e[0m - Carro" << endl << "Para sair digite qualquer outra tecla." << endl;
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
        getchar();
        getline(cin, name);
        break;
    case 'C':
    case 'c':
        cout << "Digite o novo carro para alteração: ";
        getchar();
        getline(cin, car);
        break;
    default:
        return false;
    }
    removeData(number);
    _activeReg.assignNumber(num);
    _activeReg.assignName(name);
    _activeReg.assignCar(car);
    addData(_activeReg);
    return true;
}

void dataBroker::findData(){
    string name;
    string car;
    unsigned num;
    int rrn;
    char choice;
    vector<pair<unsigned, int> > numbers;
    vector<Register> foundRegs;
    int pos;
    string upperStr;
    cout << "Deseja fazer uma busca por número, carro ou nome?"  << endl << "\e[1mN\e[0m - Número" << endl << "\e[1mP\e[0m - Nome" << endl << "\e[1mC\e[0m - Carro" << endl << "Para sair digite qualquer outra tecla." << endl;
    cin >> choice;
    switch(choice){
    case 'N':
    case 'n':
        cout << "Digite o número desejado: ";
        cin >> num;
        rrn = _indice->getRRN(num);
        if(rrn==-1){
            cout << "Erro: Número não encontrado." << endl;
        }else{
            readRegister(rrn);
            cout << "Registro encontrado: " << endl;
            cout << "\e[1m";
            cout << _activeReg.getOutStr();
            cout << "\e[0m";
        }
        break;
    case 'P':
    case 'p':
        numbers=_indice->getNumberList();
        cout << "Digite o nome desejado: ";
        getchar();
        getline(cin, name);
        for(unsigned i=0;i<numbers.size();i++){
            readRegister(numbers.at(i).second);
            if(_activeReg.getRRN()==-1){
                continue;
            }
            for(unsigned j=0;j!=name.size();j++){
                name.at(j)=toupper(name.at(j));
            }
            upperStr=_activeReg.getName();
            for(unsigned j=0;j!=upperStr.size();j++){
                upperStr.at(j)=toupper(upperStr.at(j));
            }
            pos=upperStr.find(name);
            if(pos==(int)string::npos){
                continue;
            }
            upperStr=_activeReg.getName();
            upperStr.insert(pos+name.size(), "\e[0m");
            upperStr.insert(pos, "\e[1m");
            _activeReg.assignName(upperStr);
            foundRegs.insert(foundRegs.end(), _activeReg);
            _activeReg.assignRRN(-1);
        }
        if(foundRegs.size()==0){
            cout << "Nenhum registro foi encontrado." << endl;
        }else if(foundRegs.size()==1){
            cout << "Registro encontrado: " << endl;
        }else{
            cout << "Registros encontrados: " << endl;
        }
        for(unsigned i=0;i<foundRegs.size();i++){
            cout << "\e[1m" << i+1 << ")\e[0m" << endl;
            cout << foundRegs.at(i).getOutStr();
        }
        break;
    case 'C':
    case 'c':
        numbers=_indice->getNumberList();
        cout << "Digite o carro desejado: ";
        getchar();
        getline(cin, car);
        for(unsigned i=0;i<numbers.size();i++){
            readRegister(numbers.at(i).second);
            if(_activeReg.getRRN()==-1){
                continue;
            }
            for(unsigned j=0;j!=car.size();j++){
                car.at(j)=toupper(car.at(j));
            }
            upperStr=_activeReg.getCar();
            for(unsigned j=0;j!=upperStr.size();j++){
                upperStr.at(j)=toupper(upperStr.at(j));
            }
            pos=upperStr.find(car);
            if(pos==(int)string::npos){
                continue;
            }
            upperStr=_activeReg.getCar();
            upperStr.insert(pos+car.size(), "\e[0m");
            upperStr.insert(pos, "\e[1m");
            _activeReg.assignCar(upperStr);
            foundRegs.insert(foundRegs.end(), _activeReg);
            _activeReg.assignRRN(-1);
        }
        if(foundRegs.size()==0){
            cout << "Nenhum registro foi encontrado." << endl;
        }else if(foundRegs.size()==1){
            cout << "Registro encontrado: " << endl;
        }else{
            cout << "Registros encontrados: " << endl;
        }
        for(unsigned i=0;i<foundRegs.size();i++){
            cout << "\e[1m" << i+1 << ")\e[0m" << endl;
            cout << foundRegs.at(i).getOutStr();
        }
        break;
    default:
        break;
    }

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

