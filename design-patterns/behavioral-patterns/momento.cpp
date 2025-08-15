#include <string>
#include <vector>
#include <iostream>
using namespace std;

//玩家主角相关的备忘录类
class FighterMemento
{
private:
    FighterMemento(int life, int magic, int attack) :m_life(life), m_magic(magic), m_attack(attack) {}

private:
    friend class Fighter; //友元类Fighter可以访问本类的私有成员函数
    int getLife() const { return m_life; }
    int getMagic() const { return m_magic; }
    int getAttack() const { return m_attack; }

    void setLife(int life) { m_life = life; }
    void setMagic(int magic) { m_magic = magic; }
    void setAttack(int attack) { m_attack = attack; }

private:
    int m_life;    //生命值
    int m_magic;   //魔法值
    int m_attack;  //攻击力
};

//玩家主角类
class Fighter
{
public:
    Fighter(int life, int magic, int attack) :m_life(life), m_magic(magic), m_attack(attack) {}

public:
    //将玩家数据写入备忘录（创建备忘录，并在其中存储了当前状态）
    FighterMemento* createMomento()
    {
        return new FighterMemento(m_life, m_magic, m_attack);
    }
    //从备忘录中恢复玩家数据
    void restoreMomento(FighterMemento* pfm)
    {
        m_life = pfm->getLife();
        m_magic = pfm->getMagic();
        m_attack = pfm->getAttack();
    }
    //为测试目的引入的接口，设置玩家的生命值为0（玩家死亡）
    void setToDead()
    {
        m_life = 0;
    }
    //用于输出一些信息
    void displayInfo()
    {
        cout << "玩家主角当前的生命值、魔法值、攻击力分别为：" << m_life << "," << m_magic << "," << m_attack << endl;
    }

private:
    int m_life;    //生命值
    int m_magic;   //魔法值
    int m_attack;  //攻击力
};

//管理类
class FCareTaker
{
public:
    //构造函数
    FCareTaker(FighterMemento* ptmpfm) :m_pfm(ptmpfm) {} //形参是指向备忘录对象的指针

    //获取指向备忘录对象的指针
    FighterMemento* getMemento()
    {
        return m_pfm;
    }
    //保存指向备忘录对象的指针
    void setMemento(FighterMemento* ptmpfm)
    {
        m_pfm = ptmpfm;
    }
private:
    FighterMemento* m_pfm; //指向备忘录对象的指针
};

//支持多个快照的管理类
class FCareTaker2
{
public:
    ~FCareTaker2()
    {
        for (auto iter = m_pfmContainer.begin(); iter != m_pfmContainer.end(); ++iter)
        {
            delete (*iter);
        } //end for
    }
    //保存指向备忘录对象的指针
    void setMemento(FighterMemento* ptmpfm)
    {
        m_pfmContainer.push_back(ptmpfm);
    }
    //获取指向备忘录对象的指针
    FighterMemento* getMemento(int index)
    {
        auto iter = m_pfmContainer.begin();
        for (int i = 0; i <= index; ++i)
        {
            if (i == index)
                return (*iter);
            else
                ++iter;
        } //end for
        return nullptr;
    }
private:
    //存储备忘录对象指针的容器
    vector<FighterMemento*> m_pfmContainer;  //#include <vector>
};

int main()
{

    Fighter* p_fighter = new Fighter(800, 200, 300);
    p_fighter->displayInfo();

    FCareTaker* pfcaretaker = new FCareTaker(p_fighter->createMomento());

    cout << "玩家主角与BOSS开始进行激烈的战斗------" << endl;
    p_fighter->setToDead();   
    p_fighter->displayInfo(); 

    cout << "玩家主角通过备忘录恢复自己的信息------" << endl;
    p_fighter->restoreMomento(pfcaretaker->getMemento());
    p_fighter->displayInfo(); 

    //（5）释放资源
    //delete p_fighterMemo;
    delete pfcaretaker->getMemento();
    delete pfcaretaker; //新增
    delete p_fighter;

    Fighter* p_fighter2 = new Fighter(800, 200, 300);
    FCareTaker2* pfcaretaker2 = new FCareTaker2();

    pfcaretaker2->setMemento(p_fighter2->createMomento()); // 第一次快照，生命值为800
    p_fighter2->setToDead(); // 改变玩家主角的生命值
    pfcaretaker2->setMemento(p_fighter2->createMomento()); // 第二次快照，生命值为0

    p_fighter2->displayInfo(); // 当前生命值为0
    cout << "玩家主角通过备忘录恢复自己的信息------" << endl;

    // 恢复第一次快照，生命值恢复为800
    p_fighter2->restoreMomento(pfcaretaker2->getMemento(0));
    p_fighter2->displayInfo(); // 玩家主角生命值应恢复为800

    // 释放资源
    delete p_fighter2;
    delete pfcaretaker2;

    return 0;
}
