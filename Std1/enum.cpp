#include <iostream>
using std::cout;
enum foo {one, two, three};

struct goo {
	enum hoo {four, five, six};
};

void func(goo::hoo type) {}

struct foo2
{
	enum prop_e { max_vertex_distance };
	void property(int v) { cout << "property(int)\n"; }
	void property(prop_e prop) { cout << "property(prop_e)\n"; }
};


int main(int argc, char * argv[])
{
	foo type;
	type = two;

	goo::hoo type2;
	type2 = goo::five;

	func(type2);

	int v = 0;
	foo2 f;
	f.property(v);  // vol¨¢ property(int)
	f.property(foo2::max_vertex_distance);  // vol¨¢ property(prop_e)
	f.property(0);  // property(int)
	f.property(10);  // property(int)

	return 0;
}

