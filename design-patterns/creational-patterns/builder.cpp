#include <iostream>
#include <string>
#include <vector>

using namespace std;

//1. 抽象产品类
class AbstractFood
{
public:
	virtual ~AbstractFood() {}
	virtual void add(string foodname, int price) = 0;
	virtual void show() = 0;
};

//2. 具体产品类
class Food :public AbstractFood
{
public:
	virtual ~Food() {}
	void add(string foodname, int price)
	{
		m_vec.emplace_back(make_pair(foodname, price));
	}
	void show()
	{
		int sum = 0;
		for (int i = 0; i < m_vec.size(); i++)
		{
			sum += m_vec[i].second;
			cout << m_vec[i].first << ": " << m_vec[i].second << "元" << endl;
		}
		cout << "总计: " << sum << "元" << endl;
	}
private:
	vector<pair<string, int>> m_vec;
};

//3. 抽象建造者
class AbstractBuilder
{
public:
	AbstractBuilder()
		:food(new Food) {
	}
	virtual ~AbstractBuilder() {}
	virtual void BuildFood1() = 0;
	virtual void BuildFood2() = 0;
	virtual void BuildFood3() = 0;
	AbstractFood* getFood()
	{
		return food;
	}
protected:
	AbstractFood* food;
};

//4.1 具体建造者A
class Meal_1 :public AbstractBuilder
{
public:
	~Meal_1() {}
	void BuildFood1()
	{
		food->add("香辣鸡腿堡", 10);
	}
	void BuildFood2()
	{
		food->add("薯条", 5);
	}
	void BuildFood3()
	{
		food->add("可乐", 3);
	}
};

//4.2 具体建造者B
class Meal_2 :public AbstractBuilder
{
public:
	~Meal_2() {}
	void BuildFood1()
	{
		food->add("墨西哥鸡肉卷", 10);
	}
	void BuildFood2()
	{
		food->add("鸡块", 5);
	}
	void BuildFood3()
	{
		food->add("芬达", 3);
	}
};

//5. 指挥者： 最后上菜
class Director
{
public:
	~Director() {}
	Director(AbstractBuilder* foodBuilder)
		:fooder(foodBuilder) {
	}
	AbstractFood* createFood()
	{
		fooder->BuildFood1();
		fooder->BuildFood2();
		fooder->BuildFood3();
		return fooder->getFood();
	}
private:
	AbstractBuilder* fooder;
};

void menu()
{
	cout << "-----------欢迎光临汉堡店：-----------" << endl;
	cout << "-------1. 香辣鸡肉汉堡 + 薯条 + 可乐" << endl;
	cout << "-------2. 墨西哥鸡肉卷 + 鸡块 + 芬达" << endl;
	cout << "------------------------------------" << endl;
}
int main()
{
	unique_ptr<AbstractBuilder> fooder1(new Meal_1);
	unique_ptr<Director> pD1(new Director(fooder1.get()));
	unique_ptr<AbstractFood> food1((pD1.get()->createFood()));

	unique_ptr<AbstractBuilder> fooder2(new Meal_2);
	unique_ptr<Director> pD2(new Director{ fooder2.get() });
	unique_ptr<AbstractFood> food2(pD2.get()->createFood());

	int choice{};
	menu();
	cout << "您的选择：";
	cin >> choice;
	switch (choice)
	{
	case 1:
		food1->show();
		break;
	case 2:
		food2->show();
		break;
	}
	return 0;
}
