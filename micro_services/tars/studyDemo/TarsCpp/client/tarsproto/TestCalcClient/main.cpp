#include <iostream>
#include "servant/Communicator.h"
#include "Calc.h"

using namespace std;
using namespace TestCalcApp;
using namespace tars;

int main(int argc,char ** argv)
{
    Communicator comm;

    try
    {
        CalcPrx prx;
        comm.stringToProxy("TestApp.CalcServer.CalcObj@tcp -h 172.25.0.5 -p 9000" , prx);

        try
        {
            int addRet = prx->add(5, 5);
			int subRet = prx->sub(5, 5);
			int mulRet = prx->multiply(5, 5);
			int divRet = prx->divide(5, 5);
            cout<<"addRet:"<<addRet<<" subRet:"<<subRet<<endl;
			cout<<"mulRet:"<<mulRet<<" divRet:"<<divRet<<endl;
        }
        catch(exception &ex)
        {
            cerr << "ex:" << ex.what() << endl;
        }
        catch(...)
        {
            cerr << "unknown exception." << endl;
        }
    }
    catch(exception& e)
    {
        cerr << "exception:" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }

    return 0;
}
