#include <iostream>
#include <vector>

/**
* @brief 简单概述
* @brief 形状类
*/
class Shape
{
public:
	Shape()
	{
		line_count++;
		this->id = line_count;
	}
	~Shape() {}
	virtual void AddShape(Shape*) {}
	virtual bool RemoveShape(Shape*) { return false; }
	virtual void ShowInfo() = 0;

	int id;

protected:
	std::vector<Shape*> vec_line;

private:
	static int line_count;
};
int Shape::line_count = 0;

/**
* @brief 简单概述
* @brief 三角形类
*/
class  Triangle :public Shape
{
public:
	Triangle() :Shape() {}
	~Triangle() {}

	virtual void AddShape(Shape* pShape)
	{
		vec_line.push_back(pShape);
	}
	virtual bool RemoveShape(Shape* pShape)
	{
		for (auto itr = vec_line.begin(); itr != vec_line.end(); itr++)
		{
			if (pShape->id == (*itr)->id)
			{
				vec_line.erase(itr);
				return true;
			}
		}
		return false;
	}
	virtual void ShowInfo()
	{
		std::cout << "ShowInfo in Triangle!" << std::endl;
		for (auto itr : vec_line)
		{
			itr->ShowInfo();
		}
	}
};

/**
* @brief 简单概述
* @brief 四边形类
*/
class Rectamgle :public Shape
{
public:
	Rectamgle() :Shape() {}
	~Rectamgle() {}

	virtual void AddShape(Shape* pShape)
	{
		vec_line.push_back(pShape);
	}
	virtual bool RemoveShape(Shape* pShape)
	{
		for (auto itr = vec_line.begin(); itr != vec_line.end(); itr++)
		{
			if (pShape->id == (*itr)->id)
			{
				vec_line.erase(itr);
				return true;
			}
		}
		return false;
	}
	virtual void ShowInfo()
	{
		std::cout << "ShowInfo in Rectamgle!" << std::endl;
		for (auto itr : vec_line)
		{
			itr->ShowInfo();
		}
	}
};

/**
* @brief 简单概述
* @brief 多边形类
*/
class Polygon :public Shape
{
public:
	Polygon() :Shape() {}
	~Polygon() {}

	virtual void AddShape(Shape* pShape)
	{
		vec_line.push_back(pShape);
	}
	virtual bool RemoveShape(Shape* pShape)
	{
		for (auto itr = vec_line.begin(); itr != vec_line.end(); itr++)
		{
			if (pShape->id == (*itr)->id)
			{
				vec_line.erase(itr);
				return true;
			}
		}
		return false;
	}
	virtual void ShowInfo()
	{
		std::cout << "ShowInfo in Polygon!" << std::endl;
		for (auto itr : vec_line)
		{
			itr->ShowInfo();
		}
	}
};

/**
* @brief 简单概述
* @brief 圆形类
* @brief 圆形类没有实现AddShape()和RemoveShape()函数
*/
class Circle :public Shape
{
public:
	Circle() :Shape() {}
	~Circle() {}
	virtual void ShowInfo()
	{
		std::cout << "ShowInfo in Circle!" << std::endl;
		for (auto itr : vec_line)
		{
			itr->ShowInfo();
		}
	}
};

int main(int argc, char* argv[])
{
	Triangle* triangle = new Triangle();
	Rectamgle* rectangle = new Rectamgle();
	Polygon* poly = new Polygon();

	poly->AddShape(triangle);
	poly->AddShape(rectangle);
	poly->ShowInfo();

	poly->RemoveShape(triangle);
	poly->ShowInfo();

	Circle* circle = new Circle();
	/**
	* @brief 简单概述
	* @brief 由于Circle没有实现AddShape函数
	* @brief 所以执行AddShape函数什么都不做.
	*/
	circle->AddShape(triangle);
	circle->AddShape(rectangle);
	circle->ShowInfo();

	std::cout << std::endl;
	for (size_t i = 0; i < 1; i++)
	{
		std::cout << "* ** *** **** ***** ****** *******" << std::endl;
		std::cout << "** *** **** ***** ****** ******* *" << std::endl;
		std::cout << "*** **** ***** ****** ******* * **" << std::endl;
		std::cout << "**** ***** ****** ******* * ** ***" << std::endl;
		std::cout << "**** **** **** <==> **** **** ****" << std::endl;
		std::cout << "***** ****** ******* * ** *** ****" << std::endl;
		std::cout << "****** ******* * ** *** **** *****" << std::endl;
		std::cout << "******* * ** *** **** ***** ******" << std::endl;
		std::cout << "* ** *** **** ***** ****** *******" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "<<<<<<<<== Hello! You are a good man! ==>>>>>>>" << std::endl;
	return 0;
}