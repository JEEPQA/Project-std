#include <map>
#include <string>
#include <exception>
#include <iostream>
#include <shared_mutex>
#include <vector>
#include <iomanip>
#include <charconv>
#include <variant>
#include <optional>
#include <any>
#include <filesystem>
#include <fstream>
#include <functional>
#include <numeric>
using namespace std;

/*
1.构造函数模板推导
  模板参数自动推导
2.结构化绑定 auto[]
3.if 和 switch 语句允许在条件表达式中进行变量初始化
4.内联变量
5.折叠表达式
6.constexpr lambda表达式
  if constexpr
7.namespace嵌套
8.__has_include预处理表达式
9.在lambda表达式用*this捕获对象副本
10.新增Attribute
11.<charconv>
   std::from_chars和std::to_chars
12.std::variant  
13.std::optional
14.std::any
15.std::apply std::make_from_tuple
17.std::string_view
18.std::as_const
19.<filesystem>
20.std::shared_mutex
21.std::invoke std::not_fn
22.<memory_resources>
23.std::void_t<>
24.std::size std::empty  std::data
25.u8字符串字面量
26.<execution>
27.std::byte
28.std::clamp std::reduce std::gcd std::lcm
*/

void test1()
{
	pair<int, double> p(1, 2.2); // before c++17
	auto p1 = std::make_pair(1, 2.2);
	pair p2(1, 2.2); // c++17 自动推导


	vector v2 = { 1, 2, 3 }; 
	int arr[] = { 4, 5, 6 };
	std::cout << "Vector size: " << std::size(v2) << std::endl;
	std::cout << "Array size: " << std::size(arr) << std::endl;

	std::cout << std::boolalpha;
	// 检查 vector 容器是否为空
	std::cout << "Vector is empty: " << std::empty(v2) << std::endl;
	// 检查数组是否为空
	std::cout << "Array is empty: " << std::empty(arr) << std::endl;

	// 输出 vector 容器底层数据的第一个元素
	std::cout << "Vector data: " << *std::data(v2) << std::endl;
	// 输出数组底层数据的第一个元素
	std::cout << "Array data: " << *std::data(arr) << std::endl;

	const char* utf8String = u8"This is a UTF-8 encoded string.";
	const char* multilingual = u8"日本語とEspañol";

	std::cout << "UTF-8 String: " << utf8String << std::endl;
	std::cout << "Multilingual: " << multilingual << std::endl;
}


std::tuple<int, double> tuplefunc() {
	return std::tuple(1, 2.2);
}

void f() {
	map<int, string> m = {
	  {0, "a"},
	  {1, "b"},
	};
	for (const auto& [i, s] : m) {
		cout << i << " " << s << endl;
	}
}

void test2()
{
	auto [i, d] = tuplefunc(); //是C++11的tie吗？更高级
	cout << i << endl;
	cout << d << endl;

	std::pair a(1, 2.3f);
	auto [i2, f2] = a;
	auto& [i3, f3] = a;
	cout << i2 << endl; // 1
	cout << f2 << endl; // 2.3f

	i2 = 2;
	cout << i2 << endl; // 1
	i3 = 66;
	cout << i3 << endl; // 1

	int array[3] = { 1, 2, 3 };
	auto [a1, b1, c1] = array;
	cout << a1 << " " << b1 << " " << c1 << endl;

	std::map<int, std::string> myMap = { {1, "one"}, {2, "two"}, {3, "three"} };
	for (const auto& [key, value] : myMap) {
		std::cout << "Key: " << key << ", Value: " << value << std::endl;
	}
}

class A {
public:
	inline static int m = 6; // 通过inline定义并初始化类的静态变量，可以在类内部直接初始化，无需外部定义。
};

inline A a;//在头文件中使用inline定义全局变量，避免了因头文件被多个源文件包含而导致的重复定义问题
constexpr static A a1;
const static A a2;


void test3()
{
	cout << "A::m :" << A::m << endl;

	a.m = 7;
	cout << "a.m :" << a.m << endl;

	a1.m = 8;
	cout << "a1.m :" << a1.m << endl;

	a2.m = 9;
	cout << "a2.m :" << a2.m << endl;
}

template <typename ... Args>
auto sum(Args ... args) {
	return (args + ...);
	//return (... + ts);
}

template<typename... Args>
auto multiply(Args... args) {
	//return (... * args); // 将所有参数相乘
	return (args * ...); // 将所有参数相乘
}

template<typename... Args>
bool all_true(Args... args) {
	return (args && ...); // 如果所有参数都为真，则返回true
}

template<typename... Args>
bool any_true(Args... args) {
	return (args || ...); // 如果任一参数为真，则返回true
}

template<typename... Args>
void call_all(Args&&... args) {
	(..., args()); // 调用每个参数的函数
}

template<typename... Args>
std::string concatenate(Args&&... args) {
	return (std::string{} + ... + std::forward<Args>(args));
}

template<typename T, typename... Args>
std::vector<T> make_vector(Args&&... args) {
	return { std::forward<Args>(args)... }; // 使用折叠构造初始化列表
}

