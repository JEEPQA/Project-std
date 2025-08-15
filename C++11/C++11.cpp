#include <map>
#include <string>
#include <exception>
#include <iostream>
#include <initializer_list>
using namespace std;
//using std::map;
//using std::string;
#include <process.h>

/*
1.auto和decltype
2.范围for循环
3.列表初始化和std::initializer_list
4.std::function & std::bind & lambda表达式
5.尾置返回类型
5.右值引用 移动语义 
  引用限定符
  std::move std::forward
5.模板的改进
   函数模板的默认模板参数
   变长参数模板
   模板的别名
6.线程和并发
   std::thread std::async
   thread_local
7.智能指针
8.委托构造函数和继承构造函数
9.nullptr的使用
10.final & override
11.default和delete
12.explicit 
12.noexcept
13.constexpr
14.enum class
15.非受限联合体
16.sizeof可以用的类的数据成员上
17.static_assert
18.自定义字面量
19.字符串字面常量
20.类定义中直接为成员变量提供初始值
21.内存对齐alignof() alignas() std::alignment_of()
22.随机数<random>
23.正则表达式<regex>
24.时间<chrono>
25.新增容器和算法
   <array> <forward_list> 
   <unordered_map> <unordered_set>
26.<tuple>
27.类型支持<type_traits> <typeindex>
28.std::hash
29.<system_error>
30.std::ref std::cref std::reference_wrapper
31.char16_t，char32_t 和u前缀
*/

#include "foo.h"
#include <functional>

union var  // union with non trivial type
{
	string str;
	int num;
	var() {}
	~var() {}
};

//RVO
vector<int> GetArray() {
	return vector<int>{1, 2, 3, 4};    // 这里就定义了一个临时对象
}

template <typename Value>
using string_map = map<string, Value>;

template < typename T1, typename T2 = int>
class DefClass1 {};

//template < typename T1 = int, typename T2>
//class DefClass2 {};  // ERROR: 无法通过编译：因为模板参数的默认值没有遵循“由右往左”的规则

void test1() 
{
	string_map<int> m;
}

struct s
{
	int i;
	char c;
};

void handle_exception(exception_ptr e)
{  // passing by value is ok
	try
	{
		if (e) rethrow_exception(e);
	}
	catch (exception const& e)
	{
		cout << "caught exception '" << e.what() << "'\n";
	}
}
void test2()
{
	exception_ptr e;
	try
	{
		string{}.at(1);  // this generates an out_of_range exception
	}
	catch (...)//捕获多种数据类型的异常对象 
	{
		e = current_exception();

	}
	handle_exception(e);

	std::cout << "s::c " << sizeof(s::c) << std::endl;
}

