#include "MyPimpl3.h"
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <exception>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::lock_guard;
using std::mutex;

// 异常安全的资源包装类
class SafeResource {
public:
    explicit SafeResource(const std::string& name) 
        : name_(name), is_valid_(false) {
        try {
            // 模拟资源获取可能失败
            if (name.empty()) {
                throw std::invalid_argument("资源名称不能为空");
            }
            
            cout << "获取资源: " << name_ << endl;
            is_valid_ = true;
        }
        catch (...) {
            cout << "资源获取失败: " << name_ << endl;
            throw; // 重新抛出异常
        }
    }
    
    ~SafeResource() noexcept {
        try {
            if (is_valid_) {
                cout << "释放资源: " << name_ << endl;
                // 执行清理操作
                cleanup();
            }
        }
        catch (...) {
            // 析构函数绝不能抛出异常
            cout << "警告: 资源清理时发生异常: " << name_ << endl;
        }
    }
    
    // 禁用拷贝，允许移动
    SafeResource(const SafeResource&) = delete;
    SafeResource& operator=(const SafeResource&) = delete;
    
    SafeResource(SafeResource&& other) noexcept 
        : name_(std::move(other.name_)), is_valid_(other.is_valid_) {
        other.is_valid_ = false;
        cout << "移动资源: " << name_ << endl;
    }
    
    SafeResource& operator=(SafeResource&& other) noexcept {
        if (this != &other) {
            if (is_valid_) {
                cleanup();
            }
            name_ = std::move(other.name_);
            is_valid_ = other.is_valid_;
            other.is_valid_ = false;
        }
        return *this;
    }
    
    bool isValid() const noexcept { return is_valid_; }
    const std::string& getName() const noexcept { return name_; }

private:
    void cleanup() noexcept {
        // 执行实际的清理工作
        is_valid_ = false;
    }
    
    std::string name_;
    bool is_valid_;
};

// Impl类 - 异常安全版本
class MyInterface::Impl {
public:
    Impl() : data1_(0), data2_(0), counter_(0) {
        try {
            // 使用异常安全的初始化顺序
            initializeResources();
            cout << "Impl默认构造成功" << endl;
        }
        catch (const std::exception& e) {
            cout << "Impl构造失败: " << e.what() << endl;
            // 清理已分配的资源
            cleanup();
            throw; // 重新抛出异常
        }
        catch (...) {
            cout << "Impl构造失败: 未知异常" << endl;
            cleanup();
            throw;
        }
    }
    
    explicit Impl(int initialValue) : data1_(initialValue), data2_(initialValue * 2), counter_(0) {
        try {
            if (initialValue < 0) {
                throw std::invalid_argument("初始值不能为负数");
            }
            
            initializeResources();
            cout << "Impl参数化构造成功，初始值: " << initialValue << endl;
        }
        catch (const std::exception& e) {
            cout << "Impl参数化构造失败: " << e.what() << endl;
            cleanup();
            throw;
        }
        catch (...) {
            cout << "Impl参数化构造失败: 未知异常" << endl;
            cleanup();
            throw;
        }
    }
    
    ~Impl() noexcept {
        try {
            cleanup();
            cout << "Impl析构完成" << endl;
        }
        catch (...) {
            // 析构函数不能抛出异常
            cout << "警告: Impl析构时发生异常" << endl;
        }
    }
    
    // 拷贝构造函数 - 异常安全
    Impl(const Impl& other) : data1_(other.data1_), data2_(other.data2_), counter_(other.counter_.load()) {
        try {
            initializeResources();
            
            // 拷贝缓冲区数据
            if (other.buffer_) {
                buffer_ = make_unique<std::vector<int>>(*other.buffer_);
            }
            
            cout << "Impl拷贝构造成功" << endl;
        }
        catch (const std::exception& e) {
            cout << "Impl拷贝构造失败: " << e.what() << endl;
            cleanup();
            throw;
        }
        catch (...) {
            cout << "Impl拷贝构造失败: 未知异常" << endl;
            cleanup();
            throw;
        }
    }
    
