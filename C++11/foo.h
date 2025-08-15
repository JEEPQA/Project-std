#include <initializer_list>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

template <typename T>
T num_double(T const& x) { return x * x; }

class foo
{
public:
	foo(initializer_list<int> lst)
	{
		for (auto item : lst)
			_nums.push_back(num_double(item));
	}

	void append(initializer_list<int> lst)
	{
		for (auto item : lst)
			_nums.push_back(num_double(item));
	}

	string str()
	{
		ostringstream ostr;
		ostr << "[";
		for (int i = 0; i < _nums.size() - 1; ++i)
			ostr << _nums[i] << ", ";
		ostr << _nums.back() << "]";
		return ostr.str();
	}

private:
	vector<int> _nums;
};

template <class T>
struct S {
	vector<T> v;
	S(initializer_list<T> l) : v(l) {
		cout << "constructed with a " << l.size() << "-elements lists" << endl;
	}
	void append(std::initializer_list<T> l) {
		v.insert(v.end(), l.begin(), l.end());
	}

	pair<const T*, size_t> c_arr() const {
		return { &v[0], v.size() };
	}

};

void goodbye(const std::string& s)
{
	std::cout << "Goodbye " << s << '\n';
}

class Object {
public:
	void hello(const std::string& s)
	{
		std::cout << "Hello " << s << '\n';
	}
};

class Box
{
public:
    double length;  // 长度
    double width;   // 宽度
    double height;  // 高度

public:
    Box() {
        length = 0.0;
        width = 0.0;
        height = 0.0;
        std::cout << "构造函数" << std::endl;
    }

    Box(double a, double b, double c)
    {
        length = a;
        width = b;
        height = c;
        std::cout << "构造函数" << std::endl;
    }
    ~Box() {
        std::cout << "析构函数" << std::endl;
    }

    double getVolume(void)
    {
        return length * width * height;
    }

    // 重载 + 运算符，用于把两个 Box 对象相加
    Box operator+(const Box& b)
    {
        Box box;
        box.length = this->length + b.length;
        box.width = this->width + b.width;
        box.height = this->height + b.height;
        return box;
    }
};

class Box2
{
    double length;  // 长度
    double width;   // 宽度
    double height;  // 高度

public:
    Box2() {
        length = 0.0;
        width = 0.0;
        height = 0.0;
        std::cout << "构造函数" << std::endl;
    }

    Box2(double a, double b, double c)
    {
        length = a;
        width = b;
        height = c;
        std::cout << "构造函数" << std::endl;
    }
    ~Box2() {
        std::cout << "析构函数" << std::endl;
    }

    double getVolume(void)
    {
        return length * width * height;
    }

    virtual void test() = 0;
};

Box2* box = nullptr;


