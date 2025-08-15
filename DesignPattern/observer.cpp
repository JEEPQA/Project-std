//#include <memory>
#include <vector>
#include <iostream>
#include <list>

//
//观察者模式
//

class Observer;
//抽象被观察者
class Subject {
public:

    virtual ~Subject() = default;

    virtual void Attach(const std::shared_ptr<Observer> pObserver) = 0;

    virtual void Detach(const std::shared_ptr<Observer> pObserver) = 0;

    virtual void Notify() = 0;

    //virtual void SetState(int state) = 0;

protected:
    std::list<std::shared_ptr<Observer>> m_pObserver_list;
};

//抽象观察者
class Observer {
public:
    virtual ~Observer() = default;

    Observer( const std::string& name = "unknown"): m_strName(name) {}

    virtual void Update(int State) = 0;

    virtual const std::string& name() { return m_strName; }

protected:
    std::string m_strName;
};

//具体被观察者
class ConcreteSubject : public Subject {
private:
    int m_nState;
public:
    void Attach(const std::shared_ptr<Observer> pObserver) override {
        auto iter = std::find(m_pObserver_list.begin(), m_pObserver_list.end(), pObserver);
        if (iter == m_pObserver_list.end()) {
            std::cout << "Attach observer" << pObserver->name() << std::endl;
            m_pObserver_list.emplace_back(pObserver);
        }
    }

    void Detach(const std::shared_ptr<Observer> pObserver) override {
        std::cout << "Detach observer" << pObserver->name() << std::endl;
        m_pObserver_list.remove(pObserver);
    }

    //循环通知所有观察者
    void Notify() override {
        auto it = m_pObserver_list.begin();
        while (it != m_pObserver_list.end()) {
            (*it++)->Update(m_nState);
        }
    }

    void SetState(int state) {
        std::cout << "Subject updated !" << std::endl;
        m_nState = state;
        Notify();
    }
};


//具体观察者1
class Observer1 : public Observer {
public:
    Observer1(const std::string& name = "unknown"): Observer(name) {}

    void Update(int State) override {
        std::cout << "Observer1_" << m_strName << " get the update.New state is: "
            << State << std::endl;
    }
};

//具体观察者2
class Observer2 : public Observer {
public:
    Observer2(const std::string& name = "unknown"): Observer(name) {}

    void Update(int State) override {
        std::cout << "Observer2_" << m_strName << " get the update.New state is: "
            << State << std::endl;
    }
};

//观察者 事件驱动的另种表达
int main() {

    std::shared_ptr<ConcreteSubject> pSubject = std::make_shared<ConcreteSubject>();// 创建被观察者

    // 创建观察者
    std::shared_ptr<Observer> pObserver1_1 = std::make_shared<Observer1>("1");
    std::shared_ptr<Observer> pObserver1_2 = std::make_shared<Observer1>("2");
    std::shared_ptr<Observer> pObserver1_3 = std::make_shared<Observer1>("3");

    std::shared_ptr<Observer> pObserver2_4 = std::make_shared<Observer2>("4");
    std::shared_ptr<Observer> pObserver2_5 = std::make_shared<Observer2>("5");
    std::shared_ptr<Observer> pObserver2_6 = std::make_shared<Observer2>("6");

    // 注册观察者
    pSubject->Attach(pObserver1_1);
    pSubject->Attach(pObserver1_2);
    pSubject->Attach(pObserver1_3);
    pSubject->Attach(pObserver2_4);
    pSubject->Attach(pObserver2_5);
    pSubject->Attach(pObserver2_6);

    pSubject->SetState(2);// 改变状态

    std::cout << std::string(50, '-') << std::endl;

    // 注销观察者
    pSubject->Detach(pObserver1_1);
    pSubject->Detach(pObserver1_2);

    pSubject->SetState(3);

    return 0;

}
