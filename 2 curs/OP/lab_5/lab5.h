class B1{
protected:
    int e;
public:
    B1(int);
    ~B1();
    void B1show();
};

class B2{
protected:
    int q;
public:
    B2(int);
    ~B2();
    void B2show();
};

class D1:private B1, public B2{
protected:
    int w;
public:
    D1(int,int,int);
    ~D1();
    void D1show();
    void B1s(){B1show();}
};

class D2:public B1,private B2{
protected:
    int r;
public:
    D2(int,int,int);
    ~D2();
    void D2show(); 
    void B2s(){B2show();}
};

class D3:private D1,public D2{
protected:
    int t;
public:
    D3(int,int,int,int,int);
    ~D3();
    void D3show();
    void D1s(){D1show();}
};