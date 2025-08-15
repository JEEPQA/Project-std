#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>

/*
1.锁的类型
  std::mutex 基本锁
  std::timed_mutex 定时锁
  std::recursive_mutex std::recursive_timed_mutex   递归互斥锁
  std::shared_timed_mutex C++14
  std::shared_mutex   C++17
2.安全地获取和释放互斥锁
  std::lock_guard 
  std::unique_lock
  std::shared_lock C++14
  std::scoped_lock C++17

3.条件变量
4.原子操作

*/

std::mutex mutex1, mutex2;
void thread_function() {
    std::lock(mutex1, mutex2); // 同时锁定两个互斥量
    //mutex1.lock();  mutex2.lock();
    std::cout << "线程 " << std::this_thread::get_id() << " 已锁定资源" << std::endl;

    // 手动解锁
    mutex1.unlock();
    mutex2.unlock();
}

std::mutex mtx;  // 全局互斥锁
int counter = 0; // 共享资源
void attempt_10k_increases()
{
    for (int i = 0; i < 10000; ++i)
    {
        mtx.lock();
        ++counter;  // 受保护的操作
        mtx.unlock();
    }
}

std::recursive_mutex rec_mtx;
int count = 0;
void recursive_increment(int level)
{
    if (level > 0)
    {
        rec_mtx.lock();
        recursive_increment(level - 1);
        rec_mtx.unlock();
    }
    else
    {
        ++count;
    }
}

std::timed_mutex timed_mtx;
void attempt_lock_for(int id)
{
    auto now = std::chrono::steady_clock::now();
    if (timed_mtx.try_lock_for(std::chrono::seconds(1)))
    {
        std::cout << "Thread " << id << " got the lock." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2)); // hold the lock for 2 seconds
        timed_mtx.unlock();
    }
    else
    {
        std::cout << "Thread " << id << " couldn't get the lock." << std::endl;
    }
}

std::recursive_timed_mutex rt_mtx;
void recursive_access(int level, int thread_id)
{
    if (rt_mtx.try_lock_for(std::chrono::milliseconds(100)))
    {
        std::cout << "Thread " << thread_id << " entered level " << level << std::endl;
        if (level > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            recursive_access(level - 1, thread_id);
        }
        rt_mtx.unlock();
    }
    else
    {
        std::cout << "Thread " << thread_id << " could not enter level " << level << std::endl;
    }
}

std::shared_mutex shared_mtx;
int data = 0;

void reader_function(int id)
{
    shared_mtx.lock_shared();
    std::cout << "Reader " << id << " sees data as: " << data << std::endl;
    shared_mtx.unlock_shared();
}

void writer_function(int new_data)
{
    shared_mtx.lock();
    data = new_data;
    std::cout << "Writer updates data to: " << data << std::endl;
    shared_mtx.unlock();
}


void test1()
{
    std::thread t1(thread_function);
    std::thread t2(thread_function);
    t1.join();
    t2.join();

    std::thread threads[10];
    for (int i = 0; i < 10; ++i)
    {
        threads[i] = std::thread(attempt_10k_increases);
    }

    for (auto& th : threads)
    {
        th.join();
    }
    std::cout << "Result of counter: " << counter << std::endl;

    std::thread t3(recursive_increment, 10);
    t3.join();
    std::cout << "Count is: " << count << std::endl;

    std::thread threads2[2];
    for (int i = 0; i < 2; ++i)
    {
        threads2[i] = std::thread(attempt_lock_for, i);
    }
    for (auto& th : threads2)
    {
        th.join();
    }

    std::thread writer(writer_function, 100);
    std::thread readers[10];
    for (int i = 0; i < 10; ++i)
    {
        readers[i] = std::thread(reader_function, i);
    }

    writer.join();
    for (auto& reader : readers)
    {
        reader.join();
    }

}

std::mutex mtx;
void safe_increment(int& counter) {
    std::lock_guard<std::mutex> guard(mtx); // 自动锁定
    ++counter;
    std::cout << "线程 " << std::this_thread::get_id() << " 增加计数器: " << counter << std::endl;
}

void test2()
{
    int counter = 0;
    std::thread t1(safe_increment, std::ref(counter));
    std::thread t2(safe_increment, std::ref(counter));

    t1.join();
    t2.join();
}

std::mutex mtx2;
std::condition_variable cv;
bool ready = false;
void worker_thread() {
    std::unique_lock<std::mutex> lock(mtx2); // 延迟锁定
    cv.wait(lock, [] { return ready; }); // 等待条件满足
    std::cout << "线程 " << std::this_thread::get_id() << " 开始工作" << std::endl;
}

void test3()
{
    std::thread t(worker_thread);
    {
        std::lock_guard<std::mutex> guard(mtx2);
        ready = true;
    }
    cv.notify_one(); // 通知线程

    t.join();
}

int main() {

    test1();
    test2();
    return 0;
}