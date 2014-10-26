#include <string>    
#include "ControllerEvent.h"  
#include "Controller.h"  
#include "Logger.h"  
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/python.hpp>
#include <Python.h>
#include <dlfcn.h>
#include <typeinfo>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

#include "StopMotion.h"
#include "ControlPosition.h"
#include "ApplyAction.h"
#include "MeasureDisplacement.h"
#include "EntityMotionStatus.h"
#include "ResetEntity.h"


#define PI 3.141592      
#define DEG2RAD(DEG) ( (PI) * (DEG) / 180.0 )

using namespace std;
ofstream myfile ("example_13.csv", ios_base::trunc);

class MyController : public Controller {  
public:  
  void onInit(InitEvent &evt);  
  double onAction(ActionEvent&);  
  void onRecvMsg(RecvMsgEvent &evt); 
  void onCollision(CollisionEvent &evt); 

  void applyTapAction();

private:

  const char* linkName;
  CParts *link;

  Vector3d initTargetPos;
  Rotation initTargetRotation;

  Vector3d initToolPos;
  Rotation initToolRotation;
  Rotation newToolRotation;

  Vector3d currentToolPos;
  Vector3d currentTargetPos;

  Vector3d  linkInitPos, linkPos;
  Rotation  linkInitRotation, linkCurrentRotation;


  Vector3d toolVelAtHit;

  bool positionData;
  bool didToolHit; // to check if collision with target has happened 
  bool isTargetAtInitLoc; // to check if target is at initial required position
  bool isToolAtInitLoc;   // to check if tool is at initial required position
  bool isTargetAtInitPose; // to check if target is at initial set orientation
  bool isToolAtInitPose;   // to check if tool is at its initial set orientation
  bool isTargetAtRest;   // to check if target is not moving
  bool isToolAtRest;     // to check if tool is not moving

  bool isNewPoseObtained; // to set tool in a new pose
  bool isToolAtDesiredPosition; 

  int simulationCount;
  int manipulationsPerSample;
  int totalSamples;

  double *ptrTool;
  double torque;
  double restartTime;

  bool moveForward;
  bool onRecvMsgFunction; 

  bool isXForceVarianceAllowed ;
  bool isZForceVarianceAllowed ;

  double f_x, f_z;

  bool flag;


};  
  


