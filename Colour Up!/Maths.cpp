#include "Maths.h"


bool isOppositeDirOfTravel(float currentDir, float intendedDir){
	
		return currentDir * intendedDir <0;   
	
}

double getHypotenuse(double adjacent, double opposite){
	return sqrt(adjacent * adjacent + opposite*opposite);
}
