#include "qt_signal_slot.hpp"
#include <iostream>
#include <string>

using namespace qt_signal_slot;

// 示例：按钮类
class Button : public QObject {
public:
    explicit Button(const std::string& text) : text_(text) {}
    
    // 信号：按钮被点击
    QT_SIGNAL(clicked, int, int); // x, y 坐标
    QT_SIGNAL(pressed, );         // 无参数信号
    
    // 模拟点击
    void click(int x, int y) {
        std::cout << "Button '" << text_ << "' clicked at (" << x << ", " << y << ")\n";
        QT_EMIT(clicked, x, y);
        QT_EMIT(pressed);
    }
    
private:
    std::string text_;
};

// 示例：标签类
class Label : public QObject {
public:
    explicit Label(const std::string& text) : text_(text) {}
    
    // 槽函数：更新文本
    QT_SLOT(void updateText(const std::string& newText)) {
        text_ = newText;
        std::cout << "Label text updated to: " << text_ << "\n";
    }
    
    // 槽函数：处理点击
    QT_SLOT(void onButtonClicked(int x, int y)) {
        std::cout << "Label received click event at (" << x << ", " << y << ")\n";
        updateText("Clicked at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }
    
    // 槽函数：处理按下
    QT_SLOT(void onButtonPressed()) {
        std::cout << "Label received button pressed event\n";
    }
    
    const std::string& getText() const { return text_; }
    
private:
    std::string text_;
};

// 示例：窗口类
class Window : public QObject {
public:
    Window() : button_("OK"), label_("Ready") {
        setupConnections();
    }
    
private:
    void setupConnections() {
        // 连接按钮点击到标签更新
        manageConnection(button_.clicked.connect(&label_, &Label::onButtonClicked));
        
        // 连接按钮按下到标签处理
        manageConnection(button_.pressed.connect(&label_, &Label::onButtonPressed));
        
        // 使用 lambda 连接
        manageConnection(button_.clicked.connect([this](int x, int y) {
            std::cout << "Window: Button clicked at (" << x << ", " << y << ")\n";
        }));
        
        // 创建自动断开连接
        auto scopedConn = button_.pressed.connectScoped([this]() {
            std::cout << "Window: This connection will auto-disconnect\n";
        });
        // scopedConn 会在作用域结束时自动断开
    }
    
public:
    Button button_;
    Label label_;
};

// 示例：计数器类
class Counter : public QObject {
public:
    Counter(int initial = 0) : count_(initial) {}
    
    QT_SIGNAL(valueChanged, int);
    
    void increment() {
        ++count_;
        std::cout << "Counter incremented to: " << count_ << "\n";
        QT_EMIT(valueChanged, count_);
    }
    
    void setValue(int value) {
        if (count_ != value) {
            count_ = value;
            std::cout << "Counter set to: " << count_ << "\n";
            QT_EMIT(valueChanged, count_);
        }
    }
    
    int getValue() const { return count_; }
    
private:
    int count_;
};

// 示例：显示器类
class Display : public QObject {
public:
    QT_SLOT(void showValue(int value)) {
        std::cout << "Display showing: " << value << "\n";
    }
    
    QT_SLOT(void showDoubleValue(int value)) {
        std::cout << "Display showing double: " << (value * 2) << "\n";
    }
};

void demonstrateBasicUsage() {
    std::cout << "\n=== Basic Usage Demo ===\n";
    
    Window window;
    
    // 模拟用户交互
    window.button_.click(100, 200);
    window.button_.click(150, 250);
    
    std::cout << "Final label text: " << window.label_.getText() << "\n";
}

void demonstrateSignalChaining() {
    std::cout << "\n=== Signal Chaining Demo ===\n";
    
    Counter counter1(0);
    Counter counter2(10);
    Display display;
    
    // 连接计数器到显示器
    auto conn1 = counter1.valueChanged.connect(&display, &Display::showValue);
    auto conn2 = counter2.valueChanged.connect(&display, &Display::showDoubleValue);
    
    // 链接两个计数器（counter1 的变化会触发 counter2）
    auto chainConn = counter1.valueChanged.connect(&counter2, &Counter::setValue);
    
    std::cout << "Incrementing counter1:\n";
    counter1.increment();
    counter1.increment();
    
    std::cout << "\nDisconnecting chain and incrementing again:\n";
    chainConn.disconnect();
    counter1.increment();
}

void demonstrateLambdaConnections() {
    std::cout << "\n=== Lambda Connections Demo ===\n";
    
    Counter counter;
    
    // 使用 lambda 连接
    auto lambdaConn = counter.valueChanged.connect([](int value) {
        std::cout << "Lambda: Value is now " << value << "\n";
        if (value > 5) {
            std::cout << "Lambda: Value exceeded 5!\n";
        }
    });
    
    // 使用 std::function 连接
    std::function<void(int)> func = [](int value) {
        std::cout << "Function object: Processing value " << value << "\n";
    };
    auto funcConn = counter.valueChanged.connect(func);
    
    for (int i = 0; i < 8; ++i) {
        counter.increment();
    }
}

void demonstrateScopedConnections() {
    std::cout << "\n=== Scoped Connections Demo ===\n";
    
    Counter counter;
    
    {
        // 自动断开连接的作用域
        auto scopedConn = counter.valueChanged.connectScoped([](int value) {
            std::cout << "Scoped: This will only work within scope, value = " << value << "\n";
        });
        
        counter.increment(); // 这个会触发上面的连接
        counter.increment();
    } // scopedConn 在这里自动断开
    
    std::cout << "After scope - this increment won't trigger the scoped connection:\n";
    counter.increment(); // 这个不会触发上面的连接
}

void demonstrateSignalToSignal() {
    std::cout << "\n=== Signal to Signal Demo ===\n";
    
    // 创建两个计数器和一个显示器
    Counter source;
    Counter target;
    Display display;
    
    // 连接 source 的信号到 target 的信号
    auto signalConn = source.valueChanged.connect(target.valueChanged);
    
    // 连接 target 到显示器
    auto displayConn = target.valueChanged.connect(&display, &Display::showValue);
    
    std::cout << "Incrementing source (should trigger both target signal and display):\n";
    source.increment();
    source.increment();
}

int main() {
    try {
        demonstrateBasicUsage();
        demonstrateSignalChaining();
        demonstrateLambdaConnections();
        demonstrateScopedConnections();
        demonstrateSignalToSignal();
        
        std::cout << "\n=== All demos completed successfully! ===\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}