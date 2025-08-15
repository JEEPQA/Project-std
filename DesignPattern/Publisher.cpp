// Event.h
#pragma once
#include <iostream>
#include <functional>
#include <thread>
#include <vector>

template <typename... Args>
class Event {
public:
    using Callback = std::function<void(Args...)>;

    void subscribe(Callback callback) {
        callbacks_.emplace_back(std::move(callback));
    }

    void notify(Args... args) const {
        for (const auto& callback : callbacks_) {
            callback(args...);
        }
    }

private:
    std::vector<Callback> callbacks_;
};

class Publisher {
public:
    Event<int> onDataReceived;

    void sendData(int data) {
        // 假设在这里进行一些数据处理
        onDataReceived.notify(data);
    }
};

class Subscriber {
public:
    void processData(int data) {
        std::cout << "Received data: " << data << std::endl;
    }
    void processData2(int data) {
        std::cout << "Received data2: " << data << std::endl;
    }
};


int Publishertest() {
    Publisher publisher;
    Subscriber subscriber;

    // 订阅者订阅发布者的事件
    publisher.onDataReceived.subscribe([&subscriber](int data) {
        subscriber.processData(data);
        });
    publisher.onDataReceived.subscribe([&subscriber](int data) {
        subscriber.processData2(data);
        });

    // 模拟数据发送
    for (int i = 1; i <= 5; ++i) {
        publisher.sendData(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}