#include <iostream>
using namespace std;

class Animal {
public:
    Animal() { cout << "Animal()" << endl; };
    virtual ~Animal() { cout << "delete Animal" << endl; };
    virtual void speak() {
        cout << "动物在说话" << endl;
    }
    //virtual void speak2() {
    //    cout << "动物在说话2" << endl;
    //}
    int value = 0;
};

class Cat : public Animal {
public:
    void speak() override {
        cout << "小猫在说话" << endl;
    }
    ~Cat() { cout << "delete Cat" << endl; };
    int value1 = 1;
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "小狗在说话" << endl;
    }
    virtual void fun2()  {
        cout << "Dog fun2" << endl;
    }
    ~Dog() { cout << "delete Dog" << endl; };
    int value2 = 2;
};

class Dog2 : public Dog {
public:
    void speak() override {
        cout << "Dog2在说话" << endl;
    }
    virtual void fun2() {
        cout << "Dog2 fun2" << endl;
    }
    ~Dog2() { cout << "delete Dog2" << endl; };
    int value2 = 2;
};

void DoSpeak(Animal* X) {
    X->speak();
}

void DoSpeak(Animal& X) {
    X.speak();
}

template <typename T>
void DoSpeak(T X) {
    //X.speak();
}

void test1()
{
    Dog B;
    Dog2 B1;
    B1.fun2();
    Animal* A = &B;
    //A->speak(); 
    DoSpeak(&B);
    cout << "sizeof(Animal) " << sizeof(Animal) << endl;
    cout << "sizeof(*A) " << sizeof(*A) << endl;
    cout << "sizeof(B) " << sizeof(B) << endl;
    cout << "A->value " << A->value << endl;
    cout << "B.value2 " << B.value2 << endl;
    cout << "B.value2 " << B.value << endl;

    Animal* A2 = new Cat;
    auto A22 = new Cat;
    //A2->speak(); // 
    DoSpeak(A2);
    DoSpeak(A22);

    Cat A3;
    DoSpeak(A3); // 
}


class IBase
{
public:
    IBase() { cout << "IBase()" << endl; };
    virtual ~IBase() { cout << "delete IBase" << endl; };
    virtual void DoSomething() = 0;
};

class ClxBase : public IBase  
{
public:
    //ClxBase() : value(1) {  };
    ClxBase(){ cout << "ClxBase()" << endl; };
    ClxBase(int value) { this->value = value; };
    virtual ~ClxBase() { cout << "delete ClxBase" << endl; };
    virtual void DoSomething() { cout << "Do something in class ClxBase!" << endl; };
    static void statFunc(){cout << "statFunc" << endl;}
    void statFunc2() { cout << "statFunc2" << endl; }

public:
    int value = 2;
    Animal animal;
};

class ClxDerived : public ClxBase {
public:
    ClxDerived() :ClxBase(){ cout << "ClxDerived()" << endl; };
    ~ClxDerived() { cout << "delete ClxDerived" << endl; };
    void DoSomething() { cout << "Do something in class ClxDerived!" << endl; };
};

void test2()
{
    ClxBase* pTest = new ClxDerived;
    pTest->DoSomething();
    cout << "pTest->value: " << pTest->value << endl;

    delete pTest;

    //cout << "&ClxBase::DoSomething: " << &ClxBase::DoSomething << endl;
    //printf("&ClxBase::DoSomething:%lld \n", &ClxBase::DoSomething);
    //printf("&ClxBase::statFunc:%lld \n", &ClxBase::statFunc);
    //printf("&ClxBase::statFunc2:%lld \n", &ClxBase::statFunc2);
    //auto p1 = &ClxBase::DoSomething;

}

class Base {
public:
    virtual void function() 
    {
        std::cout << "Base function" << std::endl;
    }
    virtual void function2()
    {
        std::cout << "Base function2" << std::endl;
    }
};

class Derived : public Base {
public:
    virtual void function() override
    {
        Base::function();
        //Base::function2();
        std::cout << "Derived function" << std::endl;
    }
};
 
void test3() {
    Derived obj;
    obj.function();  // 调用子类的 Init

}

int main() {

    //test1();
    //test2();
    test3();

    return 0;
}