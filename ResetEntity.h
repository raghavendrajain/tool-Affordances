#ifndef _RESETENTITY_H
#define _RESETENTITY_H

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


bool resetEntityPosition(SimObj *entity, Vector3d desiredPosition);
bool resetEntityOrientation(SimObj *entity, Rotation goalRotation);
float randomFloat(float min, float max);
bool newEntityPose(SimObj *entity, Rotation initToolRotation, double randomStep);
bool resetRandomTargetOrientation(SimObj *entity);
// double* resetToNewToolRotation(SimObj *entity, Rotation initialRotation);

void maintainOrientationOfTool(SimObj * entity, Rotation initialToolRot);


#endif