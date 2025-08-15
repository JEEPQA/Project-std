#include "MyPimpl2.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>

using std::cout;
using std::endl;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using std::unique_ptr;
using std::make_unique;

// Impl类的定义 - 包含线程安全机制
class MyInterface::Impl {
public:
    Impl() : data1_(0), data2_(0), counter_(0) {
        cout << "Impl构造函数" << endl;
    }

    ~Impl() {
        cout << "Impl析构函数" << endl;
    }

    // 线程安全的方法
    int operation1() {
        lock_guard<mutex> lock(internal_mutex_);
        ++counter_;
        data1_ += 10;

        // 模拟一些耗时操作
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cout << "operation1执行，data1_=" << data1_
            << ", counter=" << counter_.load() << endl;
        return data1_;
    }

    int operation2(int value) {
        lock_guard<mutex> lock(internal_mutex_);
        data2_ += value;
        ++counter_;

        // 模拟复杂计算
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        cout << "operation2执行，输入=" << value
            << ", data2_=" << data2_
            << ", counter=" << counter_.load() << endl;
        return data2_;
    }

    void batchProcess(int count) {
        lock_guard<mutex> lock(internal_mutex_);
        for (int i = 0; i < count; ++i) {
            data1_ += i;
            data2_ -= i;
            ++counter_;
        }
        cout << "批量处理完成，处理了" << count << "项" << endl;
    }

    int getCurrentState() const {
        lock_guard<mutex> lock(internal_mutex_);
        return data1_ + data2_;
    }

    // 深拷贝方法（线程安全）
    std::unique_ptr<Impl> clone() const {
        lock_guard<mutex> lock(internal_mutex_);
        auto newImpl = make_unique<Impl>();
        newImpl->data1_ = data1_;
        newImpl->data2_ = data2_;
        newImpl->counter_ = counter_.load();
        return newImpl;
    }

private:
    int data1_;
    int data2_;
    std::atomic<int> counter_;           // 原子操作计数器
    mutable mutex internal_mutex_;       // 保护内部数据的互斥锁
};

// MyInterface的实现
MyInterface::MyInterface() : impl(make_unique<Impl>()) {
    cout << "MyInterface构造函数" << endl;
}

MyInterface::~MyInterface() {
    // 析构时需要确保没有其他线程在使用
    lock_guard<mutex> lock(mutex_);
    cout << "MyInterface析构函数" << endl;
    // unique_ptr自动清理
}

// 线程安全的拷贝构造函数
MyInterface::MyInterface(const MyInterface& other) {
    lock_guard<mutex> lock(other.mutex_);
    impl = other.impl->clone();
    cout << "MyInterface拷贝构造函数" << endl;
}

// 线程安全的赋值操作符
MyInterface& MyInterface::operator=(const MyInterface& other) {
    if (this != &other) {
        // 避免死锁：按地址顺序获取锁
        if (this < &other) {
            lock_guard<mutex> lock1(mutex_);
            lock_guard<mutex> lock2(other.mutex_);
            impl = other.impl->clone();
        }
        else {
            lock_guard<mutex> lock1(other.mutex_);
            lock_guard<mutex> lock2(mutex_);
            impl = other.impl->clone();
        }
        cout << "MyInterface赋值操作符" << endl;
    }
    return *this;
}

int MyInterface::publicApi1() {
    lock_guard<mutex> lock(mutex_);
    return impl->operation1();
}

int MyInterface::publicApi2(int i) {
    lock_guard<mutex> lock(mutex_);
    return impl->operation2(i);
}

void MyInterface::batchOperation(int count) {
    lock_guard<mutex> lock(mutex_);
    impl->batchProcess(count);
}

int MyInterface::getState() const {
    lock_guard<mutex> lock(mutex_);
    return impl->getCurrentState();
}

// 多线程测试示例
void workerThread(MyInterface& obj, int threadId, int iterations) {
    cout << "线程" << threadId << "开始工作" << endl;

    for (int i = 0; i < iterations; ++i) {
        if (i % 2 == 0) {
            obj.publicApi1();
        }
        else {
            obj.publicApi2(threadId * 10 + i);
        }

        // 偶尔执行批量操作
        if (i % 5 == 0) {
            obj.batchOperation(2);
        }
    }

    cout << "线程" << threadId << "完成工作，最终状态: "
        << obj.getState() << endl;
}

int main() {
    cout << "=== Pimpl多线程安全示例 ===" << endl;

    MyInterface obj;

    // 创建多个线程同时操作对象
    std::vector<std::thread> threads;
    const int numThreads = 4;
    const int iterations = 5;

    cout << "\n启动" << numThreads << "个线程进行并发测试..." << endl;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(workerThread, std::ref(obj), i + 1, iterations);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    cout << "\n所有线程完成，最终对象状态: " << obj.getState() << endl;

    // 测试拷贝构造的线程安全性
    cout << "\n测试拷贝构造..." << endl;
    MyInterface obj2 = obj;
    cout << "拷贝对象状态: " << obj2.getState() << endl;

    return 0;
}