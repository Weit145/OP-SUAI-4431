    #include<iostream>
    #include <string>

    using namespace std;

class Error: public exception{
    private:
        string message_;
    public:
        Error(const string& message): message_(message){}
        const char* what()const noexcept override{
            return message_.c_str();
        }
    };
    
class Arr{
    private:
        int* arr_;
        int size_;
    public:
        Arr(int* arr, int size);

        int sum_unsined();
        int mod_proz();

        void show();
};

    