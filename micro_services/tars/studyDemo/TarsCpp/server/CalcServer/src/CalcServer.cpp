#include "CalcServer.h"
#include "CalcImp.h"

using namespace std;

CalcServer g_app;

/////////////////////////////////////////////////////////////////
void
CalcServer::initialize()
{
    //initialize application here:
    //...

    addServant<CalcImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".CalcObj");
}
/////////////////////////////////////////////////////////////////
void
CalcServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
