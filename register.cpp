#include "register.h"

Register::Register(){
    confReg(0,"","");
}
Register::Register(unsigned residenceNumber, string subjectName, string usedVehicle){
    confReg(residenceNumber, subjectName, usedVehicle);
}
//Configuração do registro
//Register comfiguration
void Register::confReg(unsigned residenceNumber, string subjectName, string usedVehicle, int rrn){
    setNumber(residenceNumber);
    setName(subjectName);
    setCar(usedVehicle);
    setRRN(rrn);
}

//Registro em estrutura para armazenamento no arquivo de dados
//Register formatted for data file storage
string Register::toStrReg(){
    string out;
    out += "#";
    out += to_string(getNumber());
    out += "|";
    out += getName();
    out += "|";
    out += getCar();
    out += "|";
    return out;
}

//Registro em forma para exibição ao usuário
//Register formatted for user view
string Register::getOutStr(){
    string out;
    out += "Nome: ";
    out += getName();
    out += "\nNúmero: ";
    out += to_string(getNumber());
    out += "\nCarro: ";
    out += getCar();
    out += "\n";
    return out;
}
