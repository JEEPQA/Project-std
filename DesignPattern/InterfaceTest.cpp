#include <cmath>
#include <iostream>

//
// 圆的接口类
//
class ICircle {
public:
    virtual ~ICircle() {};

    // 接口方法：面积
    virtual double area() = 0;
};

//
// 圆的具体实现类
//
class CircleImpl : public ICircle {

private:
    double radius;
public:
    CircleImpl(double radius);
    double area() override;
};

inline double pi() {
    return std::atan(1) * 4;
};

CircleImpl::CircleImpl(double _radius) : radius(_radius) {
};

double CircleImpl::area() {
    return pi() * radius * radius;
};

//
// 圆的创建工厂类
//
class CircleManager {
public:
    static ICircle* create(double radius);     // 创建circle实例
    static void destroy(ICircle* circlePtr);   // 销毁circle实例
};

ICircle* CircleManager::create(double radius) {
    ICircle* circlePtr = new CircleImpl(radius);
    //CircleImpl* circlePtr = new CircleImpl(radius);

    return circlePtr;
};

void CircleManager::destroy(ICircle* circlePtr) {
    delete circlePtr;
};


int Test3()
{
    ICircle* circlePtr = CircleManager::create(3);
    std::cout << circlePtr->area() << std::endl;
    CircleManager::destroy(circlePtr);

    system("pause");

    return 0;
}