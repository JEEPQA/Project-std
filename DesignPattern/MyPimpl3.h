#pragma once
#include <memory>
#include <mutex>

class MyInterface
{
public:
    MyInterface();
    explicit MyInterface(int initialValue);
    ~MyInterface();

    // Rule of Five: 拷贝构造、拷贝赋值、移动构造、移动赋值
    MyInterface(const MyInterface& other);
    MyInterface& operator=(const MyInterface& other);
    MyInterface(MyInterface&& other) noexcept;
    MyInterface& operator=(MyInterface&& other) noexcept;

    int publicApi1();
    int publicApi2(int i);
    void batchOperation(int count);
    int getState() const;

    // 资源管理相关方法
    void reset();
    bool isValid() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl;
    mutable std::mutex mutex_;
};