#ifndef _APPLYACTION_H
#define _APPLYACTION_H


#include "ControllerEvent.h"  
#include "Controller.h"  
#include "Logger.h"  
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/python.hpp>
#include <Python.h>
#include <dlfcn.h>

#include "ControlPosition.h"

using namespace std;

void tapWithTool( SimObj *toolName,  Rotation keepRotation, Vector3d forceOnTool);


#endif