#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <algorithm>
#include <type_traits>

namespace qt_signal_slot {

// 前声明
template<typename... Args>
class Signal;

template<typename... Args>
class Connection;

// 连接管理基类
class ConnectionBase {
public:
    virtual ~ConnectionBase() = default;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;

protected:
    bool connected_ = true;
};

// 具体的连接实现
template<typename... Args>
class ConnectionImpl : public ConnectionBase {
public:
    using SlotFunction = std::function<void(Args...)>;
    
    ConnectionImpl(Signal<Args...>* signal, SlotFunction slot)
        : signal_(signal), slot_(std::move(slot)) {}
    
    void disconnect() override {
        connected_ = false;
        if (signal_) {
            signal_->removeConnection(this);
        }
    }
    
    bool isConnected() const override {
        return connected_;
    }
    
    void call(Args... args) {
        if (connected_ && slot_) {
            slot_(args...);
        }
    }
    
private:
    Signal<Args...>* signal_;
    SlotFunction slot_;
};

// 连接对象
template<typename... Args>
class Connection {
public:
    Connection() = default;
    
    explicit Connection(std::shared_ptr<ConnectionImpl<Args...>> impl)
        : impl_(std::move(impl)) {}
    
    void disconnect() {
        if (auto conn = impl_.lock()) {
            conn->disconnect();
        }
    }
    
    bool isConnected() const {
        if (auto conn = impl_.lock()) {
            return conn->isConnected();
        }
        return false;
    }
    
private:
    std::weak_ptr<ConnectionImpl<Args...>> impl_;
};

// 自动断开连接的 RAII 包装器
template<typename... Args>
class ScopedConnection {
public:
    ScopedConnection() = default;
    
    explicit ScopedConnection(Connection<Args...> conn)
        : connection_(std::move(conn)) {}
    
    ~ScopedConnection() {
        disconnect();
    }
    
    // 禁止拷贝，允许移动
    ScopedConnection(const ScopedConnection&) = delete;
    ScopedConnection& operator=(const ScopedConnection&) = delete;
    
    ScopedConnection(ScopedConnection&& other) noexcept
        : connection_(std::move(other.connection_)) {}
    
    ScopedConnection& operator=(ScopedConnection&& other) noexcept {
        if (this != &other) {
            disconnect();
            connection_ = std::move(other.connection_);
        }
        return *this;
    }
    
    void disconnect() {
        connection_.disconnect();
    }
    
    bool isConnected() const {
        return connection_.isConnected();
    }
    
private:
    Connection<Args...> connection_;
};

// 信号类
template<typename... Args>
class Signal {
public:
    using SlotFunction = std::function<void(Args...)>;
    using ConnectionType = Connection<Args...>;
    using ScopedConnectionType = ScopedConnection<Args...>;
    
    Signal() = default;
    ~Signal() = default;
    
    // 禁止拷贝，允许移动
    Signal(const Signal&) = delete;
    Signal& operator=(const Signal&) = delete;
    
    Signal(Signal&& other) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        std::lock_guard<std::mutex> lock_other(other.mutex_);
        connections_ = std::move(other.connections_);
        // 更新所有连接的信号指针
        for (auto& conn : connections_) {
            if (auto impl = conn.lock()) {
                impl->signal_ = this;
            }
        }
    }
    
    Signal& operator=(Signal&& other) noexcept {
        if (this != &other) {
            std::lock_guard<std::mutex> lock(mutex_);
            std::lock_guard<std::mutex> lock_other(other.mutex_);
            connections_ = std::move(other.connections_);
            // 更新所有连接的信号指针
            for (auto& conn : connections_) {
                if (auto impl = conn.lock()) {
                    impl->signal_ = this;
                }
            }
        }
        return *this;
    }
    
    // 连接函数对象/lambda
    template<typename Callable>
    ConnectionType connect(Callable&& callable) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto impl = std::make_shared<ConnectionImpl<Args...>>(
            this, SlotFunction(std::forward<Callable>(callable)));
        connections_.emplace_back(impl);
        return ConnectionType(impl);
    }
    
    // 连接成员函数
    template<typename Object, typename Method>
    ConnectionType connect(Object* object, Method&& method) {
        return connect([object, method](Args... args) {
            (object->*method)(args...);
        });
    }
    
    // 连接到另一个信号
    ConnectionType connect(Signal<Args...>& other) {
        return connect([&other](Args... args) {
            other.emit(args...);
        });
    }
    
    // 创建自动断开连接
    template<typename... ConnectArgs>
    ScopedConnectionType connectScoped(ConnectArgs&&... args) {
        return ScopedConnectionType(connect(std::forward<ConnectArgs>(args)...));
    }
    
    // 发射信号
    void emit(Args... args) {
        std::vector<std::shared_ptr<ConnectionImpl<Args...>>> activeConnections;
        
        {
            std::lock_guard<std::mutex> lock(mutex_);
            // 清理无效连接并收集有效连接
            connections_.erase(
                std::remove_if(connections_.begin(), connections_.end(),
                    [](const std::weak_ptr<ConnectionImpl<Args...>>& conn) {
                        return conn.expired();
                    }),
                connections_.end());
            
            // 收集所有有效连接
            for (const auto& conn : connections_) {
                if (auto impl = conn.lock()) {
                    if (impl->isConnected()) {
                        activeConnections.push_back(impl);
                    }
                }
            }
        }
        
        // 在锁外调用槽函数
        for (auto& conn : activeConnections) {
            conn->call(args...);
        }
    }
    
    // 重载 operator() 为 emit 的别名
    void operator()(Args... args) {
        emit(args...);
    }
    
    // 断开所有连接
    void disconnectAll() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& conn : connections_) {
            if (auto impl = conn.lock()) {
                impl->disconnect();
            }
        }
        connections_.clear();
    }
    
    // 获取连接数量
    size_t connectionCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return std::count_if(connections_.begin(), connections_.end(),
            [](const std::weak_ptr<ConnectionImpl<Args...>>& conn) {
                if (auto impl = conn.lock()) {
                    return impl->isConnected();
                }
                return false;
            });
    }
    
    // 移除特定连接（内部使用）
    void removeConnection(ConnectionImpl<Args...>* conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.erase(
            std::remove_if(connections_.begin(), connections_.end(),
                [conn](const std::weak_ptr<ConnectionImpl<Args...>>& weak_conn) {
                    if (auto impl = weak_conn.lock()) {
                        return impl.get() == conn;
                    }
                    return true; // 移除过期的连接
                }),
            connections_.end());
    }
    
