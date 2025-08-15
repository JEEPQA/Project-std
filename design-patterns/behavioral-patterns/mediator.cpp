#include <vector>
#include <iostream>
#include <list>
#include <string>
using namespace std;

class Role //抽象角色
{
protected:
	string name;
	string office;
	Mediator* mediator;
public:
	Role(string name, string office, Mediator* mediator)
	{
		this->name = name;
		this->office = office;
		this->mediator = mediator;
	}
	string get_name()
	{
		return this->name;
	}
	string get_office()
	{
		return this->office;
	}
	virtual void match(Role* role) = 0;
};

class Mediator //抽象中介者
{
public:
	virtual void match() = 0;
	virtual void set_hr(Role* hr) = 0;
	virtual void set_student(Role* student) = 0;
};

class Boos : public Mediator //BOOS直聘平台
{
private:
	Role* hr;
	Role* student;
public:
	virtual void set_hr(Role* hr)
	{
		this->hr = hr;
	}
	virtual void set_student(Role* student)
	{
		this->student = student;
	}
	virtual void match()
	{
		cout << "=========================" << endl;
		cout << hr->get_name() << " 提供职位：" << hr->get_office() << endl;
		cout << student->get_name() << " 需求职位：" << student->get_office() << endl;
		if (hr->get_office() == student->get_office())
		{
			cout << "***匹配成功***" << endl;
		}
		else
		{
			cout << "***匹配失败***" << endl;
		}
		cout << "=========================" << endl;
	}
};

class Student : public Role
{
public:
	Student(string name, string office, Mediator* mediator) : Role(name, office, mediator) {};
	virtual void match(Role* role)
	{
		mediator->set_hr(role);
		mediator->set_student(this);
		mediator->match();
	}
};

class HR : public Role
{
public:
	HR(string name, string office, Mediator* mediator) : Role(name, office, mediator) {};
	virtual void match(Role* role)
	{
		mediator->set_hr(this);
		mediator->set_student(role);
		mediator->match();
	}
};

int main()
{

	Mediator* medi = new Boos;

	Role* hr = new HR("七总", "C++", medi);
	Role* stu1 = new Student("小明", "Java", medi);
	Role*  stu2 = new Student("小红", "C++", medi);

	hr->match(stu1);
	hr->match(stu2);

	delete stu2;
	delete stu1;
	delete hr;

	return 0;
}