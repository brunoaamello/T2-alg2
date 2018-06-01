/*  Trabalho 2 - Algoritmos e Estruturas de Dados II
 *  Bruno Arantes de Achilles Mello
 *  9866490
 */

#ifndef REGISTER_H
#define REGISTER_H
#include <string>

using namespace std;

class Register{
private:
    int _rrn;                   //Posição no arquivo de dados - Position in data file
    unsigned _residenceNumber;  //Número da residência - Residence number
    string _subjectName;        //Nome da pessoa - Person name
    string _usedVehicle;        //Carro usado - Used Car
public:
    Register();
    Register(unsigned residenceNumber, string subjectName, string usedVehicle);
    void confReg(unsigned residenceNumber, string subjectName, string usedVehicle, int rrn = -1);
    string toStrReg();
    string getOutStr();
    //Setters & Getters
    void setNumber(unsigned number){this->_residenceNumber=number;}
    void setName(string name){this->_subjectName=name;}
    void setCar(string car){this->_usedVehicle=car;}
    void setRRN(int rrn){this->_rrn=rrn;}
    int getRRN(){return _rrn;}
    unsigned getNumber(){return _residenceNumber;}
    string getName(){return _subjectName;}
    string getCar(){return _usedVehicle;}
};

#endif // REGISTER_H
