#include "register.h"

Register::Register(){
    confReg(0,"","");
}
Register::Register(unsigned residenceNumber, string subjectName, string usedVehicle){
    confReg(residenceNumber, subjectName, usedVehicle);
}
void Register::confReg(unsigned residenceNumber, string subjectName, string usedVehicle, int rrn){
    _residenceNumber=residenceNumber;
    _subjectName=subjectName;
    _usedVehicle=usedVehicle;
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
