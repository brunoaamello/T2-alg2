#include <iostream>

using namespace std;

int main(){


    int action;
    do{
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
