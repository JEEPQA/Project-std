// test cerr prudu
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	cout << "welcome in console cout" << std::endl;
	cerr << "welcome in console error" << std::endl;//cerr 是非缓冲的，错误信息会立即显示在屏幕上
	return 0;
}
