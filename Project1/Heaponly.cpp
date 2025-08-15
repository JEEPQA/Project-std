#include <stdio.h>
#include <crtdbg.h>
#include <memory>

class Heaponly
{
public:
    ~Heaponly() = default;

    static Heaponly* CreateInstance()
    {
        return new Heaponly();
    }
    void test()
    {
        printf("Heaponly test\n");
    }

    Heaponly(const Heaponly&) = delete;
    Heaponly& operator=(const Heaponly&) = delete;

private:
    Heaponly()= default;

};

class Heaponly2
{
public:
    Heaponly2() = default;
    void destroy()
    {
        delete this;
    }
    void test()
    {
        printf("Heaponly2 test\n");
    }
private:
    ~Heaponly2() = default;

};

class Heaponly3
{
public:

    static Heaponly3* CreateInstance()
    {
        return new Heaponly3();
    }

    static std::unique_ptr<Heaponly3, void(*)(Heaponly3*)> CreateInstance2()
    {
        return std::unique_ptr<Heaponly3, void(*)(Heaponly3*)>(new Heaponly3, [](Heaponly3* p)->void {p->destroy(); });
    }
    void test()
    {
        printf("Heaponly3 test\n");
    }
    void destroy()
    {
        delete this;
    }

    Heaponly3(const Heaponly3&) = delete;
    Heaponly3& operator=(const Heaponly3&) = delete;

private:
    Heaponly3() = default;
    ~Heaponly3() = default;

};

class stackonly
{
public:
    stackonly() = default;
    ~stackonly() = default;

    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;
    void* operator new(size_t,void* p) = delete;

    void test()
    {
        printf("stackonly test\n");
    }

};


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Heaponly* obj = Heaponly::CreateInstance();
    obj->test();
    delete obj;

    Heaponly2* obj2 = new Heaponly2();
    obj2->test();
    obj2->destroy();

    Heaponly3* obj3 = Heaponly3::CreateInstance();
    obj3->test();
    obj3->destroy();

    std::unique_ptr<Heaponly3, void(*)(Heaponly3*)> ob3_1 = Heaponly3::CreateInstance2();
    ob3_1->test();

    stackonly obj4;


    _CrtDumpMemoryLeaks();
}