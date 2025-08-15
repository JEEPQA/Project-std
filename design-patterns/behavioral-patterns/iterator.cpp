#include <string>
#include <vector>
#include <iostream>

/**
* @brief 简单概述
* @brief 迭代器基类
*/
class Iterator
{
public:
	Iterator() {}
	~Iterator() {}

	virtual std::string First() = 0;
	virtual std::string Next() = 0;
	virtual std::string Current() = 0;
	virtual bool IsEnd() = 0;
};

/**
* @brief 简单概述
* @brief 对象基类
*/
class Object
{
public:
	Object() {}
	~Object() {}

	virtual Iterator* CreateIterator() = 0;
	virtual int Count() = 0;
	virtual void Push(std::string) = 0;
	virtual std::string Pop(int) = 0;
};

/**
* @brief 简单概述
* @brief 迭代器子类
*/
class SubIterator : public Iterator
{
public:
	SubIterator(Object* obj) :Iterator(), m_pObj(obj), m_Cur(0) {}
	~SubIterator() {}

	std::string First()
	{
		return m_pObj->Pop(0);
	}

	std::string Next()
	{
		std::string str;
		if (m_Cur < m_pObj->Count() - 1)
		{
			m_Cur++;
			str = m_pObj->Pop(m_Cur);
		}
		return str;
	}

	std::string Current()
	{
		return m_pObj->Pop(m_Cur);
	}

	bool IsEnd()
	{
		return (m_Cur == m_pObj->Count() - 1);
	}

private:
	Object* m_pObj;
	int m_Cur;
};

/**
* @brief 简单概述
* @brief 对象类
*/
class Children : public Object
{
public:
	Children() : Object(), m_pIterator(NULL) {}
	~Children() {}

	Iterator* CreateIterator()
	{
		if (m_pIterator == NULL)
			m_pIterator = new SubIterator(this);
		return m_pIterator;
	}

	int Count()
	{
		return vec_str.size();
	}

	void Push(std::string s)
	{
		vec_str.push_back(s);
	}

	std::string Pop(int i)
	{
		std::string str;
		if (i >= 0 && i < vec_str.size())
		{
			str = vec_str[i];
		}
		return str;
	}

private:
	std::vector<std::string> vec_str;
	Iterator* m_pIterator;
};

int main(int argc, char* argv[])
{
	Children child;
	child.Push("Tom");
	child.Push("John");
	child.Push("Json");

	Iterator* itr = child.CreateIterator();
	std::cout << "First man:" << itr->First() << std::endl;
	while (!itr->IsEnd())
	{
		std::cout << "Next man:" << itr->Next() << std::endl;
	}

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