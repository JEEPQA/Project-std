#include <iostream>
#include<queue>
#include <thread>
using namespace std;

//协议处理类
class HandleClientProtocol {
public:
	//处理增加金币
	void AddMoney() {
		cout << "给玩家增加金币！" << endl;
	}
	//处理增加钻石
	void AddDiamond() {
		cout << "给玩家增加钻石!" << endl;
	}
	//处理玩家装备
	void AddEquipment() {
		cout << "给玩家穿装备！" << endl;
	}
	//处理玩家升级
	void addLevel() {
		cout << "给玩家升级!" << endl;
	}
};

//命令接口
class ICommand {
public:
	virtual void handle() = 0; //处理客户端请求的接口
};


//处理增加金币请求
class AddMoneyCommand :public ICommand {
public:
	AddMoneyCommand(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddMoney();
	}
public:
	HandleClientProtocol* pProtocol;
};

//处理增加钻石的请求
class AddDiamondCommand :public ICommand {
public:
	AddDiamondCommand(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddDiamond();
	}
public:
	HandleClientProtocol* pProtocol;
};


//处理玩家穿装备的请求
class AddEquipmentCommand : public ICommand {
public:
	AddEquipmentCommand(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddEquipment();
	}
public:
	HandleClientProtocol* pProtocol;
};

//处理玩家升级的请求
class AddLevelCommand : public ICommand {
public:
	AddLevelCommand(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->addLevel();
	}
public:
	HandleClientProtocol* pProtocol;

};


//服务器程序
class Serser {
public:
	void addRequest(ICommand* command) {
		mCommands.push(command);
	}

	void startHandle() {
		while (!mCommands.empty()) {

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			ICommand* command = mCommands.front();
			command->handle();
			mCommands.pop();
		}
	}
public:
	queue<ICommand*> mCommands;
};

void test(void) {

	HandleClientProtocol* protocol = new HandleClientProtocol;
	
	ICommand* addmoney = new AddMoneyCommand(protocol);//客户端增加金币的请求
	ICommand* adddiamond = new AddDiamondCommand(protocol);//客户端增加钻石的请求
	ICommand* addequpment = new AddEquipmentCommand(protocol);//客户端穿装备的请求
	ICommand* addlevel = new AddLevelCommand(protocol);//客户端升级请求

	Serser* server = new Serser;
	//将客户端请求加入到处理的队列中
	server->addRequest(addmoney);
	server->addRequest(adddiamond);
	server->addRequest(addequpment);
	server->addRequest(addlevel);

	//服务器开始处理请求
	server->startHandle();
}

int main(void) {
	test();
	return 0;
}
