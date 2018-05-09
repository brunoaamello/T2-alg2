#include "register.h"

Register::Register(){
    confReg(0,"","");
}
Register::Register(unsigned residenceNumber, string subjectName, string usedVehicle){
    confReg(residenceNumber, subjectName, usedVehicle);
}
void Register::confReg(unsigned residenceNumber, string subjectName, string usedVehicle, int rrn){
    assignNumber(residenceNumber);
    assignName(subjectName);
    assignCar(usedVehicle);
    assignRRN(rrn);
}
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
