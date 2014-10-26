#ifndef _MEASUREDISPLACEMENT_H
#define _MEASUREDISPLACEMENT_H


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

void measureTargetDisplacement(SimObj *target, Vector3d initTargetPos);

#endif