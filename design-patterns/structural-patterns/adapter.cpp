#include <iostream>
using namespace std;

//新旧API的兼容，例如在大型库升级时保持向后兼容性。
//第三方包的适配，使得第三方库能够在现有系统中使用。
//统一多个类的接口，如数据库ORM（对象关系映射）。
//将不兼容的接口转换为兼容的接口，使得不同的系统或组件能够协同工作。
//适配器模式（Adapter Pattern）是一种结构型设计模式，它允许将一个类的接口转换成客户端期望的另一个接口。
//类适配器和对象适配器,类适配器通过多继承来实现适配，而对象适配器通过组合来实现适配。

class Target
{
public:
    Target() {}
    virtual ~Target() {}
    virtual void Request()
    {
        cout << "Target::Request" << endl;
    }
};

class Adaptee
{
public:
    void Request()
    {
        cout << "Adaptee::Request" << endl;
    }
};

//对象适配器
class Adapter : public Target
{
public:
    Adapter() : m_Adaptee(new Adaptee) {}

    ~Adapter()
    {
        delete m_Adaptee;
        m_Adaptee = NULL;
    }

    void Request()
    {
        m_Adaptee->Request();
    }

private:
    Adaptee* m_Adaptee;
};

// 类适配器
class Adapter2 : public Target, Adaptee
{
public:
    void Request()
    {
        Adaptee::Request();
    }
};

int main(int argc, char* argv[])
{
    Target* targetObj = new Adapter();
    targetObj->Request();

    delete targetObj;
    targetObj = NULL;

    return 0;
}
