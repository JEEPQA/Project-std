#include <string>
#include <iostream>

using std::string;
using std::cout;


template <typename T>
struct foo
{
	operator T() {
		cout << "operator T()\n";
		return T("5");
	}
	operator T() const {
		cout << "operator T() const\n";
		return T("5");
	}

};

bool operator==(foo<string> const & lhs, string const & rhs)
{
    return static_cast<string>(lhs) == rhs;
}

bool operator==(foo<string>& lhs, string const& rhs)
{
	return static_cast<string>(lhs) == rhs;
}

foo<string> getfoo() 
{
	return foo<string>();
}

int main(int argc, char * argv[])
{
	foo<string> f;
	string a = foo<string>();
	getfoo() == "5";
	return 0;
}

