#include <memory>
#include <iostream>

//
//职责链模式(Chain of Responsibility Pattern)
//

//定义处理请示的接口
//抽象类
class Handler {
protected:
    std::shared_ptr<Handler> m_pNextHandler;
public:
    Handler() : m_pNextHandler(nullptr) {}

    Handler(const std::shared_ptr<Handler> pNextHandler) : m_pNextHandler(pNextHandler) {}

    virtual ~Handler() = default;

    virtual void HandleRequest(int request) = 0;

};

//链上的第1个节点
class ConcreteHandler1 : public Handler {
public:
    ConcreteHandler1() {}

    ConcreteHandler1(const std::shared_ptr<Handler> pNextHandler) : Handler(pNextHandler) {}

    void HandleRequest(int request) override {
        std::cout << "节点1收到请求" << std::endl;
        if (request >= 0 && request < 10)
            std::cout << "节点1处理请求 " << request << std::endl;
        else if (m_pNextHandler) {
            std::cout << "我是节点1,这个事情我处理不了,得交给上一级" << std::endl;
            m_pNextHandler->HandleRequest(request);
        }

    }
};

//链上的第2个节点
class ConcreteHandler2 : public Handler {
public:
    ConcreteHandler2() {}

    ConcreteHandler2(const std::shared_ptr<Handler> pNextHandler) : Handler(pNextHandler) {}

    void HandleRequest(int request) override {
        std::cout << "节点2收到请求" << std::endl;
        if (request >= 10 && request < 20)
            std::cout << "节点2处理请求 " << request << std::endl;
        else if (m_pNextHandler) {
            std::cout << "我是节点2,这个事情我处理不了,得交给上一级" << std::endl;
            m_pNextHandler->HandleRequest(request);
        }

    }
};

//链上的第3个节点
class ConcreteHandler3 : public Handler {
public:
    ConcreteHandler3() {}

    ConcreteHandler3(const std::shared_ptr<Handler> pNextHandler) : Handler(pNextHandler) {}

    void HandleRequest(int request) override {
        std::cout << "节点3收到请求" << std::endl;
        if (request >= 20)
            std::cout << "节点3处理请求 " << request << std::endl;
        else if (m_pNextHandler) {
            std::cout << "我是节点3,这个事情我处理不了,得交给上一级" << std::endl;
            m_pNextHandler->HandleRequest(request);

        }
        else {
            std::cout << "我是节点3,这个事情没有节点可以处理" << std::endl;
        }
    }
};

int main() {
    std::shared_ptr<Handler> pHandle_3 = std::make_shared<ConcreteHandler3>();
    std::shared_ptr<Handler> pHandle_2 = std::make_shared<ConcreteHandler2>(pHandle_3);
    std::shared_ptr<Handler> pHandle_1 = std::make_shared<ConcreteHandler1>(pHandle_2);

    int request[] = { 2, 5, 14, 44, 22, 15, 23, 12, -5 };

    //客户端将请求交给首节点，然后请求即可在链上传递
    for (auto i : request) {
        pHandle_1->HandleRequest(i);
        std::cout << std::string(30, '-') << std::endl;
    }

    return 0;
}
