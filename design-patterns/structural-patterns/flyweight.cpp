#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

class IFlyweight {
public:
    virtual ~IFlyweight() {}
    virtual void operation(const std::string& extrinsicState) const = 0;
};

class ConcreteFlyweight : public IFlyweight {
private:
    std::string intrinsicState;
public:
    ConcreteFlyweight(const std::string& state) : intrinsicState(state) {}
    void operation(const std::string& extrinsicState) const override 
    {
        std::cout << "Concrete Flyweight: Internal State = " << intrinsicState
            << ", Extrinsic State = " << extrinsicState << std::endl;
    }
};

class FlyweightFactory 
{
private:
    std::map<std::string, std::shared_ptr<IFlyweight>> flyweights;
public:
    std::shared_ptr<IFlyweight> getFlyweight(const std::string& key) 
    {
        if (flyweights.count(key) == 0) 
        {
            flyweights[key] = std::make_shared<ConcreteFlyweight>(key);
        }
        return flyweights[key];
    }
};

int main() 
{
    FlyweightFactory factory;
    std::vector<std::string> extrinsicStates = { "state1", "state2", "state2" };
    for (const auto& state : extrinsicStates) 
    {
        auto flyweight = factory.getFlyweight("sharedState");
        flyweight->operation(state);
    }
    return 0;
}