void MyController::onInit(InitEvent &evt) {  

  totalSamples = 240;
  simulationCount = 0;

  SimObj *box = getObj("box_003");
  SimObj *TShapeTool = getObj("TShapeTool_003");
 
  
  box->getPosition(initTargetPos);
  box->getRotation(initTargetRotation);

  TShapeTool->getPosition(initToolPos);
  TShapeTool->getRotation(initToolRotation);


  didToolHit = false;
  flag = true;


  isTargetAtInitLoc = true; 
  isToolAtInitLoc = true;   
  isTargetAtInitPose = true;
  isToolAtInitPose = true;  
  isTargetAtRest = true;   
  isToolAtRest = true;  


  isNewPoseObtained = false;
  isToolAtDesiredPosition = false;

  f_x = 0 ;
  f_z = 0 ;

  ptrTool = NULL;
  restartTime = 0;

  moveForward = false;
  onRecvMsgFunction = false; 


  // std::vector<std::string> s;

  // for(std::map<std::string, CParts *>::iterator it = tool->getPartsCollection().begin(); it != tool->getPartsCollection().end(); ++it){
  //   if (it->first != "body")
  //       s.push_back(it->first);
  //   }

  // std::string linkName; 
  // Size si;

  // Vector3d pos;

  // for (int i = 0; i < s.size(); i++){

  //   const char* linkName = s[i].c_str();
  //   CParts *link = tool->getParts(linkName);
  //   link->getPosition(pos);
  //   link->getRotation(linkRotation); 

  // }




  myfile.flush(); // I have uncommented the file, because I want to overwrite the file. 

  if (myfile.is_open())
  {
     myfile << "Action" << " , " << "FunctionalFeature" << " , " ;
     myfile << "forceOnTool_X" << " , " << "forceOnTool_Z " <<  " , ";
     myfile << "toolVelAtHit_X" << " , " << "toolVelAtHit_Z" << " , ";  
     myfile << "InitialToolPos_X" << " , " << "InitialToolPos_Z" << " , ";
     myfile << "FinalToolPos_X" << " , " << "FinalToolPos_Z" << " , ";
     myfile << "InitialLinkPos_X" << " , " << "InitialToolPos_Z" << " , ";
     myfile << "InitialTargetPos_X" << " , " << "InitialTargetPos_Z" << " , ";
     myfile <<  "targetFinalPos_X" << " , " << "targetFinalPos_Z"  << " , ";
     
     // myfile << "InitialTargetOri_X" << " , " << "InitialTargetOri_Z" << " , ";
     // myfile <<  "targetFinalOri_X" << " , " << "targetFinalOri_Z"  << " , ";

     myfile << "targetPlacement_X" << " , " << "targetPlacement_Z";
     myfile << "targetDisplacement_X" << " , " << "targetDisplacement_Z";

     // myfile << "linkInitialPos_X" << ", " << "linkInitialPos_Z" << " , "
     // myfile << "linkInitialOri_X" << ", " << "linkInitialOri_Z";
     myfile <<  "\n" ;


}



}  


  
double MyController::onAction(ActionEvent &evt) {  


  int actionNumber = 1;
  int functionalFeature = 3;
  myfile << setprecision(2) << std::fixed;


  SimObj *box = getObj("box_003");
  SimObj *TShapeTool = getObj("TShapeTool_003");
  SimObj *toolName = TShapeTool;
  SimObj *target = box;

  int xForceVariance = 500;
  int zForceVariance = 500;

  double forceOnTool_x, forceOnTool_z;

  forceOnTool_x = 0;
  // forceOnTool_x = -1000 - f_x;
  forceOnTool_z = 1000 + f_z;
  // forceOnTool_z = 0;

  Vector3d forceOnTool;
  forceOnTool.set(forceOnTool_x, 0 , forceOnTool_z);


  if( !didToolHit && isTargetAtInitLoc && isToolAtInitLoc && isToolAtInitPose && isTargetAtInitPose  )
  {
         // cout << "Into the loop" << endl;
         // cout << "The tapping force applied on tool is " <<  forceOnTool_x  << ", " << 0 << " , " << forceOnTool_z << endl; 
         maintainOrientationOfTool(toolName, initToolRotation);
         tapWithTool(toolName, initToolRotation, forceOnTool);

         // save the force applied on the tool.
  }

  


  if (didToolHit)
  {
      
        // save Linear velocity of tool at hit

   
        
        isToolAtRest   = checkEntityMotionStatus(toolName);  // checks whether the tool is moving by calculating its velocity
        isTargetAtRest = checkEntityMotionStatus(target);    // checks whether the object is moving by calculating its velocity

        if(isToolAtRest)
        {
          toolName->getPosition(currentToolPos);
          
        }

        if (isToolAtRest)
        {
 
          // reset the tool position to its initial location
            while (!isToolAtInitLoc) 
            {
              
              isToolAtInitLoc = resetEntityPosition(toolName, initToolPos);
              // cout << "Tool Position is reset  " << isToolAtInitLoc << endl; 
            }

            while (isToolAtInitLoc && !isToolAtInitPose)
            {

              isToolAtInitPose = resetEntityOrientation(toolName,initToolRotation);
              // newToolRotation = resetToNewToolRotation();
              if(isToolAtInitPose)
              {
                  // cout << "Tool Pose is set " << endl;
              }
              
            }
        }

      
       if (isTargetAtRest && isToolAtRest && isToolAtInitLoc && isToolAtInitPose)
       {

             target->getPosition(currentTargetPos);
          
              while(!isTargetAtInitLoc)
              {
                isTargetAtInitLoc = resetEntityPosition(target, initTargetPos);
                // cout << "Target position is reset  " << isTargetAtInitLoc << endl; 
              }

              while(isTargetAtInitLoc && !isTargetAtInitPose)
             {

                isTargetAtInitPose = resetRandomTargetOrientation(target);
                cout << "Target Orientation is Reset " << isTargetAtInitPose << endl;

              // isTargetAtInitPose = true;
             } 

            
            cout << "onAction " << &linkName << endl;
          
            link->getPosition(linkInitPos);  
       


       }

       if (isTargetAtRest && isTargetAtInitLoc && isTargetAtInitPose && isToolAtRest && isToolAtInitLoc && isToolAtInitPose)
       {
          
           didToolHit = false;

           myfile << actionNumber  << " , " << functionalFeature << " , " ;
           myfile << forceOnTool_x  << " , " << forceOnTool_z << " , " ;
           myfile << toolVelAtHit.x() << " , " << toolVelAtHit.z() << " , " ;
           myfile << initToolPos.x() << " , " << initToolPos.z() << " , " ;
           myfile << currentToolPos.x() << " , " << currentToolPos.z() << " , " ;
           myfile << linkInitPos.x() << " , " << linkInitPos.z() << " , " ; 
           myfile << initTargetPos.x() << " , " << initTargetPos.z() << " , " ;
           myfile << currentTargetPos.x() << " , " << currentTargetPos.z() << " , " ;
           myfile << linkInitPos.x() - initTargetPos.x() << " , " << linkInitPos.z() - initTargetPos.z() << " , ";
           myfile << currentTargetPos.x() -  initTargetPos.x() << " , " << currentTargetPos.z() - initTargetPos.z();
           myfile << "\n"; 
           
           simulationCount++;
           cout << "The simulation count = " << simulationCount << endl;

          // Reset the forces applied
          double r;
          r = ((double) rand() / (RAND_MAX)) ;
        
          cout << "f_x = " << f_x << endl;

          f_x = r * int(xForceVariance);
          f_z = r * int(zForceVariance);

        
       }

      }

      
  if (simulationCount == totalSamples)
     {

        myfile.close();
        cout << "The number of times too had been manipulated = " << totalSamples << endl;
        cout << "The simulation process has ended successfully " << endl;
        exit(0);
     }


  return 0.01;    

 }

        
