#include "structs.hpp"
#include "globalVar.hpp"
#include <iostream>

// animate takes use a struct with texOffset and animationTime.
// It takes the number of states in the texture for the struct in each direction.
// frameCount should never be zero.

template <class animatable>
void animate(animatable &ani, int frameCount){

	int state;
	ani.animationTime += dt;
	std::cout << ani.animationTime << std::endl;
	if (ani.animationTime < animationDuration)
	{	
		// playing forward
		if (animationDuration > ani.animationTime / 2)
		{
			state = floor((animationDuration / frameCount *2) * ani.animationTime);
		}
		else	//playing backwards
		{
			state = frameCount * 2 - floor((animationDuration / frameCount *2) * ani.animationTime);		
		}

	}
	else
	{	
	 	state = 0;
		ani.animationTime = 0.0;
	}

	ani.texOffset.x = textureSize.x * state + textureMargin.x * state * 2; 
}