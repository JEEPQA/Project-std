#include "MyPimpl.h"
#include <string>
#include <list>
#include <stdio.h>

//
//Impl
//
class MyInterface::Impl
{
public:
    Impl() = default;
    ~Impl() = default;

    int publicApi1();
    int publicApi2(int i);
private:
    int privateMethod1();
    int privateMethod2();
    int privateMethod3();

    std::string name;
    std::list<int> list;

};

int MyInterface::Impl::publicApi1() {
    // ...
    privateMethod1();
    return 0;
}

int MyInterface::Impl::publicApi2(int i)
{
    // ...
    privateMethod2();
    privateMethod3();
    return 0;
}

int MyInterface::Impl::privateMethod1()
{
    printf("Impl::privateMethod1 called\n");
    return 0;
}
int MyInterface::Impl::privateMethod2()
{
    printf("Impl::privateMethod2 called\n");
    return 0;
}
int MyInterface::Impl::privateMethod3()
{
    printf("Impl::privateMethod3 called\n");
    return 0;
}


//
//MyInterface
//
MyInterface::MyInterface(): impl(new Impl()) 
{

}

MyInterface::~MyInterface() {
    delete impl;
}

int MyInterface::publicApi1() 
{
    return impl->publicApi1();
}

int MyInterface::publicApi2(int i) 
{
    return impl->publicApi2(i);
}

