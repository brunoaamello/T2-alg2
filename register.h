#ifndef REGISTER_H
#define REGISTER_H
#include <string>

using namespace std;

class Register{
private:
    int _rrn;
    unsigned _residenceNumber;
    string _subjectName;
    string _usedVehicle;
public:
    Register();
    Register(unsigned residenceNumber, string subjectName, string usedVehicle);
    void confReg(unsigned residenceNumber, string subjectName, string usedVehicle, int rrn = -1);
    string toStrReg();
    void assignRRN(int rrn){this->_rrn=rrn;}
    int getRRN(){return _rrn;}
    unsigned getNumber(){return _residenceNumber;}
    string getName(){return _subjectName;}
    string getCar(){return _usedVehicle;}
};

#endif // REGISTER_H
