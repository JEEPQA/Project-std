#include <iostream>
using namespace std;


//下标[]
const int SIZE = 10;

class safearay {
private:
    int arr[SIZE];
public:
    safearay() {
        for (int i = 0; i < SIZE; i++) {
            arr[i] = i;
        }
    }

    int& operator[](int i) {
        if (i >= SIZE) {
            cout << "索引超过最大值" << endl;
            return arr[0];
        }
        return arr[i];
    }

    const int& operator[](int i) const {
        if (i >= SIZE) {
            cout << "索引超过最大值" << endl;
            return arr[0];
        }
        return arr[i];
    }
};

void test1()
{
    safearay A;
    cout << "A[2] 的值为: " << A[2] << endl;
    cout << "A[5] 的值为: " << A[5] << endl;
    cout << "A[12] 的值为: " << A[12] << endl;
}


//new运算符
void* operator new(size_t size) //重载全局operator new
{
    return malloc(size);
}
void* operator new[](size_t size)//重载全局operator new[]
{
    return malloc(size);
}
void operator delete(void* phead)//重载全局operator delete
{
    free(phead);
}
void operator delete[](void* phead)//重载全局operator delete[]
{
    free(phead);
}

class Mynew
{
public:
    Mynew() = default;
    ~Mynew() = default;

    void* operator new(size_t size)
    {
        return malloc(size);
    }
    void  operator delete(void* phead)
    {
        free(phead);
    }
    void* operator new[](size_t size)
    {
        return malloc(size);
    }
    void operator delete[](void* phead)
    {
        free(phead);
    }

    void* operator new(size_t size, void* phead)
    {
        //这里增加一些自己的额外代码，用于统计之类的，但不要分配内存
        return phead;//收到内存开始地址也只返回内存开始地址即可

    }
    void test()
    {
        printf("Mynew test");
    }
};

void test2()
{
    //Mynew* a = ::new Mynew();
    Mynew* a = new Mynew();
    a->test();
    delete a;
}

//
//指针运算符和成员访问运算符 取地址运算符
//
class Person
{
public:
    Person(int age)
    {
        this->m_Age = age;
    }
    ~Person()
    {
        cout << "Person的析构调用" << "(年龄为：" << this->m_Age << ")" << endl;
    }
    Person* operator&() {
        cout << "普通对象取地址操作符被调用" << endl;
        return this;
    }
    const Person* operator&() const {
        cout << "const对象取地址操作符被调用" << endl;
        return this;
    }


    void showAge()
    {
        cout << "年龄为：" << this->m_Age << endl;
    }

    int m_Age;
};
class smartPointer
{
public:
    smartPointer(Person* person)
    {
        this->person = person;
    }

    //重载->
    Person* operator->()
    {
        return this->person;
    }

    //重载 * 
    Person& operator*()
    {

        return *this->person;
    }

    ~smartPointer()
    {
        cout << "智能指针析构了" << endl;
        if (this->person != NULL)
        {
            delete this->person;
            this->person = NULL;
        }
    }

private:
    Person* person;
};

void test3()
{
    Person p1(30); 
    p1.showAge();

    Person* p2 = new Person(20);
    p2->showAge();
    delete p2;

    Person* p3 = &p1; 
    const Person* p4 = &p1;//

    smartPointer sp(new Person(10)); //sp开辟到了栈上，自动释放
    sp->showAge(); // 
    //(*sp).showAge();

}

//
//operator()
//
class Factorial {
public:
    int operator()(int n) const {
        int result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
};
void test4() 
{
    Factorial fact;
    std::cout << "5! = " << fact(5) << std::endl;  //  

    auto fact2 = std::make_shared<Factorial>();
    (*fact2)(6);  //

    // Lambda 表达式本质也是一种仿函数  
    auto lambda_func = [](int y) {std::cout << "Lambda called with: " << y << std::endl;};
    lambda_func(3);  //

}

//
// operator T()类型转换运算符重载函数
//
class Complex {
private:
    double real, imag;

public:
    Complex(double r, double i) : real(r), imag(i) {}

    // 类型转换运算符，将 Complex 转换为 double
    operator double() const {
        return real;  // 仅返回实部
    }
};
void test5()
{
    Complex c(3.0, 4.0);
    double x = c;  // 隐式调用 operator double()
    std::cout << "Complex 转换为 double: " << x << std::endl;
}

int main() {

    //test1();
    //test2();
    //test3();
    //test4();
    test5();

    return 0;
}