#include "databroker.h"

dataBroker::dataBroker(){
    _indice=new Indice();
    ifstream file("dados.txt");
    if(!file.good()){
        cout << "Criando arquivo e lendo entrada inicial." << endl;
        ofstream{ "dados.txt" };
        cout << ".";
        if(readInitial()){
            cout << "." << endl << "Fim da leitura." << endl;
        }else{
            char option;
            cout << endl << "Erro na leitura do arquivo de início. Deseja continuar a execução mesmo assim(s/n)?" << endl;
            cin >> option;
            if(option!='s'||option!='S'){
                cout << "Encerrando o programa." << endl;
                exit(-1);
            }
        }
    }else{
        cout << "Inicialização completa com sucesso." << endl;
    }
}

void dataBroker::removeFromData(int rrn){
    ofstream outfile("dados.txt");
    outfile.seekp(rrn+1);
    outfile.put('$');
    outfile.close();
}

bool dataBroker::readInitial(){
    return false;
}

bool dataBroker::addData(Register reg){
    int rrn = _indice->getRRN(reg.getNumber());
    if(rrn!=-1){
        return false;
    }
    _activeReg=reg;
    ofstream outfile("dados.txt");
    outfile.seekp(ios::end);
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
    ifstream infile("dados.txt");
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
        _activeReg.assignNumber(num);
        //ADICIONAR ROTINA PARA CHECAGEM DE USO DO NUMERO
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

