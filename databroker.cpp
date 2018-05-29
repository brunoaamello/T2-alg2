#include "databroker.h"

dataBroker::dataBroker(string dataFile){
    _dataFile=dataFile;
    _indice=new Indice(_dataFile);  //Inicialização do índice - Indice initialization
    ifstream file(_dataFile);
    if(!file.good()){
        //Caso não tenha um arquivo de dados lê a entrada inicial - In case there is no data file, reads the initial input
        cout << "Criando arquivo e lendo entrada inicial.";
        ofstream{ _dataFile };
        cout << ".";
        if(readInitial()){          //Lê o arquivo de entrada inicial - Reads the initial input file
            cout << "." << endl << "Fim da leitura." << endl;
        }else{
            //Caso não exista um arquivo de entrada inicial pede ao usuário o que fazer - If there is no initial input file asks for the user what to do next
            char option;
            cout << endl << "Erro na leitura do arquivo de início. Deseja continuar a execução mesmo assim(s/n)?" << endl;
            cin >> option;
            if(option!='s'&&option!='S'){
                cout << "Encerrando o programa." << endl;
                exit(-1);
            }
        }
    }else{
        //Caso já tenha um arquivo de dados a inicialização é concluída - If there is already a data file the initialization is concluded
        cout << "Inicialização completa com sucesso." << endl;
    }
}

dataBroker::~dataBroker(){
    delete _indice;
}

//Remove, de forma incondicional, o registro especificado do arquivo de dados por meio da invalidação do registro usando um '$' no primeiro caracter do campo de número
//Removes, unconditinally, the specified register from the data file by invalidating it with a '$' in the first character of the number field
void dataBroker::removeFromData(int rrn){
    FILE* outfile = fopen(_dataFile.c_str(), "r+");
    fseek(outfile, rrn+1, SEEK_SET);
    fputc('$', outfile);
    fclose(outfile);
}

