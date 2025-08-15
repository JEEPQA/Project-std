#include <iostream>
#include <thread>
#include <atomic>
#include <future>
#include <queue>
#include <string>
#include <chrono>

//
//std::promise std::future
//
void compute(int a, int b, std::promise<int> promise) {
	int sum = a + b;
	// 模拟耗时
	std::this_thread::sleep_for(std::chrono::seconds(1));
	promise.set_value(sum);
}
void func(std::future<int>& fut) {
	int x = fut.get();
	std::cout << "value: " << x << std::endl;
}

void test1()
{
	std::promise<int> promise_;
	std::future<int> future_ = promise_.get_future();
	std::thread t(compute, 3, 4, std::move(promise_));
	t.detach();
	std::cout << "test1() 1\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "test1() 2\n";

	//future_.wait();
	std::cout << "get compute result:" << future_.get() << std::endl;//在结果就绪之前，该方法会提供阻塞机制
	//t.join();

	std::promise<int> prom;
	std::future<int> fut = prom.get_future();
	std::thread t1(func, std::ref(fut));
	//std::thread t1(func, fut);
	prom.set_value(100);
	t1.join();




}

//
//std::packaged_task
//
int countdown(int from, int to) {
	for (int i = from; i != to; --i) {
		std::cout << i << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "Finished!\n";
	return from - to;
}
void test2()
{
	std::packaged_task<int(int, int)> task(countdown); // 设置 packaged_task
	std::future<int> ret = task.get_future(); // 获得与 packaged_task 共享状态相关联的 future 对象.
	std::thread th(std::move(task), 10, 0);   //创建一个新线程完成计数任务.

	int value = ret.get();                    // 等待任务完成并获取结果.

	std::cout << "The countdown lasted for " << value << " seconds.\n";

	th.join();
}

//
//std::async
//
// 模拟耗时的数据库查询操作，让该函数运行五秒
std::string fetchDataFromDB(std::string recvdData)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
    return "DB_" + recvdData;
}
// 模拟耗时的本地数据读取操作，让该函数运行五秒
std::string fetchDataFromFile(std::string recvdData)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
    return "File_" + recvdData;
}
void test3()
{
	auto start = std::chrono::system_clock::now();
	std::string dbData = fetchDataFromDB("Data");
	std::string fileData = fetchDataFromFile("Data");
	auto end = std::chrono::system_clock::now();

	auto diff = std::chrono::duration_cast <std::chrono::seconds> (end - start).count();
	std::cout << "总耗时 = " << diff << " 秒" << std::endl;

	std::string data = dbData + " :: " + fileData;
	std::cout << "Data = " << data << std::endl;
}
void test4()
{
	auto start = std::chrono::system_clock::now();
	std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
	std::string fileData = fetchDataFromFile("Data");
	std::string dbData = resultFromDB.get();
	auto end = std::chrono::system_clock::now();

	auto diff = std::chrono::duration_cast <std::chrono::seconds> (end - start).count();
	std::cout << "总耗时 = " << diff << " 秒" << std::endl;

	std::string data = dbData + " :: " + fileData;
	std::cout << "Data = " << data << std::endl;
}

//
//std::call_once
//
std::once_flag flag;// 必须全局或共享
void simple_thread(const std::string& name)
{
	//std::call_once(flag, []() { std::cout << "Simple example: called once\n"; });
	std::this_thread::sleep_for(std::chrono::seconds(1));
	thread_local int count = 0;
	++count;
	std::cout << name << ": " << count <<"\n";

	//std::cout << "simple_do_once\n";
}
void test5()
{
	std::thread st1(simple_thread, "st1");
	std::thread st2(simple_thread, "st2");
	std::thread st3(simple_thread, "st3");
	std::thread st4(simple_thread, "st4");
	//std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "simple_thread 1\n";
	st1.join();
	std::cout << "simple_thread\n";
	st2.join();
	st3.join();
	st4.join();
}

int main() {

	test1();
	//test2();
	//test3();
	//test4();
	//test5();
	return 0;
}

