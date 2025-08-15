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
    QT_SIGNAL(released, );        // 无参数信号
    
    // 模拟点击
    void click(int x, int y) {
        std::cout << "Button '" << text_ << "' clicked at (" << x << ", " << y << ")\n";
        QT_EMIT(clicked, x, y);
        QT_EMIT(pressed);
        QT_EMIT(released);
    }
    
    const std::string& text() const { return text_; }
    
private:
    std::string text_;
};

// 示例：标签类
class Label : public QObject {
public:
    explicit Label(const std::string& text) : text_(text) {}
    
    // 槽函数：更新文本
    QT_SLOT(void setText(const std::string& newText)) {
        text_ = newText;
        std::cout << "Label text updated to: " << text_ << "\n";
    }
    
    // 槽函数：处理点击
    QT_SLOT(void onButtonClicked(int x, int y)) {
        std::cout << "Label received click event at (" << x << ", " << y << ")\n";
        setText("Clicked at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }
    
    // 槽函数：处理按下
    QT_SLOT(void onButtonPressed()) {
        std::cout << "Label received button pressed event\n";
    }
    
    // 槽函数：处理释放
    QT_SLOT(void onButtonReleased()) {
        std::cout << "Label received button released event\n";
    }
    
    const std::string& text() const { return text_; }
    
private:
    std::string text_;
};

// 示例：窗口类
class MainWindow : public QObject {
public:
    MainWindow() : okButton_("OK"), cancelButton_("Cancel"), statusLabel_("Ready") {
        setupConnections();
    }
    
private:
    void setupConnections() {
        // Qt 风格的连接方式 1: 使用宏
        manageConnection(QT_CONNECT(&okButton_, clicked, &statusLabel_, onButtonClicked));
        manageConnection(QT_CONNECT(&okButton_, pressed, &statusLabel_, onButtonPressed));
        manageConnection(QT_CONNECT(&cancelButton_, clicked, &statusLabel_, onButtonClicked));
        
        // Qt 风格的连接方式 2: 使用静态函数
        manageConnection(QConnect::connect(&okButton_, &Button::released, 
                                         &statusLabel_, &Label::onButtonReleased));
        
        // Qt 风格的连接方式 3: 使用 lambda
        manageConnection(QT_CONNECT_LAMBDA(&cancelButton_, pressed, [this]() {
            std::cout << "MainWindow: Cancel button pressed\n";
            statusLabel_.setText("Cancel pressed");
        }));
        
        // Qt 风格的连接方式 4: 连接到成员函数
        manageConnection(QConnect::connect(&cancelButton_, &Button::released, 
                                         this, &MainWindow::onCancelReleased));
        
        // Qt 风格的连接方式 5: 信号到信号的连接
        manageConnection(QConnect::connect(&okButton_, &Button::clicked, 
                                         &cancelButton_, &Button::clicked));
    }
    
    QT_SLOT(void onCancelReleased()) {
        std::cout << "MainWindow: Cancel button released, closing window...\n";
    }
    
public:
    Button okButton_;
    Button cancelButton_;
    Label statusLabel_;
};

// 示例：数据模型类
class DataModel : public QObject {
public:
    DataModel(int initialValue = 0) : value_(initialValue) {}
    
    QT_SIGNAL(valueChanged, int);
    QT_SIGNAL(dataUpdated, const std::string&);
    
    void setValue(int value) {
        if (value_ != value) {
            value_ = value;
            std::cout << "DataModel: Value changed to " << value_ << "\n";
            QT_EMIT(valueChanged, value_);
            QT_EMIT(dataUpdated, "Value: " + std::to_string(value_));
        }
    }
    
    void increment() { setValue(value_ + 1); }
    void decrement() { setValue(value_ - 1); }
    
    int value() const { return value_; }
    
private:
    int value_;
};

// 示例：视图类
class DataView : public QObject {
public:
    explicit DataView(const std::string& name) : name_(name) {}
    
    QT_SLOT(void displayValue(int value)) {
        std::cout << name_ << ": Displaying value " << value << "\n";
    }
    
    QT_SLOT(void displayData(const std::string& data)) {
        std::cout << name_ << ": Displaying data '" << data << "'\n";
    }
    
private:
    std::string name_;
};

void demonstrateQtStyleConnections() {
    std::cout << "\n=== Qt Style Static Connect Demo ===\n";
    
    MainWindow window;
    
    // 模拟用户交互
    std::cout << "\nSimulating OK button click:\n";
    window.okButton_.click(100, 200);
    
    std::cout << "\nSimulating Cancel button click:\n";
    window.cancelButton_.click(150, 250);
}

void demonstrateMVCPattern() {
    std::cout << "\n=== MVC Pattern with Static Connect ===\n";
    
    DataModel model(42);
    DataView view1("View1");
    DataView view2("View2");
    Button incrementBtn("Increment");
    Button decrementBtn("Decrement");
    
    // 连接模型到视图
    auto conn1 = QConnect::connect(&model, &DataModel::valueChanged, 
                                  &view1, &DataView::displayValue);
    auto conn2 = QConnect::connect(&model, &DataModel::dataUpdated,
                                  &view2, &DataView::displayData);
    
    // 连接按钮到模型操作
    auto conn3 = QConnect::connect(&incrementBtn, &Button::clicked, [&model](int, int) {
        model.increment();
    });
    
    auto conn4 = QConnect::connect(&decrementBtn, &Button::clicked, [&model](int, int) {
        model.decrement();
    });
    
    // 模拟操作
    std::cout << "Initial state:\n";
    model.setValue(10);
    
    std::cout << "\nClicking increment button:\n";
    incrementBtn.click(0, 0);
    
    std::cout << "\nClicking decrement button:\n";
    decrementBtn.click(0, 0);
    
    std::cout << "\nDisconnecting view1 and clicking increment again:\n";
    conn1.disconnect();
    incrementBtn.click(0, 0);
}

void demonstrateAdvancedConnections() {
    std::cout << "\n=== Advanced Connection Patterns ===\n";
    
    Button sourceBtn("Source");
    Button targetBtn("Target");
    Label display("Display");
    
    // 信号链：sourceBtn -> targetBtn -> display
    auto conn1 = QConnect::connect(&sourceBtn, &Button::clicked,
                                  &targetBtn, &Button::clicked);
    auto conn2 = QConnect::connect(&targetBtn, &Button::clicked,
                                  &display, &Label::onButtonClicked);
    
    // 使用自动断开连接
    {
        auto scopedConn = QConnect::connectScoped(&sourceBtn, &Button::pressed, [&]() {
            std::cout << "Scoped connection: Source button pressed\n";
        });
        
        std::cout << "Testing signal chain:\n";
        sourceBtn.click(300, 400);
        
        std::cout << "\nTesting scoped connection:\n";
        // 注意：这里 scopedConn 在作用域结束时会自动断开
    }
    
    std::cout << "\nAfter scoped connection destroyed:\n";
    sourceBtn.click(500, 600);
}

int main() {
    try {
        demonstrateQtStyleConnections();
        demonstrateMVCPattern();
        demonstrateAdvancedConnections();
        
        std::cout << "\n=== All Qt-style connect demos completed! ===\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}