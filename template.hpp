#include "globalVar.hpp"
#include <iostream>

// animate takes use a struct with texOffset and animationTime.
// It takes the number of states in the texture for the struct in each direction.
// frameCount should never be zero.

template <class animatable>
void animate(animatable &ani, int frameCount){
 
	int state;
	ani.animationTime += dt;
	//std::cout << ani.animationTime << std::endl;
	if (ani.animationTime < animationDuration && ani.animationTime != 0.0f)
	{	
		// playing forward
		if (animationDuration > ani.animationTime / 2)
		{
			state = floor( ani.animationTime / (animationDuration / (frameCount *2)));
		}
		else	//playing backwards
		{
			state = frameCount * 2 - floor( ani.animationTime / (animationDuration / (frameCount *2)));		
		}

	}
	else
	{	
	 	state = 0;
		ani.animationTime = 0.0;
	}
	//std::cout << state << std::endl;
	ani.texOffset.x = textureSize.x * state + textureMargin.x * state * 2; 
}

template <class T>
glm::vec4 collision(T obj)
{
    int i = (obj.position[0].y / (2 / w.size.y)) - 1, j = (obj.position[0].x / (2 / w.size.x)) + 1;
    return glm::vec4((w.map[int((j    ) * w.size.x + (i - 1))].z == 1),
                     (w.map[int((j + 1) * w.size.x + (i    ))].z == 1),
                     (w.map[int((j    ) * w.size.x + (i + 1))].z == 1),
                     (w.map[int((j - 1) * w.size.x + (i    ))].z == 1));
    // TODO: 
    // - use found i and j to look up actual world position within w.map, and check agains pacmans entire body collides with something, not just if the next tile is a wall
    // - Make function take in object to check agains, maybe also the xy size of the object
}