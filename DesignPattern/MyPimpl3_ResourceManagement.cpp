#include "MyPimpl3.h"
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <exception>
#include <stdexcept>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::lock_guard;
using std::mutex;

// RAII资源管理辅助类
class ResourceManager {
public:
    ResourceManager(const std::string& name) : resource_name_(name) {
        cout << "获取资源: " << resource_name_ << endl;
    }
    
    ~ResourceManager() {
        cout << "释放资源: " << resource_name_ << endl;
    }
    
    // 禁用拷贝，只允许移动
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    ResourceManager(ResourceManager&& other) noexcept 
        : resource_name_(std::move(other.resource_name_)) {
        cout << "移动资源: " << resource_name_ << endl;
    }
    
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if (this != &other) {
            resource_name_ = std::move(other.resource_name_);
            cout << "移动赋值资源: " << resource_name_ << endl;
        }
        return *this;
    }

private:
    std::string resource_name_;
};

// Impl类的定义 - 强化资源管理
class MyInterface::Impl {
public:
    Impl() : data1_(0), data2_(0), counter_(0) {
        try {
            // 初始化各种资源
            resource1_ = make_unique<ResourceManager>("数据库连接");
            resource2_ = make_unique<ResourceManager>("文件句柄");
            buffer_ = make_unique<std::vector<int>>(1000, 0);  // 分配缓冲区
            
            cout << "Impl构造函数 - 所有资源初始化成功" << endl;
        }
        catch (const std::exception& e) {
            cout << "Impl构造失败: " << e.what() << endl;
            throw; // 重新抛出异常，确保对象构造失败
        }
    }
    
    explicit Impl(int initialValue) : Impl() {
        data1_ = initialValue;
        data2_ = initialValue * 2;
        cout << "Impl参数化构造函数，初始值: " << initialValue << endl;
    }

    ~Impl() {
        try {
            // 清理操作
            cleanup();
            cout << "Impl析构函数 - 资源清理完成" << endl;
        }
        catch (...) {
            // 析构函数中不应该抛出异常
            cout << "Impl析构函数 - 清理过程中发生异常" << endl;
        }
    }

    // 深拷贝构造函数
    Impl(const Impl& other) 
        : data1_(other.data1_), data2_(other.data2_), counter_(other.counter_.load()) {
        try {
            // 深拷贝所有资源
            resource1_ = make_unique<ResourceManager>("数据库连接(拷贝)");
            resource2_ = make_unique<ResourceManager>("文件句柄(拷贝)");
            buffer_ = make_unique<std::vector<int>>(*other.buffer_);
            
            cout << "Impl拷贝构造函数完成" << endl;
        }
        catch (const std::exception& e) {
            cout << "Impl拷贝构造失败: " << e.what() << endl;
            throw;
        }
    }

    // 移动构造函数 - 高效的资源转移
    Impl(Impl&& other) noexcept 
        : data1_(other.data1_), data2_(other.data2_), counter_(other.counter_.load()),
          resource1_(std::move(other.resource1_)),
          resource2_(std::move(other.resource2_)),
          buffer_(std::move(other.buffer_)) {
        
        // 重置源对象状态
        other.data1_ = 0;
        other.data2_ = 0;
        other.counter_ = 0;
        
        cout << "Impl移动构造函数完成" << endl;
    }

    // 线程安全的操作方法
    int operation1() {
        lock_guard<mutex> lock(internal_mutex_);
        if (!isValidInternal()) {
            throw std::runtime_error("对象状态无效");
        }
        
        ++counter_;
        data1_ += 10;
        
        // 安全地使用资源
        if (buffer_ && !buffer_->empty()) {
            (*buffer_)[0] = data1_;
        }
        
        cout << "operation1执行，data1_=" << data1_ 
             << ", counter=" << counter_.load() << endl;
        return data1_;
    }