    // 移动构造函数 - 无异常保证
    Impl(Impl&& other) noexcept 
        : data1_(other.data1_), data2_(other.data2_), counter_(other.counter_.load()),
          resource1_(std::move(other.resource1_)),
          resource2_(std::move(other.resource2_)),
          buffer_(std::move(other.buffer_)) {
        
        // 重置源对象
        other.data1_ = 0;
        other.data2_ = 0;
        other.counter_ = 0;
        
        cout << "Impl移动构造成功" << endl;
    }
    
    // 强异常安全的操作方法
    int operation1() {
        lock_guard<mutex> lock(internal_mutex_);
        
        try {
            validateState();
            
            ++counter_;
            data1_ += 10;
            
            // 可能抛出异常的操作
            if (data1_ > 1000) {
                throw std::overflow_error("data1_值过大");
            }
            
            // 更新缓冲区
            updateBuffer(0, data1_);
            
            cout << "operation1成功，data1_=" << data1_ << endl;
            return data1_;
        }
        catch (const std::exception& e) {
            cout << "operation1失败: " << e.what() << endl;
            // 回滚操作
            --counter_;
            data1_ -= 10;
            throw; // 重新抛出异常
        }
    }
    
    int operation2(int value) {
        lock_guard<mutex> lock(internal_mutex_);
        
        try {
            validateState();
            
            if (value < 0) {
                throw std::invalid_argument("输入值不能为负数");
            }
            
            int old_data2 = data2_;
            int old_counter = counter_.load();
            
            // 执行操作
            data2_ += value;
            ++counter_;
            
            // 可能失败的操作
            updateBuffer(1, data2_);
            
            // 模拟可能的失败
            if (data2_ > 2000) {
                throw std::overflow_error("data2_值过大");
            }
            
            cout << "operation2成功，输入=" << value << ", data2_=" << data2_ << endl;
            return data2_;
            
        }
        catch (const std::exception& e) {
            cout << "operation2失败: " << e.what() << endl;
            // 异常安全：回滚所有更改
            // data2_ 和 counter_ 会在异常传播时自动回滚
            throw;
        }
    }
    
    void batchProcess(int count) {
        lock_guard<mutex> lock(internal_mutex_);
        
        try {
            validateState();
            
            if (count <= 0) {
                throw std::invalid_argument("批处理数量必须大于0");
            }
            
            // 保存状态用于回滚
            int old_data1 = data1_;
            int old_data2 = data2_;
            int old_counter = counter_.load();
            
            try {
                // 确保缓冲区足够大
                ensureBufferSize(count);
                
                // 执行批处理
                for (int i = 0; i < count; ++i) {
                    data1_ += i;
                    data2_ -= i;
                    ++counter_;
                    
                    updateBuffer(i % 2, data1_ + data2_);
                    
                    // 模拟可能的失败
                    if (i == count - 1 && count > 10) {
                        throw std::runtime_error("批处理模拟失败");
                    }
                }
                
                cout << "批处理成功，处理了" << count << "项" << endl;
            }
            catch (...) {
                // 回滚所有更改
                data1_ = old_data1;
                data2_ = old_data2;
                counter_ = old_counter;
                throw; // 重新抛出异常
            }
            
        }
        catch (const std::exception& e) {
            cout << "批处理失败: " << e.what() << endl;
            throw;
        }
    }
    
    int getCurrentState() const {
        lock_guard<mutex> lock(internal_mutex_);
        try {
            return isValidInternal() ? (data1_ + data2_) : -1;
        }
        catch (...) {
            return -1; // 异常时返回错误状态
        }
    }
    
