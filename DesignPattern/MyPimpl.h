class MyInterface 
{
public:
    MyInterface();
    ~MyInterface();

    int publicApi1();
    int publicApi2(int i);

private:
    class Impl;
    Impl* impl;
};