#include <iostream>
using std::cout;

struct goo {};

class foo
{
public:
	template <typename T>
	void fce(T const & t) {
		cout << "foo::fce<T>()\n";
	}
	void fce(goo const & g) {cout << "foo::fce(goo const &)\n";}
	void fce(goo& g) { cout << "foo::fce(goo& g)\n"; }
	void fce(goo&& g) { cout << "foo::fce(goo* g)\n"; }
	//void fce(goo g) { cout << "foo::fce(goo g)\n"; }
	void fce(goo* g) { cout << "foo::fce(goo* g)\n"; }


};

class MyObject {
private:
	int* data;

public:
	// 默认构造函数
	MyObject() : data(nullptr) 
	{
		std::cout << "Default Constructor" << std::endl;
	}

	// 带参数的构造函数
	MyObject(int value) : data(new int(value)) 
	{
		std::cout << "Regular Constructor" << std::endl;
	}

	// 移动构造函数
	MyObject(MyObject&& other) noexcept : data(other.data) 
	{
		other.data = nullptr;
		std::cout << "Move Constructor" << std::endl;
	}

	// 析构函数
	~MyObject() {
		delete data;
		std::cout << "Destructor" << std::endl;
	}

	// 打印数据
	void printData() const {
		if (data != nullptr) {
			std::cout << "Data: " << *data << std::endl;
		}
		else {
			std::cout << "Data is null" << std::endl;
		}
	}
};



int main(int argc, char * argv[])
{
	goo g;
	foo f;
	f.fce(g);
	f.fce(1);

	MyObject obj1(10);
	obj1.printData();

	// 使用std::move调用移动构造函数
	MyObject obj2(std::move(obj1));
	obj2.printData();

	// obj1的data现在为null
	obj1.printData();


	return 0;
}