    void reset() {
        lock_guard<mutex> lock(internal_mutex_);
        
        try {
            // 重置数据
            data1_ = 0;
            data2_ = 0;
            counter_ = 0;
            
            // 重新初始化资源
            cleanup();
            initializeResources();
            
            cout << "重置成功" << endl;
        }
        catch (const std::exception& e) {
            cout << "重置失败: " << e.what() << endl;
            // 即使重置失败，也要确保对象处于可用状态
            data1_ = 0;
            data2_ = 0;
            counter_ = 0;
            throw;
        }
    }
    
    bool isValid() const noexcept {
        try {
            lock_guard<mutex> lock(internal_mutex_);
            return isValidInternal();
        }
        catch (...) {
            return false;
        }
    }
    
    // 异常安全的克隆方法
    unique_ptr<Impl> clone() const {
        lock_guard<mutex> lock(internal_mutex_);
        
        try {
            if (!isValidInternal()) {
                throw std::runtime_error("无法克隆无效对象");
            }
            
            return make_unique<Impl>(*this);
        }
        catch (const std::exception& e) {
            cout << "克隆失败: " << e.what() << endl;
            return nullptr;
        }
    }

private:
    void initializeResources() {
        // 按依赖顺序初始化资源
        resource1_ = make_unique<SafeResource>("数据库连接");
        resource2_ = make_unique<SafeResource>("文件句柄");
        buffer_ = make_unique<std::vector<int>>(100, 0);
    }
    
    void cleanup() noexcept {
        try {
            // 按相反顺序清理资源
            buffer_.reset();
            resource2_.reset();
            resource1_.reset();
        }
        catch (...) {
            // 清理不能失败
        }
    }
    
    void validateState() const {
        if (!isValidInternal()) {
            throw std::runtime_error("对象状态无效");
        }
    }
    
    bool isValidInternal() const noexcept {
        return resource1_ && resource1_->isValid() &&
               resource2_ && resource2_->isValid() &&
               buffer_;
    }
    
    void updateBuffer(size_t index, int value) {
        if (buffer_ && index < buffer_->size()) {
            (*buffer_)[index] = value;
        } else {
            throw std::out_of_range("缓冲区索引越界");
        }
    }
    
    void ensureBufferSize(size_t required_size) {
        if (!buffer_) {
            buffer_ = make_unique<std::vector<int>>(required_size, 0);
        } else if (buffer_->size() < required_size) {
            buffer_->resize(required_size, 0);
        }
    }

private:
    int data1_;
    int data2_;
    std::atomic<int> counter_;
    
    unique_ptr<SafeResource> resource1_;
    unique_ptr<SafeResource> resource2_;
    unique_ptr<std::vector<int>> buffer_;
    
    mutable mutex internal_mutex_;
};

// MyInterface实现 - 异常安全版本
MyInterface::MyInterface() {
    try {
        impl = make_unique<Impl>();
        cout << "MyInterface默认构造成功" << endl;
    }
    catch (const std::exception& e) {
        cout << "MyInterface构造失败: " << e.what() << endl;
        throw;
    }
}

MyInterface::MyInterface(int initialValue) {
    try {
        impl = make_unique<Impl>(initialValue);
        cout << "MyInterface参数化构造成功" << endl;
    }
    catch (const std::exception& e) {
        cout << "MyInterface参数化构造失败: " << e.what() << endl;
        throw;
    }
}

MyInterface::~MyInterface() {
    try {
        lock_guard<mutex> lock(mutex_);
        // unique_ptr自动管理资源
        cout << "MyInterface析构完成" << endl;
    }
    catch (...) {
        // 析构函数不能抛出异常
    }
}

// 异常安全的拷贝构造函数
MyInterface::MyInterface(const MyInterface& other) {
    lock_guard<mutex> lock(other.mutex_);
    
    if (!other.impl) {
        throw std::runtime_error("拷贝构造：源对象无效");
    }
    
    try {
        impl = other.impl->clone();
        if (!impl) {
            throw std::runtime_error("拷贝构造：克隆失败");
        }
        cout << "MyInterface拷贝构造成功" << endl;
    }
    catch (const std::exception& e) {
        cout << "MyInterface拷贝构造失败: " << e.what() << endl;
        throw;
    }
}

