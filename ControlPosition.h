#ifndef _CONTROLPOSITION_H
#define _CONTROLPOSITION_H


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

namespace py = boost::python; // create namespace variable for boost::python

std::string parse_python_exception(); // functional declaration for exception handling

template <typename T> string tostr(const T& t);
// { ostringstream os; os<<t; return os.str(); } // template to convert double variables to string


double* controlPosition(Vector3d goalPos, Vector3d currentPos, double K_p, double K_i, double K_d);
double* controlRotation(SimObj *entity, Rotation goalRot, double K_p, double K_i, double K_d);
double* controlRotationUsingEuler(double goalOrientationAboutY, Rotation currentRot, double K_p, double K_i, double K_d);
double* controlTargetRotation(Rotation goalRot, Rotation currentRot, double K_p, double K_i, double K_d);
double* controlAngularVelocity(Vector3d currentAngularVel, double K_p, double K_i, double K_d);
double* reachRotation( SimObj *entity, Rotation initialRotation, double changeInAngle, double K_p, double K_i, double K_d);
double* reachZeroPose(SimObj *entity, Rotation currentRotation, double K_p, double K_i, double K_d);
double* controlRotationThroughout(Rotation goalRot, Rotation currentRot, double K_p, double K_i, double K_d);


#endif