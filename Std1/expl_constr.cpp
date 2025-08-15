#include <memory>
#include <iostream>
using std::cout;

struct MyClass
{
    MyClass() : i(2) {}  // Constructor initializes i to 0
	int i;
};


int main()
{
	MyClass * mine = (MyClass *)malloc(sizeof(MyClass));  // Allocates memory
	if (mine == nullptr) {
		return -1;  // Check if memory allocation was successful
    }
    //new (mine) MyClass();  // Placement new to construct MyClass in allocated memory
	cout << mine->i;

	return 0;
}

