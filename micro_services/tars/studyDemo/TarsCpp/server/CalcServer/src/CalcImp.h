#ifndef _CalcImp_H_
#define _CalcImp_H_

#include "servant/Application.h"
#include "Calc.h"
#include <string>
#include <iostream>

using namespace std;
/**
 *
 *
 */
class CalcImp : public TestCalcApp::Calc
{
public:
    /**
     *
     */
    virtual ~CalcImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};
	virtual int add(int a,int b, tars::TarsCurrentPtr current);
	virtual int sub(int a,int b, tars::TarsCurrentPtr current);
	virtual int multiply(int a,int b, tars::TarsCurrentPtr current);
	virtual int divide(int a,int b, tars::TarsCurrentPtr current);
private:
	bool setStr(const std::string& command, const std::string& params, std::string& result);
	bool showStr(const std::string& command, const std::string& params, std::string& result);
public:
	std::string my_str;
};
/////////////////////////////////////////////////////
#endif
