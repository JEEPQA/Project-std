#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

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

using Event = std::function<int(std::string&)>;

class EventManager {

private:
    std::map<std::string, std::vector<Event> > events;

public:
    EventManager() {}
    EventManager* eventRegist(std::string event_name, Event event) {
        std::vector<Event>* listeners = &events[event_name];
        // if this listener is already registered, we wont add it again
        //if (std::find(listeners->begin(), listeners->end(), event) != listeners->end())
        //{
        //    return this;
        //}
        listeners->push_back(event);
        return this;
    }
    void emit(std::string event_name, std::string arg) {
        std::vector<Event> listeners = events[event_name];
        for (int i = 0; i < listeners.size(); i++) {
            listeners[i](arg);
        }
    }
};

class EventManager2 {

private:
    std::map<std::string, SimpleSyncQueue<Event>> events;

public:
    EventManager2() {}
    EventManager2* eventRegist(std::string event_name, Event event) {
        auto listeners = &events[event_name];
        // if this listener is already registered, we wont add it again
        //if (std::find(listeners->begin(), listeners->end(), event) != listeners->end())
        //{
        //    return this;
        //}
        listeners->Enqueue(event);
        return this;
    }
    void emit(std::string event_name, std::string arg) {
        //默认对vector中的元素进行了拷贝操作,需要实现拷贝构造函数
        auto listeners = events[event_name];

        //不会进行对元素进行拷贝
        //const auto& listeners = events[event_name];
        while (listeners.Size()) {
            Event event;
            listeners.Dequeue(event);
            event(arg);
        }
    }
};

int callback1(std::string& arg) {
    std::cout << "callback1 " << arg << std::endl;
    return 0;
}
int callback2(std::string& arg) {
    std::cout << "callback2 " << arg << std::endl;
    return 0;
}

int callback2_1(std::string& arg) {
    std::cout << "callback2_1 " << arg << std::endl;
    return 0;
}

int test() {
    EventManager* event_manager = new EventManager();
    //注册回调函数
    event_manager->eventRegist("event1", callback1);
    event_manager->eventRegist("event2", callback2_1);
    event_manager->eventRegist("event2", callback2);
    //执行回调函数
    event_manager->emit("event1", "event_manager");
    event_manager->emit("event2", "event_manager");

    EventManager2* event_manager2 = new EventManager2();
    //注册回调函数
    event_manager2->eventRegist("event1", callback1);
    event_manager2->eventRegist("event2", callback2_1);
    event_manager2->eventRegist("event2", callback2);
    //执行回调函数
    event_manager2->emit("event1", "event_manager2");
    event_manager2->emit("event2", "event_manager2");


    return 0;
}