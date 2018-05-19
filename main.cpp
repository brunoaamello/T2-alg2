#include <iostream>
#include "databroker.h"
using namespace std;

int main(){
    dataBroker data;
    Register reg;
    unsigned number;
    string name, car;

    int action;
    do{
        fflush(stdin);
        cout << "Escolha a operação que deseja realizar:" << endl;
        cout << "1) Inserir" << endl;
        cout << "2) Remover" << endl;
        cout << "3) Alterar" << endl;
        cout << "4) Procurar" << endl;
        cout << "5) Compactar" << endl;
        cout << "6) Sair" << endl;
        cin >> action;
        switch(action){
        case 1:
            cout << "Insira o nome da pessoa: ";
            getchar();
            getline(cin, name);
            cout << "Insira o número da pessoa: ";
            cin >> number;
            cout << "Insira o veículo utilizado: ";
            getchar();
            getline(cin, car);
            reg.confReg(number, name, car);
            if(!data.addData(reg)){
                cout << "Erro na adição, número já utilizado." << endl;
            }else{
                cout << "Adição feita com sucesso." << endl;
            }
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        default:
            cout << "\"" << action << "\" não é uma opção válida." << endl;
            continue;
        }
    }while(action!=6);

    return 0;
}
