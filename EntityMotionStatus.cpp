#include "ControllerEvent.h"  
#include "Controller.h"  
#include "Logger.h"  
#include <iostream>

#include "EntityMotionStatus.h"

using namespace std; 

bool checkEntityMotionStatus(SimObj * entity)
{

	  Vector3d velocityOfEntity;
  	  entity->getLinearVelocity(velocityOfEntity);
  
	  double netVelocity;
	  netVelocity=( pow(velocityOfEntity.x(),2) + pow(velocityOfEntity.y(), 2) + pow(velocityOfEntity.z(), 2 ) );
	  netVelocity = sqrt(netVelocity);	

	  if (netVelocity < 0.001)
	  {
	  	// cout << "The entity has stopped" << endl;
	  	return true;
	  }

	  else{
	  	return false;
	  }
	  
}