template<typename... Mutexes>
void lock_all(Mutexes&... mutexes) {
	std::lock(mutexes...); // 同时锁定所有互斥量
}

template<typename... Resources>
void release_all(Resources&&... resources) {
	(..., resources.release()); // 释放所有资源
}

void test4()
{
	int s{ sum(1, 2, 3, 4, 7) }; //
	cout << s << endl; 

	std::string a{ "hello " };
	std::string b{ "world" };
	cout << sum(a, b) << endl; // hello world
}

//constexpr 函数体不能包含汇编语句、goto语句、label、try块、静态变量、线程局部存储、没有初始化的普通变量，不能动态分配内存，不能有new delete等，不能虚函数
void test5() {
	constexpr auto lamb = [](int n) { return n * n; };
	static_assert(lamb(3) == 9, "a");
}

namespace M::B::C {
	void func2();
	void func2() { cout << "M::B::C::func2()" << endl; };
}
//void M::B::C::func() { cout << "M::B::C::func()" << endl; };

void test6()
{
	M::B::C::func2();
	//using namespace M::B::C;

}

//Lambda 表达式中的 this 指针
struct LA {
	int a = 100;
	void func() 
	{
		auto f3 = [=] { std::cout << a << '\n'; };
		auto f4 = [&] { std::cout << a << '\n'; };
		auto f1 = [this] {cout << a << endl; };
		auto f2 = [*this] { cout << a << endl; };//C++17 传递当前对象的拷贝
		auto f5 = [&, *this] { std::cout << a << '\n';  }; // C++17 传递当前对象的拷贝
		f1();
		f2();
		f3();
		f4();
		f5();
	}
};

void test7()
{
	LA a;
	a.func();

}


//std::from_chars 和std::to_chars
void test8()
{
	const std::string str{ "123456098" };
	int value = 0;
	const auto res = std::from_chars(str.data(), str.data() + 4, value); //从字符序列中解析数值
	if (res.ec == std::errc()) {
		cout << value << ", distance " << res.ptr - str.data() << endl;
	}
	else if (res.ec == std::errc::invalid_argument) {
		cout << "invalid" << endl;
	}

	const std::string str1 = std::string("12.34");
	double val = 0;
	const auto res1 = std::from_chars(str1.data(), str1.data() + str1.size(), value);
	if (res1.ec == std::errc()) {
		cout << value << ", distance " << res.ptr - str.data() << endl;
	}
	else if (res1.ec == std::errc::invalid_argument) {
		cout << "invalid" << endl;
	}

	//std::string str2 = std::string("xxxxxxxx");
	//std::string str2;
	char str2[20];
	const int v = 1234;
	const auto res2 = std::to_chars(str2, str2 + sizeof(str2), v);
	if (res2.ec == std::errc()) {
		std::cout << "Integer conversion: " << std::string(str2, res2.ptr) << endl;
	}
	else {
		std::cout << "Conversion failed!\n";
	}

}

void test9()
{
	std::variant<int, std::string> var("hello");
	cout << var.index() << endl;
	var = 123;
	cout << var.index() << endl;

	try {
		var = "world";
		std::string str = std::get<std::string>(var); // 通过类型获取值
		var = 3;
		int i = std::get<0>(var); // 通过index获取对应值
		cout << str << endl;
		cout << i << endl;
	}
	catch (...) {
		std::cout << "catch except" << endl;
	}
}

std::optional<int> StoI(const std::string& s) 
{
	try {
		return std::stoi(s);
	}
	catch (...) {
		return std::nullopt;
	}
}

void test10() 
{
	std::string s{ "123" };
	std::optional<int> o = StoI(s);
	if (o) {
		cout << *o << endl;
	}
	else {
		cout << "error" << endl;
	}
}

void test11() 
{
	std::any a = 1;
	cout << a.type().name() << " " << std::any_cast<int>(a) << endl;
	a = 2.2f;
	cout << a.type().name() << " " << std::any_cast<float>(a) << endl;
	if (a.has_value()) {
		cout << a.type().name();
	}
	a.reset();
	if (a.has_value()) {
		cout << a.type().name();
	}
	a = std::string("a");
	cout << a.type().name() << " " << std::any_cast<std::string>(a) << endl;
}

int add(int first, int second) { return first + second; }
auto add_lambda = [](auto first, auto second) { return first + second; };
template <typename... T>
void applytest(T... arg) {
	(std::cout << ... << arg) << std::endl;
}

void test12() 
{
	std::cout << std::apply(add, std::pair(1, 2)) << endl;
	std::cout << std::apply(add_lambda, std::tuple(2.0f, 3.0f)) << endl;

	std::tuple tu(1, 2.5, "abc");
	std::apply([](auto &&... args) {return applytest(args...); }, std::move(tu)); // 正常调用
}