//原始字符串字面值 无需转义
std::string fozu = R"(
                            _ooOoo_
                           o8888888o
                           88" . "88
                           (| -_- |)
                            O\ = /O
                        ____/`---'\____
                      .   ' \\| |// `.
                       / \\||| : |||// \
                     / _||||| -:- |||||- \
                       | | \\\ - /// | |
                     | \_| ''\---/'' | |
                      \ .-\__ `-` ___/-. /
                   ___`. .' /--.--\ `. . __
                ."" '< `.___\_<|>_/___.' >'"".
               | | : `- \`.;`\ _ /`;.`/ - ` : | |
                 \ \ `-. \_ __\ /__ _/ .-` / /
         ======`-.____`-.___\_____/___.-`____.-'======
                            `=---='
		)";

void test3()
{
	foo f{ 1, 2, 3, 4, 5 };
	cout << "f = " << f.str() << "\n";
	cout << "append 6 and 7\n";
	f.append({ 6, 7 });
	cout << "f = " << f.str() << "\n";

	std::cout << fozu << std::endl;
}

//initializer_list
void test4()
{
	foo f{ 1, 2, 3, 4, 5 };
	cout << "f = " << f.str() << "\n";
	cout << "append 6 and 7\n";
	f.append({ 6, 7 });
	cout << "f = " << f.str() << "\n";

	static std::map<string, string> const nameToBirthday = {
		{"lisi", "18841011"},
		{"zhangsan", "18850123"},
		{"wangwu", "18870908"},
		{"zhaoliu", "18810316"},
	};

	S<int> s = { 1, 2, 3, 4, 5 }; //automatically construct a initializer_list 
	// object and copy it
	s.append({ 6, 7 , 8 });         //list-initialization in function call

	cout << "The vector size is now " << s.c_arr().second << " ints:" << endl;

	for (auto n : s.v) { cout << ' ' << n; }
	cout << endl;

	cout << "range-for over brace-init-list: " << endl;

	for (auto x : { -1, -2, 03 }) { cout << x << " "; }
	cout << endl;

	auto al = { 10, 11, 12 };  //special rule for auto
	cout << "The list bound to auto has size() = " << al.size() << endl;

}

constexpr long double operator"" km(long double x) {
	return x * 1000;
}

constexpr long double operator"" m(long double x) {
	return x;
}

constexpr long double operator"" cm(long double x) {
	return x / 100;
}

void test5()
{
	long double distance = 3.4km + 150.0m + 13.0cm; // 距离单位转换
	cout << distance << endl;
}


void xprintf()
{
	cout << endl;
}

template <typename Targs>
void xprintf(const Targs& t) {
	cout << t << endl;
}

template<typename T, typename... Targs>
void xprintf(T value, Targs... Fargs)
{
	cout << value << ' ';
	if ((sizeof ...(Fargs)) > 0)
	{
		//这里调用的时候没有显式指定模板，是因为函数模板可以根据函数参数自动推导
		xprintf(Fargs...);
	}
	else
	{
		xprintf();
	}
}

void test6()
{
	xprintf("小明个人信息:", "小明", "男", 35, "程序员", 169.5);
}

int sum2(int x, int y) { return x + y; }

void test7()
{
	using namespace std::placeholders;

	Object instance;

	//using ExampleFunction = std::function<void(const std::string&)>;
	//ExampleFunction func = std::bind(&Object::hello, &instance, std::placeholders::_1);
	auto func = std::bind(&Object::hello, &instance, std::placeholders::_1);
	func("World");  // 等价于 instance.hello(str)

	auto func1 = std::bind(&goodbye, std::placeholders::_1);
	func1("ddd");  // 等价于 goodbye(str)

	auto lambda = [](std::string pre, char o, int rep, std::string post) {
		std::cout << pre;
		while (rep-- > 0) std::cout << o;
		std::cout << post << '\n';
		};

	auto g = std::bind(&decltype(lambda)::operator(), &lambda, _1, _2, _3, _4);
	g("G", 'o', 'o' - 'g', "gol");


	//Box b1(5.0, 4.0, 3.0);
	//Box b1 = { 5.0, 4.0, 3.0 };
	Box b1{ 5.0, 4.0, 3.0 };
	cout << b1.getVolume() << endl;

	Box* b2 = new Box{ 5.0, 4.0, 3.0 };
	cout << (*b2).getVolume() << endl;
	cout << (*b2).height << endl;
	cout << (*b2).length << endl;
	//Box b2(6.0, 5.0, 4.0);
	//Box b3;

	//cout << "Volume of b1 : " << b1.getVolume() << endl;
	//cout << "Volume of b2 : " << b2.getVolume() << endl;

	// 把两个对象相加，得到 Box3
	//b3 = b1 + b2;

	// Box3 的体积
	//cout << "Volume of b3 : " << b3.getVolume() << endl;

	unique_ptr<int> pInt(new int(5));
	cout << *pInt << endl;

	unique_ptr<int> ptr = make_unique<int>();
	cout << *ptr << endl;

	unique_ptr<Box> ptr_b1 = make_unique<Box>(5.0, 4.0, 3.0);
	cout << "Volume of b1 : " << ptr_b1->getVolume() << endl;

	unique_ptr<Box> ptr_b2 = move(ptr_b1);
	cout << "Volume of b2 : " << ptr_b2->getVolume() << endl;

	shared_ptr<Box> ptr_b3(new Box(5.0, 4.0, 3.0));
	cout << "Volume of b3 : " << ptr_b3->getVolume() << endl;

	shared_ptr<Box> ptr_b4 = make_shared<Box>(5.0, 4.0, 3.0);
	cout << "Volume of b4 : " << ptr_b4->getVolume() << endl;
	cout << "Volume of b4 height: " << ptr_b4->height << endl;
	cout << "Volume of b4 : " << (*ptr_b4).getVolume() << endl;

	int x = 10;
	int y = 20;
	int z = 20;

	auto sum = [=]() mutable {x = x + 1; return x + y + z; };

	cout << "x + y : " << sum() << endl;

	auto sum1 = bind(&sum2, placeholders::_1, placeholders::_2);
	cout << "sum1: " << sum1(1, 2) << endl;

	auto f = bind(&Box::getVolume, &b1);
	cout << "f: " << f() << endl;

	function<int(int x, int y)> f1 = sum2;
	//auto f1 = sum2;
	cout << "f1: " << f1(1, 3) << endl;
}

void test8()
{

	int a = 3;
	auto c1 = a;
	decltype(a) c2 = a;
	decltype((a)) c3 = a;


	//auto一般会忽略顶层const，而把底层const保留下来。与之相反，decltype会保留变量的顶层const
    //auto在推导类型时可能会去掉const和volatile限定符，特别是当表达式不是指针或引用时。
	//decltype会保留表达式的const和volatile限定符。
	const int d = 5;
	auto f1 = d;
	decltype(d) f2 = d;

	cout << typeid(c1).name() << endl;
	cout << typeid(c2).name() << endl;
	cout << typeid(c3).name() << endl;
	cout << typeid(f1).name() << endl;
	cout << typeid(f2).name() << endl;

	c1++;
	c2++;
	c3++;
	f1++;
	//f2++;   //错误，f2是整型常量，不能执行自增操作

	cout << c1 << " " << c2 << " " << c3 << " "
		<< f1 << " " << f2 << endl;

}

void test9()
{
	//auto值类型推导
	int a = 1;
	auto va = a;

	const int b = a;//b不可修改
	auto vb = b;
	vb = 4;//vb可修改

	const int* pc = &a;
	auto vpc = pc; //auto推导为int*，可以修改指针指向的值，但不能修改指针本身

	int* const pcc = &a;
	auto vpcc = pcc; //auto推导为int* const，不能修改指针指向的值，但可以修改指针本身
	printf("test1: a=%d, va=%d, b=%d, vb=%d, *pc=%d, vpc=%d, *pcc=%d, vpcc=%d\n", a, va, b, vb, *pc, *vpc, *pcc, *vpcc);

	const char name[] = "hello";
	auto vname = name; //auto推导为const char*，不能修改指针指向的值，但可以修改指针本身
	printf("test1: vname=%s\n", vname);

	//auto& 左值引用推导 
	auto& va_ref = a; //va_ref是int&，可以修改a的值

	const int b_ref = a;
	auto& vb_ref = b_ref; //vb_ref是const int&，不能修改b_ref的值

	const int* pc_ref = &a;
	auto& vpc_ref = pc_ref; //vpc_ref是const int*&，可以修改指针本身，但不能修改指针指向的值

	int* const pcc_ref = &a;
	auto& vpcc_ref = pcc_ref; //vpcc_ref是int* const&，可以修改指针本身，但不能修改指针指向的值

	printf("test1: a=%d, va_ref=%d, b_ref=%d, vb_ref=%d, *pc_ref=%d, vpc_ref=%d, *pcc_ref=%d, vpcc_ref=%d\n",
		a, va_ref, b_ref, vb_ref, *pc_ref, *vpc_ref, *pcc_ref, *vpcc_ref);

	const char name_ref[] = "hello";
	auto& vname_ref = name_ref; //vname_ref是const char (&)[6]，不能修改指针指向的值，但可以修改指针本身

	printf("test1: vname_ref=%s\n", vname_ref);

	//auto&& 右值引用推导
	auto&& va_rvalue = a; //va_rvalue是int&&，可以修改a的值

}

#pragma pack(push, 1)
struct A1
{
	char a;
	int b;
	double c;
	char d[11];
};
#pragma pack(pop)

struct alignas(8) A2
{
	char a;
	int b;
	double c;
	char d[11];
};


#define CHECK_ALIGN(ptr, alignment)                       \
  do{                                                     \
    constexpr size_t status                               \
       = reinterpret_cast<uintptr_t>(ptr) % alignment;    \
    static_assert(status == 0, "ptr must be aligned");    \
  }while(0)                                               \

void test10()
{
	std::cout << "size of A1: " << sizeof(A1) << std::endl;
	std::cout << "Alignment of A1: " << alignof(A1) << std::endl;

	std::cout << "size of A2: " << sizeof(A2) << std::endl;
	std::cout << "Alignment of A2: " << alignof(A2) << std::endl;
}

void modifyValue(int& value) {
	value *= 2;
}
void printValue(const int& value) {
	std::cout << "Value: " << value << std::endl;
}
void test11()
{
	int number = 42;
	auto refNumber = std::ref(number);
	auto crefNumber = std::cref(number);

	modifyValue(refNumber);  // 使用可修改的引用作为参数
	printValue(crefNumber);  // 使用常量引用传递参数

	int number1 = 42;
	int number2 = 73;

	std::vector<std::reference_wrapper<int>> numbers = { std::ref(number1), std::ref(number2) };

	for (auto& num : numbers) {
		num.get() += 10;  // 修改原始对象的值
	}

	std::cout << "Number 1: " << number1 << std::endl;
	std::cout << "Number 2: " << number2 << std::endl;
}

class Foo {
public:
	int x = 1;
	void func() {
		auto lambda = [this]() { x = 2; }; // 正确，捕获 this 指针
	}
};


std::tuple<std::string, int32_t, int64_t, std::vector<int>>
func1()
{ 
	std::string v1("nyaa");    
	int32_t v2 = 1;    
	int64_t v3 = 2;    
	std::vector<int> v4;   
	return std::make_tuple(v1, v2, v3, v4); 
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

	return 0;
}
