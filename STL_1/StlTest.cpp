#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <iterator>
#include<algorithm>
#include <execution>

#include <iostream>
#include <memory>
#include <functional>

#include <any>
#include <optional>
#include <variant>

#include <complex>

#include <type_traits>
#include <typeindex>
#include <typeinfo>

using namespace std;


//将函数指针放入容器
typedef void(*test)();
using maptest2 = std::function<void()>;
void mapdisplay1()
{
	cout << "one" << endl;
}
void mapdisplay2()
{
	cout << "two" << endl;
}
void mapdisplay3()
{
	cout << "three" << endl;
}

static bool abs_compare(int a, int b)
{
	return (std::abs(a) < std::abs(b));
}

//
//array vector
//
void test1()
{
	std::array<int, 5> arr = { 1, 2, 3, 4, 5 }; // 声明一个定长为5的int数组
	for (const auto& value : arr) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	// 使用索引访问数组元素
	std::cout << "Element at index 2: " << arr.at(2) << std::endl;
	std::cout << "Element at index 2: " << arr[2] << std::endl;

	// 获取数组的大小
	std::cout << "Array size: " << arr.size() << std::endl;

	// 修改数组元素
	arr[3] = 10;



	vector<string> v1;
	for (int i = 0; i < 20; i++)
	{
		v1.push_back("key");
	}
	for (int i = 0; i < v1.size(); i++)
	{
		//cout << v1[i] << endl;
	}

	//1.下标
	vector<string> v2 = { "hi","my","name","is","ldx" };
	for (int i = 0; i < v2.size(); i++)
	{
		cout << v2[i];
		cout << v2.at(i);
	}
	std::cout << std::endl;
	//2.正向迭代器
	//for (vector<string>::iterator iter = v2.begin(); iter != v2.end(); iter++)
	for (auto iter = v2.begin(); iter != v2.end(); iter++)
	{
		//cout << *iter << endl;
		//下面两种方法都都可以检查迭代器是否为空
		//cout << (*iter).empty() << endl;
		//cout << iter->empty() << endl;
	}
	//反向迭代器
	//for (vector<string>::reverse_iterator iter = v2.rbegin(); iter != v2.rend(); iter++)
	for (auto iter = v2.rbegin(); iter != v2.rend(); iter++)
	{
		//cout << *iter << endl;
	}

    //3.范围for循环
	for (auto i : v2)
	{
        cout << i << " ";
	}

	//4.for_each算法
	for_each(v2.begin(), v2.end(), [](const string& s) {cout << s << " ";});


	v2.pop_back();
	//输出 v2 容器新的size
	cout << "size is :" << v2.size() << endl;
	//输出 v2 容器新的容量
	cout << "capacity is :" << v2.capacity() << endl;
	for (int i = 0; i < v2.size(); i++) {
		cout << v2[i] << " ";
	}
	cout << endl;

	v2.insert(v2.begin() + 1, "cool");
	//输出 v2 容器新的size
	cout << "insert size is :" << v2.size() << endl;
	//输出 v2 容器新的容量
	cout << "insert capacity is :" << v2.capacity() << endl;
	for (int i = 0; i < v2.size(); i++) {
		cout << v2[i] << " ";
	}
	cout << endl;

	auto iter = v2.erase(v2.begin() + 2);//删除元素 3
	//输出 v2 容器新的size
	cout << "size is :" << v2.size() << endl;
	//输出 demo 容器新的容量
	cout << "v2 is :" << v2.capacity() << endl;
	for (int i = 0; i < v2.size(); i++) {
		cout << v2[i] << " ";
	}
	cout << endl << *iter << endl;
}

//
//list forward_list
//
void test2()
{
	list<string> l1 = { "hi","you","are","boy" };
	list<string>::iterator iter1;
	l1.push_back("go");
	for (iter1 = l1.begin(); iter1 != l1.end(); iter1++)
	{
		cout << *iter1 << " ";
	}
	cout << endl;

	l1.insert(++l1.begin(), "lo");
	for (iter1 = l1.begin(); iter1 != l1.end(); iter1++)
	{
		cout << *iter1 << " ";
	}
	cout << endl;

	l1.erase(++l1.begin());

	l1.remove("hi");
	for (iter1 = l1.begin(); iter1 != l1.end(); iter1++)
	{
		cout << *iter1 << " ";
	}
	cout << endl;

	forward_list<string> l2 = { "hi","you","are","new" };
	for (auto iter2 = l2.cbegin(); iter2 != l2.cend(); iter2++) {
		cout << *iter2 << " ";
	}
	cout << endl;
}

