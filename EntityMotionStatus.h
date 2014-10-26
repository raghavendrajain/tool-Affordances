#ifndef _ENTITYMOTIONSTATUS_H
#define _ENTITYMOTIONSTATUS_H


#include "ControllerEvent.h"  
#include "Controller.h"  
#include "Logger.h"  
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/python.hpp>
#include <Python.h>
#include <dlfcn.h>

using namespace std;

bool checkEntityMotionStatus(SimObj * entity);

#endif