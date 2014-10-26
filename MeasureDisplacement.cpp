#include "MeasureDisplacement.h"

void measureTargetDisplacement(SimObj *target, Vector3d initTargetPos) 
{
	Vector3d currentTargetPosition;
	target->getPosition(currentTargetPosition);
	cout << currentTargetPosition.x() << endl;
	cout << currentTargetPosition.z() << endl;
}  