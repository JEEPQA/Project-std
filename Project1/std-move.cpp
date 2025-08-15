#include <iostream>
#include <vector>


using namespace std;


class Test {
    int* arr{ nullptr };
public:
    Test() :arr(new int[5000] {1, 2, 3, 4}) 
    {
        cout << "default constructor" << endl;
    }
    Test(const Test& t) 
    {
        cout << "copy constructor" << endl;
        if (arr == nullptr) arr = new int[5000];
        memcpy(arr, t.arr, 5000 * sizeof(int));
    }
    Test(Test&& t) : arr(t.arr) 
    {
        cout << "move constructor" << endl;
        t.arr = nullptr;
    }
    ~Test() 
    {
        cout << "destructor" << endl;
        delete[] arr;
    }
};

class Teacher
{
private:
    std::string name;
    std::string position;

public:
    Teacher(const std::string& n, const std::string& p)
        : name(n), position(p) {
    }

    Teacher(std::string&& n, std::string&& p)
        : name(std::move(n)), position(std::move(p)) {
    };
};

class Teacher2
{
private:
    std::string name;
    std::string position;

public:
    template <typename S1, typename S2>
    Teacher2(S1&& n, S2&& p)
        : name(std::forward<S1>(n)), position(std::forward<S2>(p)) {
    };
};

void test_right_reference3()
{
    auto f_right_ref = [](int&& a) -> void {
        cout << "f_right_ref():: &a = " << &a << ", a = " << a << endl;
        };
    auto f_left_ref = [](int& a) -> void {
        cout << "f_right_ref():: &a = " << &a << ", a = " << a << endl;
        };
    int a = 10;
    cout << "test_right_reference3():: &a = " << &a << ", a = " << a << endl;

    f_left_ref(a);
    //    f_right_ref(a);         // 报错
    //    f_left_ref(1);          // 报错
    f_right_ref(1);
}


class A {
public:
    A(int n, int m) : x(n), y(m) {}
    int getX()& { return x; }   //该方法只能被左值对象调用
    int getY()&& { return y; }  //该方法只能被右值对象调用
private:
    int x;
    int y;
};

void test() {
    A a(100, 200);
    std::cout << a.getX() << std::endl;
    // std::cout << std::move(a).getX() << std::endl; //错误，getX只能左值对象调
    // std::cout << a.getY() << std::endl;            //错误，getY只能右值对象调
    std::cout << std::move(a).getY() << std::endl;
}

template <class T>
void swap(T& a, T& b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

void func(Test&& data) {
    // 函数体
}

//完美转发
template <typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));
}

template<typename T>
void wrapper2(T&& param)
{
    auto&& local_var = std::forward<T>(param); //使用完美转发
}

int main(int argc, char** argv) {

    std::vector<std::string> vec;
    std::string x = "abcd";       //将左值或右值强制转换为右值引用
    vec.push_back(std::move(x));
    std::cout << "x: " << x << "\n";
    std::cout << "vector: " << vec[0] << "\n";

    test_right_reference3();

    Test data;
    Test&& rvalue_ref = std::move(data);
    func(std::forward<Test>(rvalue_ref)); // OK

    return 0;
}