    int operation2(int value) {
        lock_guard<mutex> lock(internal_mutex_);
        if (!isValidInternal()) {
            throw std::runtime_error("对象状态无效");
        }
        
        data2_ += value;
        ++counter_;
        
        cout << "operation2执行，输入=" << value 
             << ", data2_=" << data2_ 
             << ", counter=" << counter_.load() << endl;
        return data2_;
    }

    void batchProcess(int count) {
        lock_guard<mutex> lock(internal_mutex_);
        if (!isValidInternal()) {
            throw std::runtime_error("对象状态无效");
        }
        
        // 确保缓冲区足够大
        if (buffer_ && buffer_->size() < static_cast<size_t>(count)) {
            buffer_->resize(count * 2);
        }
        
        for (int i = 0; i < count; ++i) {
            data1_ += i;
            data2_ -= i;
            ++counter_;
            
            if (buffer_) {
                (*buffer_)[i % buffer_->size()] = data1_ + data2_;
            }
        }
        cout << "批量处理完成，处理了" << count << "项" << endl;
    }

    int getCurrentState() const {
        lock_guard<mutex> lock(internal_mutex_);
        return isValidInternal() ? (data1_ + data2_) : -1;
    }

    // 重置资源
    void reset() {
        lock_guard<mutex> lock(internal_mutex_);
        data1_ = 0;
        data2_ = 0;
        counter_ = 0;
        
        // 重新创建资源
        try {
            resource1_ = make_unique<ResourceManager>("数据库连接(重置)");
            resource2_ = make_unique<ResourceManager>("文件句柄(重置)");
            buffer_ = make_unique<std::vector<int>>(1000, 0);
            cout << "资源重置完成" << endl;
        }
        catch (const std::exception& e) {
            cout << "资源重置失败: " << e.what() << endl;
        }
    }

    bool isValid() const {
        lock_guard<mutex> lock(internal_mutex_);
        return isValidInternal();
    }

    // 创建深拷贝
    unique_ptr<Impl> clone() const {
        lock_guard<mutex> lock(internal_mutex_);
        if (!isValidInternal()) {
            return nullptr;
        }
        
        try {
            return make_unique<Impl>(*this);
        }
        catch (const std::exception& e) {
            cout << "克隆失败: " << e.what() << endl;
            return nullptr;
        }
    }

private:
    // 内部有效性检查（不加锁版本）
    bool isValidInternal() const {
        return resource1_ && resource2_ && buffer_;
    }
    
    // 清理资源
    void cleanup() {
        // unique_ptr会自动清理，但可以添加额外的清理逻辑
        if (buffer_) {
            buffer_->clear();
        }
    }

private:
    int data1_;
    int data2_;
    std::atomic<int> counter_;
    
    // 使用RAII管理的资源
    unique_ptr<ResourceManager> resource1_;
    unique_ptr<ResourceManager> resource2_;
    unique_ptr<std::vector<int>> buffer_;
    
    mutable mutex internal_mutex_;
};

// MyInterface的实现 - 强化资源管理
MyInterface::MyInterface() {
    try {
        impl = make_unique<Impl>();
        cout << "MyInterface默认构造函数成功" << endl;
    }
    catch (const std::exception& e) {
        cout << "MyInterface构造失败: " << e.what() << endl;
        throw; // 重新抛出，确保构造失败
    }
}

MyInterface::MyInterface(int initialValue) {
    try {
        impl = make_unique<Impl>(initialValue);
        cout << "MyInterface参数化构造函数成功" << endl;
    }
    catch (const std::exception& e) {
        cout << "MyInterface参数化构造失败: " << e.what() << endl;
        throw;
    }
}

MyInterface::~MyInterface() {
    try {
        lock_guard<mutex> lock(mutex_);
        cout << "MyInterface析构函数开始" << endl;
        // unique_ptr会自动清理impl
        cout << "MyInterface析构函数完成" << endl;
    }
    catch (...) {
        // 析构函数不应抛出异常
    }
}