std::string msg = " ";

void MyController::onRecvMsg(RecvMsgEvent &evt) {  

  }  

void MyController::onCollision(CollisionEvent &evt) { 

   SimObj *TShapeTool = getObj("TShapeTool_001");
   SimObj *toolName = TShapeTool;

   toolName->getLinearVelocity(toolVelAtHit);

   const vector<string> & wname  = evt.getWith();       // Get a name of the other  
   const vector<string> & wparts = evt.getWithParts();  // Get a parts name of the other's collision point  
   const vector<string> & mparts = evt.getMyParts();    // Get a parts of collision point of myself   
  
   Vector3d pos;
   didToolHit = true;

    for(int i = 0; i < wname.size(); i++)  
      {  

        // const char* linkName = mparts[i].c_str();
        linkName = mparts[i].c_str();
        link = toolName->getParts(linkName);
        // link->getPosition(pos);
        // cout << pos.x() << " , " << pos.z() << endl;
        // link->getRotation(linkRotation); 
        // CParts *link = tool->getParts(linkName);
        // link->getPosition(pos);
        // link->getRotation(linkRotation);  
    }

   
   isTargetAtRest = false;
   isToolAtRest = false;
   isToolAtInitLoc = false;
   isTargetAtInitLoc = false;
   isToolAtInitPose = false;
   isTargetAtInitPose = false;
   flag = 1;



}



  
extern "C" Controller * createController() {  
  return new MyController;  
}  

