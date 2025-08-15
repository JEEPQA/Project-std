#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <list>
#include <queue>
#include <future>

std::queue<int> g_queue;
std::mutex g_mutex;
std::condition_variable g_cv;
void producer() {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::unique_lock<std::mutex> lock(g_mutex);
        g_queue.push(i);

        std::cout << "Producer: " << i << std::endl;

        // 通知等待中的消费者线程
        g_cv.notify_one();
    }
}
void consumer(int id) {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock(g_mutex);

        // 等待条件满足
        g_cv.wait(lock, [] { return !g_queue.empty(); });

        int value = g_queue.front();
        g_queue.pop();

        std::cout << "Consumer " << id << ": " << value << std::endl;
    }
}

std::atomic<bool> ready{ false };
std::atomic<int> data1{ 0 };
void producer1() {
    data1.store(42, std::memory_order_relaxed); // 原子性的更新data的值, 但是不保证内存顺序
    //ready.store(true, std::memory_order_relaxed); // 原子性的更新ready的值, 但是不保证内存顺序
    ready.store(true, std::memory_order_release); // 保证data的更新操作先于ready的更新操作
}
void consumer1() {
    // 原子性的读取ready的值, 但是不保证内存顺序
    while (!ready.load(std::memory_order_relaxed)) {
        std::this_thread::yield(); // 啥也不做, 只是让出CPU时间片
    }

    // 保证先读取ready的值, 再读取data的值
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield(); // 啥也不做, 只是让出CPU时间片
    }

    // 当ready为true时, 再原子性的读取data的值
    std::cout << data1.load(std::memory_order_relaxed);  // 4. 消费者线程使用数据
}



std::mutex mtx; // 全局互斥锁.
std::condition_variable cv; // 全局条件变量.
bool ready = false; // 全局标志位.

void do_print_id(int id)
{
    std::unique_lock <std::mutex> lck(mtx);
    while (!ready) // 如果标志位不为 true, 则等待...
        cv.wait(lck); // 当前线程被阻塞, 当全局标志位变为 true 之后,
    // 线程被唤醒, 继续往下执行打印线程编号id.
    std::cout << "thread " << id << '\n';
}

void go()
{
    std::unique_lock <std::mutex> lck(mtx);
    ready = true; // 设置全局标志位为 true.
    cv.notify_all(); // 唤醒所有线程.
}

void test1()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
        threads[i] = std::thread(do_print_id, i);

    std::cout << "10 threads ready to race...\n";
    //go(); // go!

    for (auto& th : threads)
        //th.join();
        th.detach();

    //std::unique_lock <std::mutex> lck(mtx);
    //cv.wait(lck); // 当前线程被阻塞, 当全局标志位变为 true 之后,

    //Sleep(10000);
    //go(); // go!

}

void test2() 
{
    // launch一个生产者线程
    //std::thread t1(producer1);
    // launch一个消费者线程
    //std::thread t2(consumer);
    //t1.join();
    //t2.join();
}



int main()
{
    //test1();
    //test2();

    return 0;
}