#pragma once
template <class T> 
class Stack 
{
public:
    Stack();
    ~Stack();
    void push(T t);
    T pop();
    bool isEmpty();
private:
    T* m_pT;
    int m_maxSize;
    int m_size;
};

template <class  T>
Stack<T>::Stack()
{
    m_maxSize = 100;
    m_size = 0;
    m_pT = new T[m_maxSize];
}
template <class T>
Stack<T>::~Stack()
{
    delete[] m_pT;
}

template <class T>
void Stack<T>::push(T t)
{
    m_size++;
    m_pT[m_size - 1] = t;

}
template <class T>
T Stack<T>::pop()
{
    T t = m_pT[m_size - 1];
    m_size--;
    return t;
}
template <class T>
bool Stack<T>::isEmpty()
{
    return m_size == 0;
}

/******************************************************/
template <class T, int maxsize = 100> 
class Stack1 {
public:
    Stack1();
    ~Stack1();
    void push(T t);
    T pop();
    bool isEmpty();
private:
    T* m_pT;
    int m_maxSize;
    int m_size;
};

template <class T, int maxsize> 
Stack1<T, maxsize>::Stack1() 
{
    m_maxSize = maxsize;
    m_size = 0;
    m_pT = new T[m_maxSize];
}
template <class T, int maxsize> 
Stack1<T, maxsize>::~Stack1() 
{
    delete[] m_pT;
}

template <class T, int maxsize> 
void Stack1<T, maxsize>::push(T t) 
{
    m_size++;
    m_pT[m_size - 1] = t;

}
template <class T, int maxsize> 
T Stack1<T, maxsize>::pop() 
{
    T t = m_pT[m_size - 1];
    m_size--;
    return t;
}
template <class T, int maxsize> 
bool Stack1<T, maxsize>::isEmpty() 
{
    return m_size == 0;
}

// 类模板中使用默认模板参数
template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
public:
    // 使用std::vector作为底层容器
    std::vector<T, Allocator> data;

    void add(const T& value) {
        data.push_back(value);
    }

    T& operator[](size_t index) {
        return data[index];
    }
};

// 函数模板中使用默认模板参数
template <typename T = int>
void printValue(const T& value) {
    std::cout << value << std::endl;
}

// 函数模板，返回类型由 auto 推导
template <typename T, typename U>
auto add(T t, U u) {
    return t + u;
}


template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 变长参数模板函数，用于日志记录
template<typename... Args>
void logMessage(const Args&... args) {
    std::ostringstream stream;
    using expander = int[];
    (void)expander {
        0, (void(stream << args << " "), 0)...
    };
    stream << std::endl;
    std::cout << stream.str();
}


template <typename T>
void fun_hehe(T t) {
    //do something
}

template <typename... T>
void fun(T... t) {
    //t...;
    //fun_hehe(t)...;
}