//Lê o arquivo de entrada inicial
//Reads the initial input file
bool dataBroker::readInitial(){
    ifstream infile("TabelaInicial.txt");
    if(!infile.good()){
        //Se não houver arquivo de dados inicial - If there is no initial input file
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
    //Aquisição e separação do número de linhas de entrada: - Aquisition and parsing of number of input lines:
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
    //Descarte de linhas inúteis: - Purging of useless lines:
    getline(infile, line);
    getline(infile, line);
    getline(infile, line);
    count=0;
    while(count < size){
        val.clear();
        name.clear();
        car.clear();
        //Separação do número - Number parsing
        for(i=1;i<line.size();i++){
            if(line.at(i)=='|' && val.size()>0){
                i++;
                break;
            }else if(line.at(i)!=' '){
                val+=line.at(i);
            }
        }
        number=(unsigned int)stoul(val);
        //Separação do nome - Name parsing
        for(;i<line.size();i++){
            if(line.at(i)=='|' && name.size()>0){
                i++;
                break;
            }else if((line.at(i-1)>'A' && line.at(i-1)<'z' && line.at(i-1)!=' ' && line.at(i+1)!=' ')||(line.at(i)>'A' && line.at(i)<'z')){
                name+=line.at(i);
            }
        }
        //Separação do carro - Car parsing
        for(;i<line.size()-1;i++){
            if(line.at(i)=='|' && car.size()>0){
                i++;
                break;
            }else if((line.at(i-1)>'A' && line.at(i-1)<'z' && line.at(i-1)!=' ' && line.at(i+1)!=' ')||(line.at(i)>'A' && line.at(i)<'z')){
                car+=line.at(i);
            }
        }
        reg.confReg(number, name, car);
        //Adição do registro gerado ao arquivo de dados - Add generated register to data file
        if(!addData(reg)){
            cout << "Erro na escrita do registro." << endl;
        }
        getline(infile, line);
        count++;
    }
    infile.close();
    return true;
}

//Adiciona o registro ao arquivo de dados
//Adds the register to data file
bool dataBroker::addData(Register reg){
    int rrn = _indice->getRRN(reg.getNumber());
    if(rrn!=-1){
        //Caso o número já esteja em uso retorna erro - In case the number is already taken returns error
        return false;
    }
    _activeReg=reg;
    ofstream outfile(_dataFile, ios::app);
    _activeReg.setRRN(outfile.tellp());      //Atribuição de RRN - RRN atribution
    outfile << _activeReg.toStrReg() << endl;   //Escrita no arquivo - File writing
    outfile.close();
    _indice->addReg(_activeReg);                //Adição ao índice - Indice addition
    return true;
}

//Remove o registro do arquivo e do índice
//Removes the register from data file and indice
bool dataBroker::removeData(unsigned number){
    if(number == _activeReg.getNumber() && _activeReg.getRRN() != -1){
        //Caso o registro procurado esteja em cache e o cache for válido - If the searched number is cached and the cache is valid
        _indice->removeReg(number);
        removeFromData(_activeReg.getRRN());
        _activeReg.setRRN(-1);
    }else if(_indice->getRRN(number)!=-1){
        //Caso o registro seja exista no índice - If the register is present in the indice
        readRegister(_indice->getRRN(number));
        _indice->removeReg(number);
        removeFromData(_activeReg.getRRN());
        _activeReg.setRRN(-1);
    }else{
        //Caso o registro não seja encontrado - If the register is not found
        return false;
    }
    return true;
}

//Lê o registro em uma dada posição do arquivo de dados
//Read the register in a given position of the data file
bool dataBroker::readRegister(int rrn){
    //Abertura - Open
    ifstream infile(_dataFile);
    string line;
    infile.seekg(rrn);
    if(rrn!=infile.tellg()){
        _activeReg.setRRN(-1);
        return false;
    }
    //Leitura - Read
    getline(infile, line);
    infile.close();
    //Separação - Parsing
    if(line.at(0)!='#'){
        return false;
    }
    unsigned i;
    unsigned number;
    string name;
    string car;
    for(i=1;line.at(i)!='|';i++){
        if(line.at(i)=='$'){
            return false;
        }
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
    //Informação é armazenada no cache - Info is stored in cache
    _activeReg.confReg(number, name, car, rrn);
    return true;
}

//Alteração de um dos campos de um dado registrador
//Alteration of one of the fields in a register
bool dataBroker::changeData(unsigned number){
    //Leitura e validação - Read and validate
    int rrn = _indice->getRRN(number);
    if(rrn == -1){
        return false;
    }
    if(!readRegister(rrn)){
        return false;
    }
    char choice;
    char choice2;
    unsigned num = _activeReg.getNumber();
    string name = _activeReg.getName();
    string car = _activeReg.getCar();
    //Pergunta de campo - Field prompt
    cout << "Registo encontrado: " << endl << "\e[1m" << _activeReg.getOutStr() << "\e[0m";
    cout << "Qual dos dados deseja alterar?" << endl << "\e[1mN\e[0m - Número" << endl << "\e[1mP\e[0m - Nome" << endl << "\e[1mC\e[0m - Carro" << endl << "Para sair digite qualquer outra tecla." << endl;
    cin >> choice;
    switch(choice){
    case 'N':
    case 'n':
        //Número - Number
        cout << "Digite o novo número para alteração: ";
        cin >> num;
        while(_indice->getRRN(num)!=-1){
            //Número já utilizado - Taken number
            cout << "Numero já utilizado, deseja digitar outro número?(s/n)" << endl;
            cin >> choice2;
            switch(choice2){
            case 'S':
            case 's':
                cout << "Digite o novo número: ";
                cin >> num;
                break;
            default:
                return false;
            }
        }
        break;
    case 'P':
    case 'p':
        //Nome - Name
        cout << "Digite o novo nome para alteração: ";
        getchar();
        getline(cin, name);
        break;
    case 'C':
    case 'c':
        //Carro - Car
        cout << "Digite o novo carro para alteração: ";
        getchar();
        getline(cin, car);
        break;
    default:
        return false;
    }
    //Consolidação - Consolidation
    removeData(number);
    _activeReg.setNumber(num);
    _activeReg.setName(name);
    _activeReg.setCar(car);
    addData(_activeReg);
    return true;
}

//Busca de um registro a partir de um dos seus campos
//Search of a register by one of its fields
void dataBroker::findData(){
    string name;
    string car;
    unsigned num;
    int rrn;
    char choice;
    vector<pair<unsigned, int> > numbers;
    vector<Register> foundRegs;             //Lista de registros compatíveis - List of compatible registers
    int pos;
    string upperStr;
    //Pergunta de campo - Field prompt
    cout << "Deseja fazer uma busca por número, carro ou nome?"  << endl << "\e[1mN\e[0m - Número" << endl << "\e[1mP\e[0m - Nome" << endl << "\e[1mC\e[0m - Carro" << endl << "Para sair digite qualquer outra tecla." << endl;
    cin >> choice;
    switch(choice){
    case 'N':
    case 'n':
        //Número - Number
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
        //Nome - Name
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
            //Não sensível a maiúsculas/minúsculas - Case insensitive
            for(unsigned j=0;j!=upperStr.size();j++){
                upperStr.at(j)=toupper(upperStr.at(j));
            }
            pos=upperStr.find(name);
            if(pos==(int)string::npos){
                continue;
            }
            upperStr=_activeReg.getName();
            //Destaque de coincidência - Match highlighting
            upperStr.insert(pos+name.size(), "\e[0m");
            upperStr.insert(pos, "\e[1m");
            _activeReg.setName(upperStr);
            foundRegs.insert(foundRegs.end(), _activeReg);
            _activeReg.setRRN(-1);
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
        //Carro - Car
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
            //Não sensível a maiúsculas/minúsculas - Case insensitive
            for(unsigned j=0;j!=upperStr.size();j++){
                upperStr.at(j)=toupper(upperStr.at(j));
            }
            pos=upperStr.find(car);
            if(pos==(int)string::npos){
                continue;
            }
            upperStr=_activeReg.getCar();
            //Destaque de coincidência - Match highlighting
            upperStr.insert(pos+car.size(), "\e[0m");
            upperStr.insert(pos, "\e[1m");
            _activeReg.setCar(upperStr);
            foundRegs.insert(foundRegs.end(), _activeReg);
            _activeReg.setRRN(-1);
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

//Encolhimento de dados, elimina registros inválidos do arquivo de dados
//Data shrinking, removes invalid registers from data file
void dataBroker::shrinkData(){
    vector<pair<unsigned, int> > numbers = _indice->getNumberList();
    _indice->clear();
    ofstream outfile("dados.temp", ofstream::app);  //Arquivo temporário - Temp file
    int rrn;
    for(auto it=numbers.begin();it!=numbers.end();it++){
        rrn=outfile.tellp();
        readRegister(it->second);
        outfile << _activeReg.toStrReg() << endl;
        _activeReg.setRRN(rrn);
        _indice->addReg(_activeReg);
    }
    outfile.close();
    remove(_dataFile.c_str());
    rename("dados.temp", _dataFile.c_str());
}