// 拷贝构造函数 - 异常安全
MyInterface::MyInterface(const MyInterface& other) {
    lock_guard<mutex> lock(other.mutex_);
    if (other.impl) {
        impl = other.impl->clone();
        if (!impl) {
            throw std::runtime_error("拷贝构造失败：无法克隆实现");
        }
        cout << "MyInterface拷贝构造函数成功" << endl;
    } else {
        throw std::runtime_error("拷贝构造失败：源对象无效");
    }
}

// 拷贝赋值操作符 - 异常安全
MyInterface& MyInterface::operator=(const MyInterface& other) {
    if (this != &other) {
        // 创建临时对象，确保异常安全
        MyInterface temp(other);
        
        // 交换实现（强异常安全保证）
        {
            lock_guard<mutex> lock1(mutex_);
            lock_guard<mutex> lock2(other.mutex_);
            impl.swap(temp.impl);
        }
        
        cout << "MyInterface拷贝赋值成功" << endl;
    }
    return *this;
}

// 移动构造函数 - 无异常
MyInterface::MyInterface(MyInterface&& other) noexcept 
    : impl(std::move(other.impl)) {
    cout << "MyInterface移动构造函数" << endl;
}

// 移动赋值操作符 - 无异常
MyInterface& MyInterface::operator=(MyInterface&& other) noexcept {
    if (this != &other) {
        lock_guard<mutex> lock1(mutex_);
        impl = std::move(other.impl);
        cout << "MyInterface移动赋值" << endl;
    }
    return *this;
}

// 公共接口实现 - 添加安全检查
int MyInterface::publicApi1() {
    lock_guard<mutex> lock(mutex_);
    if (!impl) {
        throw std::runtime_error("对象未初始化");
    }
    return impl->operation1();
}

int MyInterface::publicApi2(int i) {
    lock_guard<mutex> lock(mutex_);
    if (!impl) {
        throw std::runtime_error("对象未初始化");
    }
    return impl->operation2(i);
}

void MyInterface::batchOperation(int count) {
    lock_guard<mutex> lock(mutex_);
    if (!impl) {
        throw std::runtime_error("对象未初始化");
    }
    impl->batchProcess(count);
}

int MyInterface::getState() const {
    lock_guard<mutex> lock(mutex_);
    return impl ? impl->getCurrentState() : -1;
}

void MyInterface::reset() {
    lock_guard<mutex> lock(mutex_);
    if (impl) {
        impl->reset();
    } else {
        impl = make_unique<Impl>();
    }
}

bool MyInterface::isValid() const {
    lock_guard<mutex> lock(mutex_);
    return impl && impl->isValid();
}

// 资源管理测试
int main() {
    cout << "=== Pimpl资源管理示例 ===" << endl;
    
    try {
        // 测试正常构造和析构
        {
            cout << "\n--- 测试1: 正常构造和析构 ---" << endl;
            MyInterface obj1;
            MyInterface obj2(100);
            
            obj1.publicApi1();
            obj2.publicApi2(50);
            
            cout << "对象1状态: " << obj1.getState() << endl;
            cout << "对象2状态: " << obj2.getState() << endl;
        } // 对象在此处自动析构
        
        // 测试拷贝和移动
        {
            cout << "\n--- 测试2: 拷贝和移动语义 ---" << endl;
            MyInterface original(200);
            
            // 拷贝构造
            MyInterface copied = original;
            cout << "拷贝对象状态: " << copied.getState() << endl;
            
            // 移动构造
            MyInterface moved = std::move(copied);
            cout << "移动对象状态: " << moved.getState() << endl;
            cout << "移动后原对象是否有效: " << copied.isValid() << endl;
        }
        
        // 测试异常处理
        {
            cout << "\n--- 测试3: 异常处理 ---" << endl;
            MyInterface obj;
            obj.reset(); // 重置资源
            
            try {
                obj.batchOperation(10);
                cout << "批量操作完成，状态: " << obj.getState() << endl;
            }
            catch (const std::exception& e) {
                cout << "捕获异常: " << e.what() << endl;
            }
        }
        
        cout << "\n所有测试完成" << endl;
    }
    catch (const std::exception& e) {
        cout << "程序异常: " << e.what() << endl;
    }
    
    return 0;
}