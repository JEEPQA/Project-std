#include <map>
#include <string>
#include <exception>
#include <iostream>
#include <shared_mutex>
#include <vector>
#include <iomanip>
#include <charconv>
#include <variant>
#include <optional>
#include <any>
#include <filesystem>
#include <fstream>


#include <concepts>

using namespace std;

/*
1.module模块
2.concepts概念
 concept requires <concepts>
2.协程<coroutine>
3.Ranges 范围库
4.三路比较运算符（<=>）
5.consteval和constinit关键字
6.constexpr改进
  虚函数支持
  try-catch块支持
  动态内存分配
7.Lambda改进
  允许[=, this]捕获
  模板参数支持
  静态成员变量
8.线程增加
  std::jthread
  std::stop_token std::stop_source std::stop_callback
9.同步增加
<semaphore> std::latch  std::barrier
10.范围for循环增强
11.属性增加
  [[likely]] [[unlikely]] [[nodiscard(reason)]]
12.<chrono>增强
13.std::span
14.std::format
15.std::source_location
16.位运算增加
17.字符串支持 starts_with, ends_with
18.容器和算法的增强
19.<bit>
20.using 改进
21.char8_t
*/