//
//deque
//
void test3()
{

	deque<string> deq1 = { "Header","files","for","the" };
	deq1.push_front("b");
	deq1.push_back("C++");
	for (auto iter3 = deq1.begin(); iter3 != deq1.end(); iter3++)
	{
		cout << *iter3 << " ";
	}
	cout << endl;

	string string1 = "Header";
	string string2 = "Headers";

	replace(deq1.begin(), deq1.end(), string1, string2);
	for (auto iter4 = deq1.begin(); iter4 != deq1.end(); iter4++)
	{
		cout << *iter4 << " ";
	}
	cout << endl;
}

//
//map
//
void test4()
{

	map<int, string> m1;
	m1[1] = "map1";
	m1[2] = "map2";
	m1.insert(map<int, string>::value_type(3, "map3"));
	m1.insert(map<int, string>::value_type(4, "map4"));
	m1.insert(pair<int, string>(5, "map5"));
	m1.insert(pair<int, string>(6, "map6"));
	m1.insert(make_pair(8, "map8"));
	m1.insert(make_pair(8, "map8.1"));
	m1.insert(make_pair(7, "map7"));

	map<int, string>::iterator iter5;
	for (iter5 = m1.begin(); iter5 != m1.end(); iter5++)
	{
		//printf("%d-->%s\n", iter5->first, iter5->second);
		cout << "map " << iter5->first << " " << iter5->second << endl;
	}


	iter5 = m1.find(1);
	if (iter5 != m1.end())
		cout << "Find, the value is" << iter5->second << endl;
	else
		cout << "Do not Find" << endl;


	int n = m1.erase(1); //如果刪除了會返回1，否則返回0
	//用迭代器范围刪除 : 把整个map清空
	//m1.erase(m1.begin(), m1.end());

	//等同于m1.clear()
	int nSize = m1.size();

	map<int, test> m2;
	//pair<int, test> p1 = make_pair(1, mapdisplay1);
	//pair<int, test> p2 = make_pair(2, mapdisplay2);
	m2.insert(make_pair(1, mapdisplay1));
	m2.insert(make_pair(2, mapdisplay2));

	m2[1]();
	(*m2[2])();

	map<int, maptest2> m22;
	m22.insert(make_pair(3, mapdisplay3));
	m22[3]();

	unordered_map<int, string> m3;
	m3[1] = "unordered_map1";
	m3[2] = "unordered_map2";
	m3.insert(map<int, string>::value_type(3, "unordered_map3"));
	m3.insert(map<int, string>::value_type(4, "unordered_map4"));
	m3.insert(pair<int, string>(5, "unordered_map5"));
	m3.insert(pair<int, string>(6, "unordered_map6"));
	m3.insert(make_pair(8, "unordered_map8"));
	m3.insert(make_pair(8, "unordered_map8.1"));
	m3.insert(make_pair(7, "unordered_map7"));

	unordered_map<int, string>::iterator iter7;
	for (iter7 = m3.begin(); iter7 != m3.end(); iter7++)
	{
		//printf("%d-->%s\n", iter5->first, iter5->second);
		cout << "unordered_map " << iter7->first << " " << iter7->second << endl;
	}

	unordered_multimap<int, string> m4;
	m4.insert(map<int, string>::value_type(3, "unordered_multimap3"));
	m4.insert(map<int, string>::value_type(4, "unordered_multimapp4"));
	m4.insert(pair<int, string>(5, "unordered_multimap5"));
	m4.insert(pair<int, string>(6, "unordered_multimap6"));
	m4.insert(make_pair(8, "unordered_multimap8"));
	m4.insert(make_pair(8, "unordered_multimap8.1"));
	m4.insert(make_pair(7, "unordered_multimap7"));

	unordered_multimap<int, string>::iterator iter8;
	for (iter8 = m4.begin(); iter8 != m4.end(); iter8++)
	{
		//printf("%d-->%s\n", iter5->first, iter5->second);
		cout << "unordered_map " << iter8->first << " " << iter8->second << endl;
	}
}

