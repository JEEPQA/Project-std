#include <iostream>
#include <functional>
#include <vector>
#include <csignal>
#include <exception>
#include <type_traits>

// 普通函数
int add(int a, int b) {
    return a + b;
}

// Lambda 表达式
auto lambda = [](int a, int b) -> int {return a + b;};

// 仿函数
struct Functor {
    int operator()(int a, int b) {
        return a + b;
    }
};

void signal_handler(int signal)
{
    std::cout << "Received signal " << signal << '\n';
}

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_ + i << '\n'; }
    int num_;
};
void print_num(int i) {
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(int i) const {
        std::cout << i << '\n';
    }
};

void test2() {

    std::cout << "Is foo invocable with int? " << std::is_invocable_v<decltype(print_num), int> << std::endl;
    // 调用自由函数
    std::invoke(print_num, -9);

    // 调用lambda
    std::invoke([]() { print_num(42); });

    // 调用成员函数
    const Foo foo(314159);
    std::invoke(&Foo::print_add, foo, 1);

    // 调用（访问）数据成员
    std::cout << "num_: " << std::invoke(&Foo::num_, foo) << '\n';

    // 调用函数对象
    std::invoke(PrintNum(), 18);
}

int main() 
{
    // 封装普通函数
    std::function<int(int, int)> func = add;
    std::cout << "普通函数: " << func(1, 2) << std::endl;

    // 封装 Lambda 表达式
    func = lambda;
    std::cout << "Lambda 表达式: " << lambda(3, 4) << std::endl;
    std::cout << "Lambda 表达式: " << func(3, 4) << std::endl;

    auto add = [](auto a, auto b) {return a + b; };

    std::cout << add(1, 2) << std::endl; // 输出 3
    std::cout << add(1.5, 2.5) << std::endl; // 输出 4.0


    // 封装仿函数
    Functor functor;
    func = functor;
    std::cout << "仿函数: " << func(5, 6) << std::endl;

    // Install a signal handler
    std::signal(SIGTERM, signal_handler);

    std::cout << "Sending signal " << SIGTERM << '\n';
    std::raise(SIGTERM);

    return 0;
}