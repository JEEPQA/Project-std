// clang -fsyntax-only -std=c++0x constmem.cpp
#include <memory>
#include <vector>
#include <iostream>

struct foo {
	void boundary() { std::cout << "foo boundary() " << "\n"; };
};

struct goo {
	goo() {
		_f.push_back(std::shared_ptr<foo>(new foo()));
	}

	void boundary() const {_f[0]->boundary();}
	std::vector<std::shared_ptr<foo>> _f;
};

int main()
{
	goo g;
	g.boundary();
	return 0;
}
