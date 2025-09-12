#include <string>

using namespace std;

class Worker{
private:
    string name;
    string surname;
    double rate;
    double days;
public:

    double getSalary(double,double);

    Worker(){
        name="";
        surname="";
        rate=0;
        days=0;
    }

    Worker(string name_new,string surname_new, double rate_new, double days_new){
        name=name_new;
        surname=surname_new;
        rate=rate_new;
        days=days_new;
    }

    Worker(const Worker& other){
        name=other.name;
        surname=other.surname;
        rate=other.rate;
        days=other.days;
    }


};

