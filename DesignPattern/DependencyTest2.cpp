#include <memory>
#include <iostream>
#include <fstream>

// 定义日志记录器接口（面向接口编程）
class ILogger 
{
public:
    virtual ~ILogger() {}
    virtual void log(const std::string& message) = 0;
};

// 1.日志记录器的具体实现：控制台日志记录器
class ConsoleLogger : public ILogger 
{
public:
    void log(const std::string& message) override {
        std::cout << "Console Logger: " << message << std::endl;
    }
};

// 2.文件日志记录器的具体实现
class FileLogger : public ILogger 
{
public:
    explicit FileLogger(const std::string& filename)
        : _outputFile(filename) {}

    void log(const std::string& message) override {
        _outputFile << "File Logger: " << message << std::endl;
    }

private:
    std::ofstream _outputFile;
};

// 3：复合日志记录器，同时记录到控制台和文件
class CompositeLogger : public ILogger 
{
public:
    CompositeLogger(std::shared_ptr<ILogger> consoleLogger,
        std::shared_ptr<ILogger> fileLogger)
        : _consoleLogger(std::move(consoleLogger)),
        _fileLogger(std::move(fileLogger)) {}

    void log(const std::string& message) override {
        _consoleLogger->log(message);
        _fileLogger->log(message);
    }

private:
    std::shared_ptr<ILogger> _consoleLogger;
    std::shared_ptr<ILogger> _fileLogger;
};


// 日志服务类，依赖于ILogger接口
class LoggerService 
{
public:
    // 构造函数注入ILogger对象
    explicit LoggerService(std::shared_ptr<ILogger> logger)
        : _logger(std::move(logger)) {}

    // 使用注入的日志记录器记录消息
    void logMessage(const std::string& message) {
        _logger->log(message);
    }

private:
    std::shared_ptr<ILogger> _logger;
};


std::shared_ptr<ILogger> CreateConsoleLogger()
{
    return std::make_shared<ConsoleLogger>();
}
std::shared_ptr<ILogger> CreateFileLogger()
{
    return std::make_shared<FileLogger>("app.log");
}
std::shared_ptr<ILogger> CreateCompositeLogger()
{
    std::shared_ptr<ILogger> consoleLogger = std::make_shared<ConsoleLogger>();
    std::shared_ptr<ILogger> fileLogger = std::make_shared<FileLogger>("app.log");
    return std::make_shared<CompositeLogger>(consoleLogger, fileLogger);
}

//依赖注入
void test1()
{
    // 创建具体的日志记录器实现
    //std::shared_ptr<ILogger> consoleLogger = std::make_shared<ConsoleLogger>();
    std::shared_ptr<ILogger> consoleLogger = CreateConsoleLogger();

    // 通过构造函数注入日志记录器到日志服务类
    LoggerService loggerService(consoleLogger);

    // 使用日志服务类记录消息
    loggerService.logMessage("Hello, this is a log message!");

}

void test2()
{
    // 创建控制台日志记录器
    std::shared_ptr<ILogger> consoleLogger = std::make_shared<ConsoleLogger>();

    // 创建文件日志记录器
    std::shared_ptr<ILogger> fileLogger = std::make_shared<FileLogger>("app.log");

    // 创建复合日志记录器并注入两个底层日志记录器
    std::shared_ptr<ILogger> compositeLogger = std::make_shared<CompositeLogger>(consoleLogger, fileLogger);

    // 通过构造函数注入复合日志记录器到日志服务类
    LoggerService loggerService(compositeLogger);

    // 使用日志服务类记录消息
    loggerService.logMessage("Hello, this is a log message that will be written to both console and file!");

}


// 
int Detest() {

    test1();

    return 0;
}
