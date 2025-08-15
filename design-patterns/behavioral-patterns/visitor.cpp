#include <iostream>
using namespace std;

class A;
class B;
class C;
class D;
class E;

class Base {
public:
    class IVisitor {
    public:
        virtual void visit(A*) = 0;
        virtual void visit(B*) = 0;
        virtual void visit(C*) = 0;
        virtual void visit(D*) = 0;
        virtual void visit(E*) = 0;

    };
    virtual void say() = 0;
    virtual void accept(IVisitor*) = 0;
    virtual ~Base() {}
};

class A : public Base {
public:
    void say() override {
        cout << "Class A" << endl;
    }
    void accept(IVisitor* vis) override {
        vis->visit(this); // 匹配数据操作类中的重载函数
        return;
    }
};

class B : public Base {
public:
    void say() override {
        cout << "Class B" << endl;
    }
    void accept(IVisitor* vis) override {
        vis->visit(this);
        return;
    }
};

class C : public Base {
public:
    void say() override {
        cout << "Class C" << endl;
    }
    void accept(IVisitor* vis) override {
        vis->visit(this);
        return;
    }
};

class D : public Base {
public:
    void say() override {
        cout << "Class D" << endl;
    }
    void accept(IVisitor* vis) override {
        vis->visit(this);
        return;
    }
};

class E : public Base {
public:
    void say() override {
        cout << "Class E" << endl;
    }
    void accept(IVisitor* vis) override {
        vis->visit(this);
        return;
    }
};

class calcVisitor : public Base::IVisitor { // 数据操作类 
public:
    calcVisitor(int& x) : x(x) {}
    void visit(A* obj) override {
        x *= 2;
    }
    void visit(B* obj) override {
        x -= 3;
    }
    void visit(C* obj) override {
        x += 6;
    }
    void visit(D* obj) override {
        x /= 3;
    }
    void visit(E* obj) override {
        x -= 2;
    }
private:
    int& x;
};

int main() { // 访问者模式实现, 类型->功能(补充知识: 解决新增功能维护难——通过让编译器提示改代码)
    srand(time(0));
    Base* p[5] = { nullptr };
    for (int i = 0; i < 5; ++i) { // 第一次类指针类型转换
        switch (rand() % 5) {
        case 0: p[i] = new A(); break;
        case 1: p[i] = new B(); break;
        case 2: p[i] = new C(); break;
        case 3: p[i] = new D(); break;
        case 4: p[i] = new E(); break;
        }
    }
    int x = 1;
    calcVisitor vis(x);
    for (int i = 0, pre = x; i < 5; ++i) {
        p[i]->accept(&vis); // 第二次类指针类型转换
        p[i]->say();
        cout << pre << " -> " << x << endl;
        pre = x;
    }
    return 0;
}
