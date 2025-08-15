#include <string>
#include <iostream>

/**
* @brief 简单概述
* @brief 操作系统基类
*/
class OperationSys
{
public:
	OperationSys() {}
	~OperationSys() {}

	virtual void SetName() = 0;
	virtual void OutputName() = 0;

protected:
	std::string m_sName;
};

/**
* @brief 简单概述
* @brief IOS操作系统
*/
class IOS_Sys :public OperationSys
{
public:
	IOS_Sys() {}
	~IOS_Sys() {}

	virtual void SetName()
	{
		m_sName = "IOS-SYS";
	}
	virtual void OutputName()
	{
		std::cout << "I am IOS，name:" << m_sName << std::endl;
	}
};

/**
* @brief 简单概述
* @brief 鸿蒙操作系统
*/
class HarmonySys :public OperationSys
{
public:
	HarmonySys() {}
	~HarmonySys() {}

	virtual void SetName()
	{
		m_sName = "HarmonySys";
	}
	virtual void OutputName()
	{
		std::cout << "I am Harmony operation system，name:" << m_sName << std::endl;
	}
};

/**
* @brief 简单概述
* @brief 安卓操作系统
*/
class AndroidSys :public OperationSys
{
public:
	AndroidSys() {}
	~AndroidSys() {}

	virtual void SetName()
	{
		m_sName = "AndroidSys";
	}
	virtual void OutputName()
	{
		std::cout << "I am Android operation system，name:" << m_sName << std::endl;
	}
};

/**
* @brief 简单概述
* @brief 手机基类
*/
class Phone
{
public:
	Phone() {}
	~Phone() {}

	virtual void SetName() = 0;
	virtual void OutputName() = 0;
	virtual void SetOperation(OperationSys* sys) { m_pOperSys = sys; }
	virtual void OutputSysName() = 0;

protected:
	OperationSys* m_pOperSys;
	std::string m_sName;
};

/**
* @brief 简单概述
* @brief 苹果手机
*/
class IPhone :public Phone
{
public:
	IPhone() {}
	~IPhone() {}

	virtual void SetName()
	{
		m_sName = "IPhone";
	}
	virtual void OutputName()
	{
		std::cout << "I am IPhone,Name:" << m_sName << std::endl;
	}
	virtual void SetOperation(OperationSys* sys)
	{
		m_pOperSys = sys;
	}
	virtual void OutputSysName()
	{
		m_pOperSys->OutputName();
	}
};

/**
* @brief 简单概述
* @brief 华为手机
*/
class HwPhone :public Phone
{
public:
	HwPhone() {}
	~HwPhone() {}

	virtual void SetName()
	{
		m_sName = "HuaWeiPhone";
	}
	virtual void OutputName()
	{
		std::cout << "I am HuaWei,Name:" << m_sName << std::endl;
	}
	virtual void SetOperation(OperationSys* sys)
	{
		m_pOperSys = sys;
	}
	virtual void OutputSysName()
	{
		m_pOperSys->OutputName();
	}
};

/**
* @brief 简单概述
* @brief 小米手机
*/
class MiPhone :public Phone
{
public:
	MiPhone() {}
	~MiPhone() {}

	virtual void SetName()
	{
		m_sName = "MiPhone";
	}
	virtual void OutputName()
	{
		std::cout << "I am XiaoMi,Name:" << m_sName << std::endl;
	}
	virtual void SetOperation(OperationSys* sys)
	{
		m_pOperSys = sys;
	}
	virtual void OutputSysName()
	{
		m_pOperSys->OutputName();
	}
};

int main(int argc, char* argv[])
{
	IOS_Sys* iSys = new IOS_Sys();
	iSys->SetName();
	IPhone* iPhone = new IPhone();
	iPhone->SetName();
	iPhone->SetOperation(iSys);

	HarmonySys* hSys = new HarmonySys();
	hSys->SetName();
	HwPhone* wPhone = new HwPhone();
	wPhone->SetName();
	wPhone->SetOperation(hSys);

	AndroidSys* aSys = new AndroidSys();
	aSys->SetName();
	MiPhone* mPhone = new MiPhone();
	mPhone->SetName();
	mPhone->SetOperation(aSys);

	iPhone->OutputName();
	iPhone->OutputSysName();

	wPhone->OutputName();
	wPhone->OutputSysName();

	mPhone->OutputName();
	mPhone->OutputSysName();

	std::cout << std::endl;
	for (size_t i = 0; i < 1; i++)
	{
		std::cout << "* ** *** **** ***** ****** *******" << std::endl;
		std::cout << "** *** **** ***** ****** ******* *" << std::endl;
		std::cout << "*** **** ***** ****** ******* * **" << std::endl;
		std::cout << "**** ***** ****** ******* * ** ***" << std::endl;
		std::cout << "**** **** **** <==> **** **** ****" << std::endl;
		std::cout << "***** ****** ******* * ** *** ****" << std::endl;
		std::cout << "****** ******* * ** *** **** *****" << std::endl;
		std::cout << "******* * ** *** **** ***** ******" << std::endl;
		std::cout << "* ** *** **** ***** ****** *******" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "<<<<<<<<== Hello! You are a good man! ==>>>>>>>" << std::endl;
	getchar();
	return 0;
}