class B1{
protected:
    int e;
public:
    B1();
    void setB1(int);
    virtual~B1();
    void B1show();
};

class B2{
protected:
    int q;
public:
    B2();
    void setB2(int);
    virtual ~B2();
    void B2show();
};

class D1:virtual private B1, virtual  public B2{
protected:
    int w;
public:
    D1(int,int,int);
    virtual ~D1();
    void D1show();
};

class D2:virtual public B1,virtual private B2{
protected:
    int r;
public:
    D2(int,int,int);
    virtual ~D2();
    void D2show(); 
};

class D3: private D1, public D2{
protected:
    int t;
public:
    D3(int,int,int,int,int);
    ~D3();
    void D3show();
};