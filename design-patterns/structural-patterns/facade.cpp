#include <iostream>
#include <string>

using namespace std;

class SubSystem1
{
public:
	void DoSomething1()
	{
		cout << "DoSomething1" << endl;
	}
};

class SubSystem2
{
public:
	void DoSomething2()
	{
		cout << "DoSomething2" << endl;
	}
};

class SubSystem3
{
public:
	void DoSomething3()
	{
		cout << "DoSomething3" << endl;
	}
};

class Facade
{
public:
	void DoSomething()
	{
		m_a.DoSomething1();
		m_b.DoSomething2();
	}
private:
	SubSystem1 m_a;
	SubSystem2 m_b;
	SubSystem3 m_c;
};

int main()
{
	Facade face;
	face.DoSomething();
}