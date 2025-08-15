#pragma once
#include <memory>
#include <mutex>

class MyInterface
{
public:
    MyInterface();
    ~MyInterface();

    // 拷贝构造和赋值操作符（线程安全版本）
    MyInterface(const MyInterface& other);
    MyInterface& operator=(const MyInterface& other);

    int publicApi1();
    int publicApi2(int i);

    void batchOperation(int count);
    int getState() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl;  // 使用smart pointer代替原始指针
    mutable std::mutex mutex_;   // 保护impl访问的互斥锁
};
