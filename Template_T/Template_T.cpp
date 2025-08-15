#include <stdio.h>
#include <vector>

#include <execution>
#include <memory>
#include <iostream>

#include "simple.h"

using namespace std;

template<typename  T> 
void swap1(T& t1, T& t2) 
{
    T tmpT;
    tmpT = t1;
    t1 = t2;
    t2 = tmpT;
}

template<typename T>
using Vec = std::vector<T>;



int main() {
    //模板方法 
    int num1 = 1, num2 = 2;
    swap1<int>(num1, num2);
    printf("num1:%d, num2:%d\n", num1, num2);

    Stack<int> intStack;
    intStack.push(1);
    intStack.push(2);
    intStack.push(3);
    while (!intStack.isEmpty()) 
    {
        printf("num:%d\n", intStack.pop());
    }

    int maxsize = 1024;
    Stack1<int,1024> intStack1;
    for (int i = 0; i < maxsize; i++) 
    {
        intStack1.push(i);
    }
    while (!intStack1.isEmpty()) 
    {
        printf("num:%d\n", intStack1.pop());
    }

    unique_ptr<int> uptr = make_unique<int>();

    *uptr = 1000;

    unique_ptr<int> uptr1 = move(uptr);

    cout << "uptr1:" << *uptr1 << endl;


    unique_ptr<int[]> uptr2 = make_unique<int[]>(10);
    for (int i = 0; i < 10; i++) {
        cout << uptr2[i] << endl;
    }
    for (int i = 0; i < 10; i++) {
        uptr2[i] = i * i;
    }
    for (int i = 0; i < 10; i++) {
        cout << uptr2[i] << endl;
    }

    shared_ptr<int> sptr;
    shared_ptr<int> sptr1;
    shared_ptr<int> sptr2;
    cout << "0. sptr use_count:" << sptr.use_count() << endl;
    cout << "0. sptr1 use_count:" << sptr1.use_count() << endl;
    cout << "0. sptr2 use_count:" << sptr2.use_count() << endl;


    sptr = make_shared<int>(200);
    cout << "1. sptr use_count:" << sptr.use_count() << endl;

    sptr1 = sptr;
    cout << "2. sptr use_count:" << sptr.use_count() << endl;
    cout << "2. sptr1 use_count:" << sptr1.use_count() << endl;

    sptr2 = sptr;
    cout << "3. sptr use_count:" << sptr.use_count() << endl;
    cout << "3. sptr1 use_count:" << sptr1.use_count() << endl;
    cout << "3. sptr2 use_count:" << sptr2.use_count() << endl;


    cout << "3. *sptr2:" << *sptr2 << endl;
    cout << "3. *sptr2.get():" << *sptr2.get() << endl;



    *sptr2 = 300;
    cout << "4. *sptr2:" << *sptr2 << endl;
    cout << "4. *sptr1:" << *sptr1 << endl;
    cout << "4. *sptr:" << *sptr << endl;
    //sptr2 = sptr;

    sptr2.reset(new int(32));
    cout << "reset. *sptr2:" << *sptr2 << endl;
    cout << "reset. *sptr2.get():" << *sptr2.get() << endl;
    cout << "reset. *sptr1:" << *sptr1 << endl;
    cout << "reset. sptr use_count:" << sptr.use_count() << endl;
    cout << "reset. sptr1 use_count:" << sptr1.use_count() << endl;
    cout << "reset. sptr2 use_count:" << sptr2.use_count() << endl;


    shared_ptr<int> sptr3(new int);


    std::unique_ptr<int> uptr3 = std::make_unique<int>(200);
    std::unique_ptr<int> uptr4 = std::move(uptr3);

    std::shared_ptr<int> sptr4 = std::make_shared<int>(200);
    std::shared_ptr<int> sptr5 = sptr4;

    /*
    std::allocator<int> a1; // int的默认分配器
    int* a = a1.allocate(1); // 分配一个int的空间
    a1.construct(a, 7); // 构造int
    std::cout << a[0] << '\n';
    a1.deallocate(a, 1); // 解分配一个int的空间

    std::allocator<std::string> a2; // string的默认分配器
    std::string* s = a2.allocate(2); // 分配2个string的空间
    a2.construct(s, "foo");
    a2.construct(s + 1, "bar");
    std::cout << s[0] << ' ' << s[1] << '\n';
    a2.destroy(s);
    a2.destroy(s + 1);
    a2.deallocate(s, 2);
    */

    Vec<int> v;

    return 0;
}