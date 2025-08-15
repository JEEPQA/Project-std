#include <memory>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// 系列产品1：汽车
class Car
{
public:
	Car(string name) :_name(name) {}
	virtual void show() = 0;
protected:
	string _name;
};
class Bmw :public Car
{
public:
	Bmw(string name) :Car(name) {}
	void show()
	{
		cout << "获取了一辆宝马汽车：" << _name << endl;
	}
};
class Audi :public Car
{
public:
	Audi(string name) :Car(name) {}
	void show()
	{
		cout << "获取了一辆奥迪汽车：" << _name << endl;
	}
};

// 系列产品2：车灯
class Light
{
public:
	virtual void show() = 0;
};
class BmwLight : public Light
{
public:
	void show() { cout << "BMW light!" << endl; }
};
class AudiLight : public Light
{
public:
	void show() { cout << "Audi light!" << endl; }
};



// 产品枚举
enum Cartype
{
	BMW, AUDI
};
// 简单工厂类
class SimpleFactory
{
public:
	// 用户想要创建一个对象，只需要知道名称就可以了
	Car* createCar(Cartype ct)
	{
		switch (ct)
		{
		case BMW:
			return new Bmw("x6");
		case AUDI:
			return new Audi("a8");
		default:
			cerr << "传入参数错误：" << ct << endl;
		}
		return nullptr;
	}
};

// 基类（包含纯虚函数，不能实例化对象）
class IFactory
{
public:
	virtual Car* createCar(string name) = 0; // 工厂方法 创建汽车
	virtual Light* createCarLight() = 0; // 工厂方法 创建汽车关联的产品，车灯
};
// 宝马汽车工厂，负责生产宝马汽车
class BmwFac : public IFactory
{
public:
	Car* createCar(string name)
	{
		return new Bmw(name);
	}
	Light* createCarLight()
	{
		return new BmwLight();
	}
};
// 奥迪汽车工厂，负责生产奥迪汽车
class AudiFac :public IFactory
{
public:
	Car* createCar(string name)
	{
		return new Audi(name);
	}
	Light* createCarLight()
	{
		return new AudiLight();
	}
};




int FactoryTest()
{
	unique_ptr<SimpleFactory> fac(new SimpleFactory());
	unique_ptr<Car> p1(fac->createCar(BMW));
	unique_ptr<Car> p2(fac->createCar(AUDI));


	unique_ptr<IFactory> bmwfty(new BmwFac());
	unique_ptr<IFactory> audifty(new AudiFac());
	unique_ptr<Car> p3(bmwfty->createCar("X6"));
	unique_ptr<Car> p4(audifty->createCar("A8"));
	unique_ptr<Light> p5(bmwfty->createCarLight());
	unique_ptr<Light> p6(audifty->createCarLight());

	p1->show();
	p2->show();
	p3->show();
	p4->show();
	p5->show();
	p6->show();

	return 0;
}