struct Foo1 
{
	Foo1(int first, float second, int third) {
		std::cout << first << ", " << second << ", " << third << "\n";
	}
};
class Test {
public:
	Test(int a, double b, const std::string& c) : a_(a), b_(b), c_(c) {}
	void show() const { std::cout << a_ << " " << b_ << " " << c_ << std::endl; }
private:
	int a_;
	double b_;
	std::string c_;
};
void test13() 
{
	auto tuple = std::make_tuple(42, 3.14f, 0);
	std::make_from_tuple<Foo1>(std::move(tuple));

	std::tuple tu(1, 2.5, "abc");
	Test&& t = std::make_from_tuple<Test>(std::move(tu));
	t.show();

}

void func(std::string_view stv) { cout << stv << endl; }

void test14()
{
	std::string str = "Hello World";
	std::cout << str << std::endl;

	std::string_view stv(str.c_str(), str.size());
	cout << stv << endl;
	func(stv);
}

void test15()
{
	std::string str = "str";
	const std::string constStr = std::as_const(str);
}

void test16()
{
	// 设置测试目录和文件的路径
	std::filesystem::path dir_path = "test_dir";
	std::filesystem::path file_path = dir_path / "test_file.txt"; // 使用 / 来拼接路径

	// 创建一个目录 
	if (!std::filesystem::exists(dir_path)) {
		std::filesystem::create_directory(dir_path);
	}

	// 创建并写入一个文件 
	std::ofstream file(file_path);
	file << "Hello, Filesystem!";
	file.close();

	// 检查文件是否存在 
	if (std::filesystem::exists(file_path)) {
		std::cout << "File created successfully.\n";
	}

	// 读取文件大小 
	std::cout << "File size: " << std::filesystem::file_size(file_path) << " bytes.\n";

	// 重命名文件 
	std::filesystem::path new_file_path = dir_path / "renamed_file.txt";
	std::filesystem::rename(file_path, new_file_path);

	// 遍历目录 
	std::cout << "Contents of directory:\n";
	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
		std::cout << entry.path() << std::endl;
	}

	// 删除文件和目录
	std::filesystem::remove(new_file_path);
	std::filesystem::remove(dir_path);
}

template<typename T>
void checkType() {
	if constexpr (std::is_same_v<T, int>) {
		std::cout << "Type is int" << std::endl;
	}
	else if constexpr (std::is_same_v<T, float>) {
		std::cout << "Type is float" << std::endl;
	}
	else {
		std::cout << "Type is something else" << std::endl;
	}
}

void test17()
{
	checkType<int>(); // Output: Type is int
	checkType<float>(); // Output: Type is float
	checkType<double>(); // Output: Type is something else
}

bool is_even(int n) {
	return n % 2 == 0;
}

void test18()
{
	auto add = [](auto a, auto b) {return a + b; };
	auto a = std::invoke(add, 1, 2);// a = 3
	auto a1 = add(1,2);


	std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
	// 使用 std::not_fn 过滤奇数
	auto is_odd = std::not_fn(is_even);
	numbers.erase(
		std::remove_if(numbers.begin(), numbers.end(), is_odd),
		numbers.end()
	);

	for (auto n : numbers) {
		std::cout << n << " "; // 输出: 2 4 6
	}
	std::cout << "\n";

	auto is_positive = [](int n) { return n > 0; };
	auto is_non_positive = std::not_fn(is_positive);

}

void test19()
{
	std::byte b1{ 0x0F };  // 0000 1111
	std::byte b2{ 0xF0 };  // 1111 0000

	// 位运算
	std::byte result1 = b1 | b2;         // 位或：1111 1111
	std::byte result2 = b1 & b2;         // 位与：0000 0000
	std::byte result3 = b1 ^ b2;         // 位异或：1111 1111
	std::byte result4 = ~b1;             // 位取反：1111 0000

	// 移位操作
	std::byte shift1 = b1 << 2;          // 左移：0011 1100
	std::byte shift2 = b1 >> 1;          // 右移：0000 0111

	// 复合赋值
	std::byte compound = b1;
	compound |= b2;                       // 位或赋值
	compound &= b2;                       // 位与赋值
	compound ^= b2;                       // 位异或赋值
	compound <<= 2;                       // 左移赋值
	compound >>= 1;                       // 右移赋值
}

void test20()
{
	std::vector<int> numbers{ 1, 2, 3, 4 };
	//double avg = std::reduce(numbers.begin(), numbers.end(), 0.0, std::plus<double>()) / numbers.size();
	double avg = std::reduce(numbers.begin(), numbers.end(), 0.0, std::plus<>()) / numbers.size();
	std::cout << "Average: " << avg << std::endl;

	int num1 = 12;
	int num2 = 18;
	int result = std::lcm(num1, num2);// 计算最小公倍数
	std::cout << "LCM of " << num1 << " and " << num2 << " is " << result << std::endl;

	int a = 48, b = 18;
	int gcd_result = std::gcd(a, b); // 计算最大公约数
	std::cout << "GCD of " << a << " and " << b << " is: " << gcd_result << std::endl;
}


int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	test11();
	test12();
	test13();
	test14();
	test15();
	test16();
	test17();
	test18();
	test19();
	test20();

	return 0;
}