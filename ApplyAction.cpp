#include "ApplyAction.h"
#include "ControlPosition.h"

void tapWithTool( SimObj *toolName, Rotation keepRotation,  Vector3d forceOnTool)
{
 
  double *ptrTool = NULL;
  double torque;
  ptrTool = controlRotation(toolName, keepRotation, 20.0, 0.0, 20.0);
  // cout << "The difference in tool orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 10000 ;
  // cout << " torque = " << torque << endl;
  toolName->addTorque(0, torque, 0);
   
  toolName->addForce(forceOnTool.x(), forceOnTool.y(), forceOnTool.z());

}



