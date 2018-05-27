#include <iostream>
#include "databroker.h"
using namespace std;

int main(){
    dataBroker data;
    Register reg;
    unsigned number;
    string name, car;

    char action;
    do{
        fflush(stdin);
        cout << "Escolha a operação que deseja realizar:" << endl;
        cout << "\e[1m1) Inserir\e[0m" << endl;
        cout << "\e[1m2) Remover\e[0m" << endl;
        cout << "\e[1m3) Alterar\e[0m" << endl;
        cout << "\e[1m4) Procurar\e[0m" << endl;
        cout << "\e[1m5) Compactar\e[0m" << endl;
        cout << "\e[1m6) Sair\e[0m" << endl;
        cin >> action;
        switch(action){
        case '1':
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
        case '2':
            cout << "Digite o número da pessoa a ser removida: ";
            getchar();
            cin >> number;
            if(!data.removeData(number)){
                cout << "Número não encontrado." << endl;
            }else{
                cout << "Remoção efetuada com sucesso." << endl;
            }
            break;
        case '3':
            cout << "Digite o número da pessoa a ser alterada: ";
            getchar();
            cin >> number;
            if(!data.changeData(number)){
                cout << "Operação cancelada ou número não encontrado." << endl;
            }else{
                cout << "Alteração efetuada com sucesso." << endl;
            }
            break;
        case '4':
            data.findData();
            break;
        case '5':
            data.shrinkData();
            cout << "Compactação concluída com sucesso." << endl;
            break;
        case '6':
            break;
        default:
            cout << "\"" << action << "\" não é uma opção válida." << endl;
            break;
        }
    }while(action!='6');

    return 0;
}
