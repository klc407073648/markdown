#ifndef _CalcServer_H_
#define _CalcServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class CalcServer : public Application
{
public:
    /**
     *
     **/
    virtual ~CalcServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern CalcServer g_app;

////////////////////////////////////////////
#endif
