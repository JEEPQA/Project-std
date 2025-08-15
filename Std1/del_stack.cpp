#include <iostream>
#include <basetsd.h>
using std::cout;
using std::endl;

struct Person {
	int age;
	double salary;
	char grade;
	bool isActive;
};

void fun(int(&p)[10]) {
	cout << "sizeof(p)=" << sizeof(p) << endl;
}
void fun2(int* p) {
	cout << "sizeof(p)=" << sizeof(p) << endl;
}

int main()
{

	int a = 12;
	int& b = a; //引用实际上开辟了内存用于保存被引用变量的地址
	int* p1 = &a;

	cout << std::hex << "&a = " << &a << ",&b = " << &b << ",p1 = " << p1 << endl;

	int k = 20, i = 0, j = 19;
	//delete &i;
	i = 15;
	cout << k << "," << i << ", " << j << "\n";

	int& ref = i; // 创建一个引用
	cout << "引用 ref 的值: " << ref << endl; // 输出 15
	ref = 30; // 修改引用的值
	cout << "修改引用后 i 的值: " << i << endl; // 输出 30
	cout << "修改引用后 ref 的值: " << ref << endl; // 输出 30

	int&& rvalueRef = 40; // 创建一个右值引用
	cout << "右值引用 rvalueRef 的值: " << rvalueRef << endl; // 输出 40
	rvalueRef = 50; // 修改右值引用的值
	cout << "修改右值引用后 rvalueRef 的值: " << rvalueRef << endl; // 输出 50

	int* p = (int*)malloc(sizeof(int)); // 使用 malloc 分配内存
	*p = 10; // 给分配的内存赋值
	free(p); // 释放内存

	int* q = new int; // 使用 new 分配内存
	*q = 20; // 给分配的内存赋值
	delete q; // 释放内存

	int arr[] = { 10, 20, 30, 40, 50 };
	int* ptr = arr; // 指向数组第一个元素

	cout << "当前指针指向的值: " << *ptr << endl; // 输出 10

	ptr++; // 指针加1，移动到下一个元素
	cout << "指针加1后指向的值: " << *ptr << endl; // 输出 20

    cout << "sizeof(int): " << sizeof(int) << endl; // 输出 4
	cout << "sizeof(long): " << sizeof(long) << endl;//
	cout << "sizeof(long long): " << sizeof(long long) << endl;//
    cout << "sizeof(double): " << sizeof(double) << endl; // 输出 8
    cout << "sizeof(char): " << sizeof(char) << endl; // 输出 1
    cout << "sizeof(bool): " << sizeof(bool) << endl; // 输出 1

	cout << "sizeof(UINT64): " << sizeof(UINT64) << endl; //

	Person person = { 25, 5000.50, 'A', true };
	char* basePtr = (char*)&person;
    cout << "Person age: " << *(int*)basePtr << endl; // 输出 25
    cout << "Person salary: " << *(double*)(basePtr + sizeof(int)) << endl; // 输出 5000.50
    cout << "Person grade: " << *(char*)(basePtr + sizeof(int) + sizeof(double)) << endl; // 输出 'A'
    cout << "Person isActive: " << *(bool*)(basePtr + sizeof(int) + sizeof(double) + sizeof(char)) << endl; // 输出 1 (true)
	
	int arr1[10] = { 0 };
	int(&p2)[10] = arr1;
	cout << "sizeof(arr)=" << sizeof(arr1) << endl;
	fun(p2);

	return 0;
}
