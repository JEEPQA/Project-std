#include <map>
#include <string>
#include <exception>
#include <iostream>
#include <shared_mutex>
#include <vector>
#include <iomanip>

using namespace std;


/*
1.auto的改进:返回值类型推导
2.变量模板
3.constexpr的改进
4.[[deprecated]]标记
5.二进制字面量与整形字面量分隔符
6.std::make_unique
7.std::shared_timed_mutex与std::shared_lock
8.std::integer_sequence
9.std::exchange
10.std::quoted
*/


auto func(int i) {return i;}
template<typename T> auto func2(T t) { return t; }
auto func3 = [](auto a) { return a; };
auto add = [](auto a, auto b) {return a + b;};
void test1()
{
	cout << func(4) << endl;
	cout << func2(3.4) << endl;
	cout << func3(2.3f) << endl;
	cout << func3(1) << endl;
	cout << add(1,2) << endl;
}


template<class T>
constexpr T pi = T(3.1415926535897932385L);
void test2()
{
	cout << pi<int> << endl; // 3
	cout << pi<double> << endl; // 3.14159

	int a = 0b0001'0011'1010;
	double b = 3.14'1234'1234'1234;
	cout << a << endl;
	cout << b << endl;
}

constexpr int factorial(int n) { // C++11中不可，C++14中可以
	int ret = 0;
	for (int i = 0; i < n; ++i) {
		ret += i;
	}
	return ret;
}
constexpr int factorial2(bool flag) { // C++11中不可，C++14中可以
	if (flag) return 1;
	else return 0;
}

//保证多个线程可以同时读，但是写线程必须独立运行，写操作不可以同时和读操作一起进行
struct ThreadSafe {
	mutable std::shared_timed_mutex mutex_;
	int value_;

	ThreadSafe() {
		value_ = 0;
	}

	int get() const {
		std::shared_lock<std::shared_timed_mutex> loc(mutex_);
		return value_;
	}

	void increase() {
		std::unique_lock<std::shared_timed_mutex> lock(mutex_);
		value_ += 1;
	}
};

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
	std::cout << "The sequence of size " << int_seq.size() << endl;
	//((std::cout << ints << ' '), ...);
	//std::cout << '\n';
}
void test3()
{
	print_sequence(std::integer_sequence<int, 9, 2, 5, 1, 9, 1, 6>{});
}


void test4()
{
	int a = 1;
	int b = 5;
	a = std::exchange(a, 2); // a 的值被替换为 2，返回的是 a 原来的值 1
	std::swap(a, b); 

	std::vector<int> v;
	std::exchange(v, { 1,2,3,4 });

	cout << v.size() << endl;
	for (const int& a : v) {
		cout << a << " ";
	}
}


void test5()
{
	string str = "hello world";
	cout << str << endl;
	cout << std::quoted(str) << endl;

	std::u16string s = u"Hello, C++14!";
	//std::u16string s = L"Hello, C++14!";
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();
	test4();
	test5();

	return 0;
}