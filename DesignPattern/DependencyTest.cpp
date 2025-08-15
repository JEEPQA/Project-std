#include <stdio.h>

class Dependency 
{
public:
    Dependency() { printf("Dependency 构造\n"); };
    Dependency(const Dependency& s) { printf("Dependency 拷贝构造\n"); };
    Dependency(Dependency&& s) { printf("Dependency 移动构造\n"); };

    Dependency& operator=(const Dependency& s) { printf("Dependency 重载赋值运算\n"); };

    ~Dependency() { printf("Dependency 析构\n"); };

    void doSomething() {
        // 实现 
        a += 1;
        printf("doSomething %d\n",a);
    }

private:
    int a=0;
};


class MyClass 
{
private:
    enum TokenType {
        tokenEndOfStream = 0,
        tokenObjectBegin,
        tokenObjectEnd,
        tokenArrayBegin,
        tokenArrayEnd,
        tokenString,
        tokenNumber,
        tokenTrue,
        tokenFalse,
        tokenNull,
        tokenArraySeparator,
        tokenMemberSeparator,
        tokenComment,
        tokenError
    };

    Dependency* dep;
    Dependency dep1;
    //Dependency dep2;
public:
    MyClass(){ printf("MyClass 构造1\n"); }
    MyClass(Dependency* dep) : dep(dep) { printf("MyClass 构造2\n"); }
    MyClass(Dependency dep) : dep1(dep) { printf("MyClass 构造3\n"); }
    ~MyClass() { printf("MyClass 析构\n"); }

    void setDependency(Dependency* dep) {
        this->dep = dep;
    }
    void someMethod() {
        dep->doSomething();
    }
    void someMethod1() {
        printf("someMethod1\n");
    }
    void someMethod2() {
        dep1.doSomething();
    }

};


class MyClass2 
{
private:
    Dependency* dep = nullptr;

public:
    void setDependency(Dependency* dep) {
        this->dep = dep;
    }

    void useDependency() {
        if (dep) {
            dep->doSomething();
        }
    }
};


//接口注入

class IDependency {
public:
    virtual void doSomething() = 0;
    virtual ~IDependency() {}
};

class Dependency2 : public IDependency 
{
public:
    void doSomething() override {
        // 实现细节  
    }
};


class MyClass3 
{
private:
    IDependency* dep;

public:
    MyClass3(IDependency* dep) : dep(dep) {}

    void useDependency() {
        dep->doSomething();
    }
};

//Dependency dep1;
//MyClass myClass1(&dep1);

int DependencyTest()
{
    //Dependency *dep = new Dependency;
    //MyClass myClass(dep);
    //myClass.someMethod();
    //delete dep;
    //dep = nullptr;
    
    //MyClass myClass;
    //myClass.someMethod1();

    Dependency dep;
    MyClass myClass1(dep);
    myClass1.someMethod2();

    //myClass.~MyClass();


    //MyClass2 myClass2;
    //myClass2.setDependency(dep);
    //myClass2.useDependency();


    //Dependency2* dep2 = new Dependency2;
    //MyClass3 myClass3(dep2);
    //myClass3.useDependency();

    return 0;
}

