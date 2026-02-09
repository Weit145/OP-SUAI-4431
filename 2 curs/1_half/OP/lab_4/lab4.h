class Money{
    protected:
        int all_sum=0;
};

class Bankomat:Money{
    public:
        bool input_sum(int);
        bool out_sum(int);
        void show_sum();
};