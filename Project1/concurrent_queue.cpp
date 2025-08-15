#pragma once

#include <queue>
#include <mutex>
#include <utility>

#define CONCURRENT_NAMESPACE_BEGIN namespace concurrency {
#define CONCURRENT_NAMESPACE_END }

CONCURRENT_NAMESPACE_BEGIN

template <typename Elem>
class concurrent_queue
{
public:

    void clear()
    {
        std::lock_guard<std::mutex> lg(mtx);
        while (!q.empty()) q.pop();
    }

    [[nodiscard]] bool empty() const
    {
        std::lock_guard<std::mutex> lg(mtx);
        bool ret = q.empty();
        return ret;
    }

    template <typename... Ts>
    void push(Ts&&... args)
    {
        std::lock_guard<std::mutex> lg(mtx);
        q.emplace(std::forward<Ts>(args)...);
    }

    bool try_pop(Elem& out)
    {
        std::lock_guard<std::mutex> lg(mtx);
        if (q.empty()) return false;
        out = std::move(q.front());
        q.pop();
        return true;
    }

private:

    std::queue<Elem> q;
    mutable std::mutex mtx;
};

CONCURRENT_NAMESPACE_END

//同步队列
template<typename T>
class SimpleSyncQueue
{
public:
    SimpleSyncQueue() {}

    //SimpleSyncQueue(const SimpleSyncQueue& other) = default;
    //SimpleSyncQueue& operator=(const SimpleSyncQueue& other) = default;

    SimpleSyncQueue& operator=(const SimpleSyncQueue& other)
    {
        this->_queue = other._queue;
        //this->_mutex = other._mutex;
        //this->_notEmpty = other._notEmpty;
        return *this;
    }

    SimpleSyncQueue(const SimpleSyncQueue& other)
    {
        *this = other;
    }
    void Enqueue(const T& x)
    {
        std::lock_guard<std::mutex> locker(_mutex); _queue.push(x); _notEmpty.notify_one();
    }
    void Dequeue(T& x)
    {
        std::unique_lock<std::mutex> locker(_mutex); _notEmpty.wait(locker, [this] {return !_queue.empty(); }); x = _queue.front();
        //_queue.pop_front();
        _queue.pop();
    }
    bool Empty()
    {
        std::lock_guard<std::mutex> locker(_mutex);
        return _queue.empty();
    }
    size_t Size()
    {
        std::lock_guard<std::mutex> locker(_mutex);
        return _queue.size();
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _notEmpty;
};

template <typename T>
class ThreadSafeQueue
{
protected:
    std::queue<std::shared_ptr<T>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;

public:
    void enqueue(std::shared_ptr<T> new_value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(new_value));
        m_cv.notify_one();
    }

    std::shared_ptr<T> dequeue()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
        {
            m_cv.wait(lock);
        }

        std::shared_ptr<T> value = std::move(m_queue.front());
        m_queue.pop();
        return value;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        while (!m_queue.empty())
        {
            m_queue.pop();
        }
    }

    int size()
    {
        return m_queue.size();
    }
};