private:
    mutable std::mutex mutex_;
    std::vector<std::weak_ptr<ConnectionImpl<Args...>>> connections_;
    
    template<typename...>
    friend class ConnectionImpl;
};

// QObject 风格的基类
class QObject {
public:
    virtual ~QObject() {
        disconnectAll();
    }
    
protected:
    // 在析构时自动断开所有连接
    void disconnectAll() {
        for (auto& conn : connections_) {
            conn();
        }
        connections_.clear();
    }
    
    // 添加连接到管理列表
    template<typename... Args>
    void manageConnection(Connection<Args...> conn) {
        connections_.emplace_back([conn]() mutable { conn.disconnect(); });
    }
    
private:
    std::vector<std::function<void()>> connections_;
};


// ==================== 静态 connect 函数 (Qt 风格) ====================

// 静态 connect 类，模仿 Qt 的 QObject::connect
class QConnect {
public:
    // 1. 连接信号到 lambda/函数对象
    template<typename... Args, typename Callable>
    static auto connect(Signal<Args...>& signal, Callable&& callable) {
        return signal.connect(std::forward<Callable>(callable));
    }
    
    // 2. 连接信号到对象的成员函数
    template<typename... Args, typename Object, typename Method>
    static auto connect(Signal<Args...>& signal, Object* receiver, Method&& slot) {
        return signal.connect(receiver, std::forward<Method>(slot));
    }
    
    // 3. 连接对象的信号到另一个对象的槽
    template<typename Sender, typename SignalType, typename Receiver, typename SlotType>
    static auto connect(Sender* sender, SignalType Sender::* signal, 
                       Receiver* receiver, SlotType slot) {
        return (sender->*signal).connect(receiver, slot);
    }
    
    // 4. 连接对象的信号到 lambda
    template<typename Sender, typename SignalType, typename Callable>
    static auto connect(Sender* sender, SignalType Sender::* signal, Callable&& callable) {
        return (sender->*signal).connect(std::forward<Callable>(callable));
    }
    
    // 5. 连接信号到信号
    template<typename... Args>
    static auto connect(Signal<Args...>& source, Signal<Args...>& target) {
        return source.connect(target);
    }
    
    // 6. 连接对象的信号到另一个对象的信号
    template<typename Sender, typename SignalType1, typename Receiver, typename SignalType2>
    static auto connect(Sender* sender, SignalType1 Sender::* sourceSignal,
                       Receiver* receiver, SignalType2 Receiver::* targetSignal) {
        return (sender->*sourceSignal).connect(receiver->*targetSignal);
    }
    
    // 7. 创建自动断开连接
    template<typename... ConnectArgs>
    static auto connectScoped(ConnectArgs&&... args) {
        auto conn = connect(std::forward<ConnectArgs>(args)...);
        using ConnectionType = decltype(conn);
        return ScopedConnection<typename ConnectionType::value_type>(conn);
    }
};

// 全局静态 connect 函数，类似 Qt 的用法
template<typename... Args>
static auto connect = [](auto&&... args) {
    return QConnect::connect(std::forward<decltype(args)>(args)...);
};

} // namespace qt_signal_slot

// 宏定义，简化使用
#define QT_SIGNAL(name, ...) qt_signal_slot::Signal<__VA_ARGS__> name
#define QT_SLOT(name) name
#define QT_EMIT(signal, ...) signal.emit(__VA_ARGS__)

// Qt 风格的连接宏
#define QT_CONNECT(sender, signal, receiver, slot) \
    qt_signal_slot::QConnect::connect(sender, &std::remove_reference_t<decltype(*sender)>::signal, \
                                     receiver, &std::remove_reference_t<decltype(*receiver)>::slot)

#define QT_CONNECT_LAMBDA(sender, signal, lambda) \
    qt_signal_slot::QConnect::connect(sender, &std::remove_reference_t<decltype(*sender)>::signal, lambda)