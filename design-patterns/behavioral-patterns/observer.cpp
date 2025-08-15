#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

// 前向声明
class Observer;

// 主题接口
class Subject {
public:
    virtual ~Subject() = default;
    virtual void attach(std::shared_ptr<Observer> observer) = 0;
    virtual void detach(std::shared_ptr<Observer> observer) = 0;
    virtual void notify() = 0;
protected:
    std::vector<std::shared_ptr<Observer>> observers;
};

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(double price) = 0;
};

// 具体主题：股票
class Stock : public Subject {
private:
    std::string symbol;
    double price;
public:
    Stock(const std::string& sym, double p) : symbol(sym), price(p) {}

    void attach(std::shared_ptr<Observer> observer) override {
        observers.push_back(observer);
        std::cout << "Observer attached: " << symbol << std::endl;
    }

    void detach(std::shared_ptr<Observer> observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        std::cout << "Observer detached: " << symbol << std::endl;
    }

    void notify() override {
        std::cout << "Notifying observers about price change in " << symbol << std::endl;
        for (auto& observer : observers) {
            observer->update(price);
        }
    }

    void setPrice(double newPrice) {
        if (price != newPrice) {
            price = newPrice;
            notify();
        }
    }

    double getPrice() const {
        return price;
    }

    std::string getSymbol() const {
        return symbol;
    }
};

// 具体观察者：股票投资者
class Investor : public Observer {
private:
    std::string name;

public:
    Investor(const std::string& n) : name(n) {}

    void update(double price) override {
        std::cout << name << " received price update: " << price << std::endl;
    }
};

void GetStockPrice(std::vector<std::shared_ptr<Stock>>& StockTable, float Price)
{
    for (auto& stock: StockTable)
    {
        stock->setPrice(Price);
    }
}

// 客户端代码
int main() {
    // 创建股票（主题）
    auto appleStock = std::make_shared<Stock>("AAPL", 150.0);
    auto googleStock = std::make_shared<Stock>("GOOGL", 2000.0);

    // 创建投资者（观察者）
    auto investor1 = std::make_shared<Investor>("Alice");
    auto investor2 = std::make_shared<Investor>("Bob");

    // 注册观察者到主题
    appleStock->attach(investor1);
    appleStock->attach(investor2);
    googleStock->attach(investor1);

    std::vector<std::shared_ptr<Stock>> StockTable;
    StockTable.push_back(appleStock);
    StockTable.push_back(googleStock);

    // 股票价格变化
    GetStockPrice(StockTable,155.5);

    // 投资者2不再关注苹果股票
    appleStock->detach(investor2);

    // 苹果股票再次变化
    GetStockPrice(StockTable,166.6);

    return 0;
}
