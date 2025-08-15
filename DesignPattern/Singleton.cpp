#include <mutex>

// 线程安全的单例模式
class Singleton {
public:

    //双重检查锁定（DCLP）：不可靠
    const Singleton& getInstance(void)
    {
        static Singleton* pInstance = NULL;
        if (pInstance == NULL)
        {
            // need entry a mutex for thread safe
            std::lock_guard<std::mutex> lock(mtx);
            if (pInstance == NULL)
            {
                pInstance = new Singleton();
                //pInstance->initialization();
            }
        }
        const Singleton& ref = (*pInstance);
        return ref;
    }

    static Singleton& getInstance1()  //线程安全
    {
        std::call_once(init_flag, []() {instance2.reset(new Singleton());});
        return *instance2;
    }

    Singleton& getInstance2() 
    {
        static Singleton instance;  //线程安全
        return instance;
    }

    Singleton* getInstance3()
    {
        static Singleton instance;  //线程安全
        return &instance;
    }

    // 禁止拷贝和赋值
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;  // 私有构造函数
    ~Singleton() = default;

    static std::unique_ptr<Singleton> instance2;
    static std::once_flag init_flag;
    static std::mutex mtx;
};

// 静态成员初始化
std::unique_ptr<Singleton> Singleton::instance2;
std::once_flag Singleton::init_flag;
std::mutex Singleton::mtx;

