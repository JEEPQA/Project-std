#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
using std::make_unique;

// Widget类的声明（头文件部分）
class Widget {
public:
    // 构造函数和析构函数
    Widget();
    Widget(const string& name, int value);
    ~Widget();
    
    // 拷贝构造函数和拷贝赋值操作符
    Widget(const Widget& other);
    Widget& operator=(const Widget& other);
    
    // 移动构造函数和移动赋值操作符（C++11/14）
    Widget(Widget&& other) noexcept;
    Widget& operator=(Widget&& other) noexcept;
    
    // 公共接口
    void setName(const string& name);
    void setValue(int value);
    string getName() const;
    int getValue() const;
    void display() const;
    void processData();

private:
    // Pimpl idiom: 指向实现的指针
    struct Impl;
    unique_ptr<Impl> pImpl;
};

// Widget的私有实现结构体
struct Widget::Impl {
    string name;
    int value;
    bool isProcessed;
    
    // 构造函数
    Impl() : name("默认"), value(0), isProcessed(false) {}
    Impl(const string& n, int v) : name(n), value(v), isProcessed(false) {}
    
    // 一些复杂的私有方法
    void internalProcess() {
        cout << "执行内部处理逻辑..." << endl;
        value *= 2;
        isProcessed = true;
    }
    
    void complexCalculation() {
        cout << "执行复杂计算..." << endl;
        // 模拟复杂计算
        for (int i = 0; i < 1000; ++i) {
            value = (value + i) % 1000;
        }
    }
};

// Widget类的实现
Widget::Widget() : pImpl(make_unique<Impl>()) {
    cout << "Widget默认构造函数" << endl;
}

Widget::Widget(const string& name, int value) 
    : pImpl(make_unique<Impl>(name, value)) {
    cout << "Widget参数化构造函数: " << name << endl;
}

Widget::~Widget() {
    cout << "Widget析构函数" << endl;
    // unique_ptr自动管理内存，无需手动delete
}

// 拷贝构造函数
Widget::Widget(const Widget& other) 
    : pImpl(make_unique<Impl>(*other.pImpl)) {
    cout << "Widget拷贝构造函数" << endl;
}

// 拷贝赋值操作符
Widget& Widget::operator=(const Widget& other) {
    if (this != &other) {
        *pImpl = *other.pImpl;
        cout << "Widget拷贝赋值操作符" << endl;
    }
    return *this;
}

// 移动构造函数
Widget::Widget(Widget&& other) noexcept 
    : pImpl(std::move(other.pImpl)) {
    cout << "Widget移动构造函数" << endl;
}

// 移动赋值操作符
Widget& Widget::operator=(Widget&& other) noexcept {
    if (this != &other) {
        pImpl = std::move(other.pImpl);
        cout << "Widget移动赋值操作符" << endl;
    }
    return *this;
}

// 公共接口实现
void Widget::setName(const string& name) {
    pImpl->name = name;
}

void Widget::setValue(int value) {
    pImpl->value = value;
}

string Widget::getName() const {
    return pImpl->name;
}

int Widget::getValue() const {
    return pImpl->value;
}

void Widget::display() const {
    cout << "Widget信息 - 名称: " << pImpl->name 
         << ", 值: " << pImpl->value 
         << ", 已处理: " << (pImpl->isProcessed ? "是" : "否") << endl;
}

void Widget::processData() {
    cout << "开始处理数据..." << endl;
    pImpl->internalProcess();
    pImpl->complexCalculation();
    cout << "数据处理完成!" << endl;
}

// 使用示例
int main() {
    cout << "=== Pimpl设计模式示例 ===" << endl;
    
    // 创建Widget对象
    Widget w1;
    w1.setName("第一个Widget");
    w1.setValue(100);
    w1.display();
    
    // 使用参数化构造函数
    Widget w2("第二个Widget", 200);
    w2.display();
    
    // 处理数据
    cout << "\n处理第一个Widget的数据:" << endl;
    w1.processData();
    w1.display();
    
    // 测试拷贝构造
    cout << "\n测试拷贝构造:" << endl;
    Widget w3 = w2;
    w3.display();
    
    // 测试赋值操作
    cout << "\n测试赋值操作:" << endl;
    Widget w4;
    w4 = w1;
    w4.display();
    
    // 测试移动语义
    cout << "\n测试移动构造:" << endl;
    Widget w5 = std::move(w3);
    w5.display();
    
    cout << "\n程序结束" << endl;
    return 0;
}