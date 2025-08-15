#include <iostream>
#include <functional>
#include <vector>
#include <csignal>
#include <exception>
#include <map>

#include <type_traits>
#include <typeindex>
#include <typeinfo>

#include <any>
#include <random>
#include <iomanip>


class Test {
public:
    static int m_value; // 定义私有类的静态成员变量
    int m_value2 = 0;

public:
    Test() { m_value++; }
    virtual ~Test();

    static int getValue() { // 定义类的静态成员函数
        return m_value;
    }

    virtual int getValue2() {
        return m_value2;
    }
};
int Test::m_value = 0;

class Counter {
private:
    mutable int count = 0;

public:
    void increment() const {
        ++count;  // 可以在const函数中修改mutable成员
    }

    int getCount() const {
        return count;
    }
};


void test2()
{
    char* p = new(std::nothrow) char[100];
    if (p == nullptr) {
        std::cout << "Failed!\n";
    }
    else {
        std::cout << "Succeeded!\n";
        delete[] p;
    }
}



class MyException : public std::exception {
public:
    const char* what() const throw() {
        return "Custom Exception Occurred!";
    }
};

int test3() {
    try
    {
        std::vector<int> v(5);
        v.at(10) = 100; // 越界访问
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << e.what() << std::endl;
    }

    try
    {
        throw MyException();
    }
    catch (MyException& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

template <class Arg, class F>
auto operator|(Arg&& arg, F&& f) -> decltype(f(std::forward<Arg>(arg)))
{
    return f(std::forward<Arg>(arg));
}

using std::cout;
using std::endl;

void test4()
{
    std::map<char, std::function<double(double, double)> > host;
    host['+'] = [](double a, double b)->double { return a + b; };
    host['-'] = [](double a, double b)->double { return a - b; };
    host['*'] = [](double a, double b)->double { return a * b; };
    host['/'] = [](double a, double b)->double { return a / b; };
    host['^'] = [](double a, double b)->double { return pow(a, b); };
    cout << "1.1 + 2.2= " << host['+'](1.1, 2.2) << endl; // => 3.3
    cout << "1.1 - 2.2= " << host['-'](1.1, 2.2) << endl; // => -1.1
    cout << "1.1 * 2.2= " << host['*'](1.1, 2.2) << endl; // => 2.42
    cout << "1.1 / 2.2= " << host['/'](1.1, 2.2) << endl; // => 0.5
    cout << "1.1 ^ 2.2= " << host['^'](1.1, 2.2) << endl; // => 1.23329
}

constexpr int factorial(int n) {
    if (n <= 1) return 1;
    else return n * factorial(n - 1);
}

template<typename T>
void func(T x) {
    if constexpr (std::is_integral_v<T>) std::cout << "Operations to int" << std::endl;
    else std::cout << "General operations" << std::endl;
}

template<class T, class F>
inline std::pair<const std::type_index, std::function<void(std::any const&)>>
to_any_visitor(F const& f)
{
    return {
        std::type_index(typeid(T)),
        [g = f](std::any const& a)
        {
            if constexpr (std::is_void_v<T>)
                g();
            else
                g(std::any_cast<T const&>(a));
        }
    };
}

//生成均匀分布的随机数
void test5()
{
    std::random_device rd; // 获取真随机数种子
    std::mt19937 gen(rd()); // 使用Mersenne Twister算法的随机数生成器
    std::uniform_int_distribution<> distrib(1, 6); // 定义均匀分布，范围为1到6

    for (int n = 0; n < 50; ++n) {
        std::cout << distrib(gen) << ' '; // 生成随机数并输出
    }
    std::cout << '\n';
}

//生成正态分布的随机数
void test6()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> distrib(0, 1); // 均值为0，标准差为1

    for (int n = 0; n < 50; ++n) {
        std::cout << std::fixed << std::setprecision(2) << distrib(gen) << ' ';
    }
    std::cout << '\n';
}

struct A {
    A() { std::cout << "A()\n"; };
    A(const A&){ std::cout << "A(const A&)\n"; };
    A(int) { std::cout << "A(int)\n"; }

    int data;
};

void test7()
{
    //A a();作为函数声明
    A a;
    A a1{}; // 列表初始化
    A a2(1);
    A* a3 = new A();
    A* a4 = new A(1);
    auto a5 = static_cast<A>(1);
}

int main()
{
    Test t1;
    t1.m_value = 6;
    Test::m_value = 7;
    Test::getValue();

    std::cout << "sizeof(Test):" << sizeof(Test) << std::endl; // 输出 4.0

    constexpr int result = factorial(5); // 在编译时计算结果
    std::cout << "result:" << result << std::endl; // 输出 4.0


    int result2 = 5 | [](int _) { return _ + 2; }; //

    func(1); //输出：Operations to int
    func(1.3); //输出：General operations

    test5();
    test6();

    test7();
}