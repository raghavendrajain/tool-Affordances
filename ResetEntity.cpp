#include "ResetEntity.h"
#include "ControlPosition.h"

bool resetEntityPosition(SimObj *entity, Vector3d desiredPosition){

  Vector3d currentPos;
  entity->getPosition(currentPos);	
 

  Vector3d differenceInPos;
  differenceInPos.x(desiredPosition.x() - currentPos.x());
  differenceInPos.y(desiredPosition.y() - currentPos.y());
  differenceInPos.z(desiredPosition.z() - currentPos.z());

  double* ptr1 = NULL;
  // cout << "The desiredPosition = " << desiredPosition.x() << "  ,  " << desiredPosition.z() << endl ; 
  ptr1 = controlPosition(desiredPosition, currentPos, 4.0,0.2, 1.2);
  // cout << "The velocity should be applied on tool for position reset is  " << ptr1[0] << " , " << ptr1[1] << endl; 
  double xVel = ptr1[0];
  double yVel = ptr1[1];
  // cout << "xVel = " << xVel << endl;
  // cout << "yVel = " << yVel << endl;


  entity->setLinearVelocity( xVel , 0, yVel );
    
  // Vector3d currentLinearVelocity;  
  // entity->getLinearVelocity(currentLinearVelocity);
  // cout << "currentLinearVelocity = " << currentLinearVelocity.x() << " , " << currentLinearVelocity.z() << endl;


  // LOG_MSG((" current entity pos is : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));

  if ( abs(differenceInPos.x()) < 0.1 && abs(differenceInPos.z()) < 0.1)
  {
            // LOG_MSG((" Tool reset at : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));
            // LOG_MSG (("The difference in position is: %f, %f, %f ", differenceInPos.x(), differenceInPos.y(), differenceInPos.z() )) ; 
            differenceInPos.set(0,0,0);
            // cout << "The entity is reset" << endl;
            return true;

  }


}


// bool resetTargetEntityPosition(SimObj *target,  SimObj *tool, Vector3d initToolPos, Vector3d desiredTargetPos){

//   Vector3d currentPos;
//   entity->getPosition(currentPos);  

//   std::vector<std::string> s;
//   for(std::map<std::string, CParts *>::iterator it = tool->getPartsCollection().begin(); it != tool->getPartsCollection().end(); ++it){
//     if (it->first != "body")
//         s.push_back(it->first);
//     }

//   std::string linkName; 
//   Size si;
//   Vector3d linkPos;

//   for (int i = 0; i < s.size(); i++){

//     const char* linkName = s[i].c_str();
//     CParts *link = tool->getParts(linkName);
//     if(strcmp(linkName ,"LINK1")==0)
//     {
//       link->getPosition(linkPos);
//       if (currentPos.x() > linkPos.x() )
//     }
//   }



   
  








 

//   Vector3d differenceInPos;
//   differenceInPos.x(desiredPosition.x() - currentPos.x());
//   differenceInPos.y(desiredPosition.y() - currentPos.y());
//   differenceInPos.z(desiredPosition.z() - currentPos.z());

//   double* ptr1 = NULL;
//   // cout << "The desiredPosition = " << desiredPosition.x() << "  ,  " << desiredPosition.z() << endl ; 
//   ptr1 = controlPosition(desiredPosition, currentPos, 4.0,0.2, 1.2);
//   // cout << "The velocity should be applied on tool for position reset is  " << ptr1[0] << " , " << ptr1[1] << endl; 
//   double xVel = ptr1[0];
//   double yVel = ptr1[1];
//   // cout << "xVel = " << xVel << endl;
//   // cout << "yVel = " << yVel << endl;


//   entity->setLinearVelocity( xVel , 0, yVel );
    
//   // Vector3d currentLinearVelocity;  
//   // entity->getLinearVelocity(currentLinearVelocity);
//   // cout << "currentLinearVelocity = " << currentLinearVelocity.x() << " , " << currentLinearVelocity.z() << endl;


//   // LOG_MSG((" current entity pos is : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));

//   if ( abs(differenceInPos.x()) < 0.2 && abs(differenceInPos.z()) < 0.2)
//   {
//             // LOG_MSG((" Tool reset at : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));
//             // LOG_MSG (("The difference in position is: %f, %f, %f ", differenceInPos.x(), differenceInPos.y(), differenceInPos.z() )) ; 
//             differenceInPos.set(0,0,0);
//             // cout << "The entity is reset" << endl;
//             return true;

//   }


// }






void maintainOrientationOfTool(SimObj * entity, Rotation initialToolRot)
{
    double torque;
    Vector3d angularVel;
    entity->getAngularVelocity(angularVel); 

    if ( abs(angularVel.y()) > 0.1  )
    {
        // LOG_MSG((" Current angular Velocity is  : %f %f %f ", angularVel.x(), angularVel.y(), angularVel.z() ));
        double* ptr1 = NULL;
        ptr1 = controlAngularVelocity(angularVel, 10.0, 0, 8.0);
        torque  =  ptr1[0] * 10000;
        // torque =  ptr1[0] * 12000 ;
        // cout << "The torque applied for controlling angular velocity is " << torque << "   N. m" << endl; 
        entity->addTorque( 0 , torque, 0);
   } 

    Rotation currentToolRot;
    entity->getRotation(currentToolRot);
    double *ptrTool = NULL;
    ptrTool = controlRotationThroughout(initialToolRot, currentToolRot, 20.0, 0.0, 20.0);
    torque  =  ptrTool[1] * 10000 ;
    // cout << "The torque applied for controlling the rotation = " << torque << endl;
    entity->addTorque(0, torque,0); 

   
}


bool resetEntityOrientation(SimObj *entity, Rotation goalRotation){

  double *ptrTool = NULL;
  double torque;
  ptrTool = controlRotation(entity, goalRotation, 20.0, 0.4, 20.0);
  // cout << "The difference in entity orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 10000 ;
  // cout << " torque = " << torque << endl;
  entity->addTorque(0, torque, 0);

  if ( abs(ptrTool[3]) < 0.05)
  {
    return true;
  }

}


bool resetRandomTargetOrientation(SimObj *entity){

  double *ptrTool = NULL;
  double torque;
  Rotation currentRot;
  entity->getRotation(currentRot);

  ptrTool = reachZeroPose(entity, currentRot, 1.0, 0.0, 0.2);
  // cout << "The difference in entity orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 100 ;
  // cout << " torque = " << torque << endl;
  entity->addTorque(0, torque, 0);

  if ( abs(ptrTool[3]) < 0.1)
  {
   
    cout << " The entity is set to a new pose" << endl;
    return true;
    // exit(0);

  }


}


float randomFloat(float min, float max) {
    
    static bool init = false;
    if (!init) {
    srand(time(NULL));
    init = true;
  }
    
  return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min ;
}

bool newEntityPose(SimObj *entity, Rotation initToolRotation, double randomStep)
{

   

  double *ptrTool = NULL;
  double torque;
  // entity->addTorque(0, 10000, 0);

  // ptrTool = controlRotation(entity, initToolRotation, 1.0, 0.4, 1.2);
  ptrTool = reachRotation(entity, initToolRotation, randomStep, 20, 0, 20); 
  // cout << "The difference in entity orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 1000 ;
  // cout << " torque = " << torque << endl;
  entity->addTorque(0, torque, 0);

  if ( abs(ptrTool[3]) < 0.1)
  {
   
    
    cout << " The entity is set to a new pose" << endl;
    return true;
    // exit(0);

  }



}