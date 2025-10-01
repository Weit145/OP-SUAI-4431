#include <string>

using namespace std;

class Worker{
private:
    string name;
    string surname;
    double rate;
    double days;
public:

    void print();

    double getSalary(double,double);

    Worker(){
        name="Ivan";
        surname="Ivaniv";
        rate=10;
        days=20;
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

    ~Worker(){
        cout<<"Worker destroyed"<<endl;
    }

};

