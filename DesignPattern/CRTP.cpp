#include <iostream>

//模板继承
template <typename T>
class Shape {
public:
    void draw() {
        // 调用派生类的具体实现
        static_cast<T*>(this)->drawImpl();
    }
};

class Circle : public Shape<Circle> {
public:
    void drawImpl() { std::cout << "Drawing Circle" << std::endl; }
};

class Square : public Shape<Square> {
public:
    void drawImpl() { std::cout << "Drawing Square" << std::endl; }
};


template <typename Derived>
class Addable {
public:
    Derived operator+(const Derived& other) const {
        Derived result = static_cast<const Derived&>(*this);
        result += other;
        return result;
    }
};

class Vector : public Addable<Vector> {
public:
    int x, y;
    Vector(int x, int y) : x(x), y(y) {}
    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

template <typename T>
class VectorBase {
public:
    T operator+(const  T& other) const {
        T result;
        for (size_t i = 0; i < static_cast<const  T*>(this)->size(); ++i) {
            result[i] = (*static_cast<const  T*>(this))[i] + other[i];
        }
        return result;
    }
};

class Vec3f : public VectorBase<Vec3f> {
public:
    float x, y, z;
    size_t size() const { return 3; }

    Vec3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vec3f(std::initializer_list<float> list) {
        auto it = list.begin();
        x = (it != list.end()) ? *it++ : 0;
        y = (it != list.end()) ? *it++ : 0;
        z = (it != list.end()) ? *it++ : 0;
    }

    float operator[](size_t i) const {
        return (&x)[i];
    }
    float& operator[](size_t i) {
        return (&x)[i];
    }
};

//
//对象计数
//
template <typename Derived>
class ObjectCounter {
public:
    static int count;
    ObjectCounter() { count++; }
    ~ObjectCounter() { count--; }
};

template <typename Derived>
int ObjectCounter<Derived>::count = 0;

class MyClass : public ObjectCounter<MyClass> {};


//
//链式调用
//
template <typename Derived>
class Chainable {
public:
    Derived& self() { return static_cast<Derived&>(*this); }
    Derived& setName(const std::string& name) {
        self().name = name;
        return self();
    }
    Derived& setValue(int value) {
        self().value = value;
        return self();
    }
};

class Config : public Chainable<Config> {
public:
    std::string name;
    int value;
};

//
//编译期策略模式
//
template <typename Derived>
class SortingPolicy {
public:
    void sort(int* data, size_t size) {
        static_cast<Derived*>(this)->sortImpl(data, size);
    }
};

class QuickSort : public SortingPolicy<QuickSort> {
public:
    void sortImpl(int* data, size_t size) {
        std::cout << "QuickSort" << std::endl;
        // 具体实现
    }
};

class MergeSort : public SortingPolicy<MergeSort> {
public:
    void sortImpl(int* data, size_t size) {
        std::cout << "MergeSort" << std::endl;
        // 具体实现
    }
};

// 单例模式
template<typename T>
class Singleton {
protected:
    Singleton() = default;

public:
    static T& getInstance() {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

class DatabaseManager : public Singleton<DatabaseManager> {
    friend class Singleton<DatabaseManager>;
private:
    DatabaseManager() = default;
public:
    void connect() { /* 数据库连接逻辑 */ }
};

// 资源管理器
template<typename T>
class ResourceManager {
public:
    ~ResourceManager() {
        static_cast<T*>(this)->cleanup();
    }

    void acquire() {
        static_cast<T*>(this)->acquireImpl();
    }
};

class FileManager : public ResourceManager<FileManager> {
public:
    void acquireImpl() { file_ = fopen("data.txt", "r"); }
    void cleanup() { if (file_) fclose(file_); }

private:
    FILE* file_ = nullptr;
};

// 编译时计算
template<typename T>
class CompileTimeCalculator {
public:
    static constexpr auto calculate() {
        return T::calculateImpl();
    }
};

class Factorial : public CompileTimeCalculator<Factorial> {
public:
    static constexpr int calculateImpl() {
        return factorial(5);  // 编译时计算
    }

private:
    static constexpr int factorial(int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    }
};


int main() {
    Circle circle;
    Square square;
    circle.draw();  // 输出 "Drawing Circle"
    square.draw();  // 输出 "Drawing Square"

    Shape<Circle> shape;
    shape.draw();  // 输出 "Drawing Circle" (调用 Circle 的实现)

    Vector v1(1, 2), v2(3, 4);
    Vector v3 = v1 + v2;  // 使用基类的 operator+ 实现
    std::cout << v3.x << ", " << v3.y << std::endl;  // 输出 "4, 6"

    Vec3f a1{ 1, 2, 3 }, b1{ 4, 5, 6 };
    Vec3f c1 = a1 + b1; // c.x=5, c.y=7, c.z=9
    std::cout << c1[0] << ", " << c1[1] << ", " << c1[2] << std::endl;  // 输出 "5, 7, 9"


    MyClass a, b;
    std::cout << MyClass::count << std::endl;  // 输出 "2"
    {
        MyClass c;
        std::cout << MyClass::count << std::endl;  // 输出 "3"
    }
    std::cout << MyClass::count << std::endl;  // 输出 "2"


    Config config;
    config.setName("MyApp").setValue(42);
    std::cout << config.name << " " << config.value << std::endl; // 输出 "MyApp 42"


    int arr[5] = { 5, 3, 1, 4, 2 };
    QuickSort sorter;
    sorter.sort(arr, 5);  // 输出 "QuickSort"
}
