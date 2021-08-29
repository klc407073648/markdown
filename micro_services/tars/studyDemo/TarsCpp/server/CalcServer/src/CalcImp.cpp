#include "CalcImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void CalcImp::initialize()
{
    //initialize servant here:
    //...
	//注册处理函数：
    TARS_ADD_ADMIN_CMD_NORMAL("SETSTRING", CalcImp::setStr);
	TARS_ADD_ADMIN_CMD_NORMAL("SHOWSTRING", CalcImp::showStr);
}

//////////////////////////////////////////////////////
void CalcImp::destroy()
{
    //destroy servant here:
    //...
}

bool CalcImp::setStr(const std::string& command, const std::string& params, std::string& result)
{
	TLOGDEBUG("CalcImp::setStr:"<<params<<endl);
	my_str = params;
    return true;
}

bool CalcImp::showStr(const std::string& command, const std::string& params, std::string& result)
{
	TLOGDEBUG("CalcImp::showStr:"<<my_str<<endl);
    return true;
}

int CalcImp::add(int a,int b, tars::TarsCurrentPtr current)
{
	int ret = a+b;
    TLOGDEBUG("CalcImp::add:"<<ret<<endl);
    return ret;
}

int CalcImp::sub(int a,int b, tars::TarsCurrentPtr current)
{
	int ret = a-b;
    TLOGDEBUG("CalcImp::sub:"<<ret<<endl);
    return ret;
}

int CalcImp::multiply(int a,int b, tars::TarsCurrentPtr current)
{
	int ret = a*b;
    TLOGDEBUG("CalcImp::multiply:"<<ret<<endl);
    return ret;
}

int CalcImp::divide(int a,int b, tars::TarsCurrentPtr current)
{
	int ret = a/b;
    TLOGDEBUG("CalcImp::divide:"<<ret<<endl);
    return ret;
}


