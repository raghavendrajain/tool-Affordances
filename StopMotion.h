#ifndef _STOPMOTION_H
#define _STOPMOTION_H

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

template <typename T> string tostr(const T& t);

double* stopMotion(SimObj *target, Vector3d desiredTargetVelocity, double K_p, double K_i, double K_d);

std::string parse_python_exception(); // functional declaration for exception handling


#endif