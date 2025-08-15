#include <iostream>
using std::cout;

class MyClass {
public:

	MyClass& operator=(const MyClass& other) {
		if (this != &other) {
			// 释放当前对象的资源
			delete[] data;
			// 分配新资源并复制内容
			data = new int[other.size];
			std::copy(other.data, other.data + other.size, data);
			size = other.size;
		}
		return *this;
	}
public:
	int* data = nullptr;
	size_t size;
};

struct foo {};
struct goo : foo
{
	int i;
};

int main()
{
	goo g;
	g.i = 101;

	foo & f = g;
	
	foo f2;
	f = f2;  // je vobec nieco taketo validne ??

	cout << "g.i:" << g.i << "\n";	

    MyClass obj1;
	MyClass obj2;
    obj2.data = new int[5] {1, 2, 3, 4, 5}; // 
    obj2.size = 5; //

    obj1 = obj2;  // 使用重载的赋值运算符
    cout << "obj1.data[0]: " << obj1.data[0] << "\n"; // 
    cout << "obj1.size: " << obj1.size << "\n"; //

	int* p = nullptr;
	delete p;

    int* p2 = new int[obj2.size];
	for (size_t i = 0; i < obj2.size; ++i) {
		p2[i] = obj2.data[i]; // 复制数据
    }
    delete[] p2; // 释放内存

	return 0;
}