// 异常安全的拷贝赋值操作符（copy-and-swap）
MyInterface& MyInterface::operator=(const MyInterface& other) {
    if (this != &other) {
        try {
            // 创建临时对象（如果失败，不会影响当前对象）
            MyInterface temp(other);
            
            // 原子交换（无异常保证）
            {
                lock_guard<mutex> lock(mutex_);
                impl.swap(temp.impl);
            }
            
            cout << "MyInterface拷贝赋值成功" << endl;
        }
        catch (const std::exception& e) {
            cout << "MyInterface拷贝赋值失败: " << e.what() << endl;
            throw;
        }
    }
    return *this;
}

// 移动构造函数（无异常保证）
MyInterface::MyInterface(MyInterface&& other) noexcept 
    : impl(std::move(other.impl)) {
    cout << "MyInterface移动构造" << endl;
}

// 移动赋值操作符（无异常保证）
MyInterface& MyInterface::operator=(MyInterface&& other) noexcept {
    if (this != &other) {
        lock_guard<mutex> lock(mutex_);
        impl = std::move(other.impl);
        cout << "MyInterface移动赋值" << endl;
    }
    return *this;
}

// 公共接口 - 异常传播
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
    try {
        lock_guard<mutex> lock(mutex_);
        return impl ? impl->getCurrentState() : -1;
    }
    catch (...) {
        return -1;
    }
}

void MyInterface::reset() {
    try {
        lock_guard<mutex> lock(mutex_);
        if (impl) {
            impl->reset();
        } else {
            impl = make_unique<Impl>();
        }
    }
    catch (const std::exception& e) {
        cout << "重置失败: " << e.what() << endl;
        throw;
    }
}

bool MyInterface::isValid() const {
    try {
        lock_guard<mutex> lock(mutex_);
        return impl && impl->isValid();
    }
    catch (...) {
        return false;
    }
}

// 异常安全测试
int main() {
    cout << "=== Pimpl异常安全测试 ===" << endl;
    
    // 测试1：正常操作
    try {
        cout << "\n--- 测试1：正常操作 ---" << endl;
        MyInterface obj(50);
        obj.publicApi1();
        obj.publicApi2(25);
        cout << "状态: " << obj.getState() << endl;
    }
    catch (const std::exception& e) {
        cout << "测试1异常: " << e.what() << endl;
    }
    
    // 测试2：构造异常
    try {
        cout << "\n--- 测试2：构造异常 ---" << endl;
        MyInterface obj(-10); // 负数会导致异常
    }
    catch (const std::exception& e) {
        cout << "测试2异常（预期）: " << e.what() << endl;
    }
    
    // 测试3：操作异常和恢复
    try {
        cout << "\n--- 测试3：操作异常和恢复 ---" << endl;
        MyInterface obj(900);
        
        try {
            // 这会导致overflow异常
            for (int i = 0; i < 5; ++i) {
                obj.publicApi1();
            }
        }
        catch (const std::exception& e) {
            cout << "操作异常（预期）: " << e.what() << endl;
            cout << "异常后状态: " << obj.getState() << endl;
            
            // 重置并继续使用
            obj.reset();
            obj.publicApi2(10);
            cout << "重置后状态: " << obj.getState() << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "测试3异常: " << e.what() << endl;
    }
    
    // 测试4：批处理异常
    try {
        cout << "\n--- 测试4：批处理异常 ---" << endl;
        MyInterface obj;
        
        try {
            obj.batchOperation(15); // 会触发模拟异常
        }
        catch (const std::exception& e) {
            cout << "批处理异常（预期）: " << e.what() << endl;
            cout << "异常后状态: " << obj.getState() << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "测试4异常: " << e.what() << endl;
    }
    
    cout << "\n异常安全测试完成" << endl;
    return 0;
}