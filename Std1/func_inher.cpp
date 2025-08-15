#include <iostream>
using std::cout;

struct foo
{
	virtual void b() {cout << "foo::b()\n";}
	void a() { b(); }
};

struct goo : public foo
{
	void b() override {cout << "goo::b()\n";}
};

int main(int argc, char * argv[])
{
	foo* f = new goo();
	f->a();
	return 0;
}