//
//map
//
void test5()
{
	set<string> s1;
	s1.insert("a");
	s1.insert("c");
	s1.insert("b");
	s1.insert("d");
	s1.insert("a");

	//cout << "set 中的第一个元素是 ：" << *s1.begin() << endl;
	for (set<string>::iterator iter6 = s1.begin(); iter6 != s1.end(); iter6++)
	{
		cout << *iter6 << " ";
	}
	cout << endl;

	multiset<string> s2;
	s2.insert("a");
	s2.insert("c");
	s2.insert("b");
	s2.insert("d");
	s2.insert("a");

	for (set<string>::iterator iter6 = s2.begin(); iter6 != s2.end(); iter6++)
	{
		cout << *iter6 << " ";
	}
	cout << endl;
}

//
//queue stack
//
void test6()
{
	queue<string>q1;
	q1.push("queue1");
	q1.push("queue2");
	q1.push("queue3");
	q1.push("queue4");
	q1.push("queue5");
	cout << "q1 front " << q1.front() << endl;
	cout << "q1 back " << q1.back() << endl;

	q1.pop();
	cout << "q1 front " << q1.front() << endl;
	cout << "q1 back " << q1.back() << endl;

	stack<string> s3;
	s3.push("stack1");
	s3.push("stack2");
	s3.push("stack3");
	s3.push("stack4");
	s3.push("stack5");
	cout << "s3 top " << s3.top() << endl;
	s3.pop();
	cout << "s3 top " << s3.top() << endl;

}

//
//pair tuple
//
void test7()
{

	pair<string, double> p1 = make_pair("shoes", 20.0);
	pair <string, double> p2("tomatoes", 3.25);
	pair <string, double> p3;
	p3.first = "lightbulbs"; // type of first is string
	p3.second = 0.99; // type of second is double

	std::tuple<int, double, std::string> myTuple = { 1, 2.2, "example" };
	auto [x1, y1, z1] = myTuple;
	std::cout << x1 << ", " << y1 << ", " << z1 << std::endl;

	int a; double b; std::string c;
	std::tie(a, b, c) = std::make_tuple(1, 2.3, "hello");

	std::tuple<int, double, std::string> t1(1, 2.3, "hello");
	auto t2 = std::make_tuple(1, 2.3, "hello");

	int a11 = std::get<0>(t2);
	double b11 = std::get<1>(t2);
	std::string c11 = std::get<2>(t2);

	std::cout << std::tuple_size<decltype(t2)>::value << std::endl;

	using T = std::tuple<int, double, std::string>;
	std::tuple_element<1, T>::type b12 = std::get<1>(t2);
}

//
//any optional variant
//
void test8()
{
	std::any a1 = 32; //type: int
	std::any b1 = "wegrthweg"; //type : const chr*

	std::any b2{ std::in_place_type<int>, 420 };
	std::any a2{ std::in_place_type<std::string>, "asdfbsrghtr34" };

	auto an0 = std::make_any<std::string>("Hello, std::any!\n");
	auto an1 = std::make_any<std::complex<double>>(0.1, 2.3);

	int a = 1;
	std::optional<double> op1(10.09); // 创建一个包含值10.09的std::optional对象
	std::optional<double> op2(std::move(a)); //使用移动构造函数创建一个新的std::optional对象

	std::variant<std::monostate, int> test; // OK
	std::cout << "index: " << test.index(); //输出： 0

	//std::variant<std::complex<double>> t1{ 1.0,564.0 }; // ERROR
	std::variant<std::complex<double>> t2{ std::in_place_type<std::complex<double>>,322.0, 2323.0 };
	std::variant<std::complex<double>> t3{ std::in_place_index<0>, 35.0, 8.0 };
}

int main(int argc, char** argv)
{

	std::vector<int> vec(1000, 1);
	std::for_each(std::execution::par, vec.begin(), vec.end(), [](int& n) { n++; });

	std::cout << "Sum: " << std::accumulate(vec.begin(), vec.end(), 0) << std::endl;


	std::vector<int> v{ 3, 1, -14, 1, 5, 9 };
	std::vector<int>::iterator result;

	result = std::max_element(v.begin(), v.end());
	std::cout << "max element is: " << *result << '\n';
	std::cout << "max element at: " << std::distance(v.begin(), result) << '\n';

	result = std::max_element(v.begin(), v.end(), abs_compare);
	std::cout << "max element is: " << *result << '\n';
	std::cout << "max element (absolute) at: " << std::distance(v.begin(), result) << '\n';


	std::list<int> mylist = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
	mylist.sort();
	for (int i : mylist) {
		std::cout << i << ' ';
	}

	return 1;
}


