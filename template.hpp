#include "globalVar.hpp"
#include <iostream>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define CHECK_FOR_WALL(X, Y) ((w.map[X].z == 1) ? ((Y < collTolerance) && (Y > -collTolerance)) : 1)

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
    else  //playing backwards
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

/**
    This function does either collision with map (walls) isn't specified OR
    if it is given a object in "bObj" it does collition between "aObj" and "bObj"

    @param "aObj" the object to check on
    @param "bObj" DEFUALT NULL (map/walls get's checked), if not NULL second object to check against

    @return glm::vec4 with as a bool value for all collision directions (x = up, y = down, z = left, w = right)
*/
template<class A, class B>
glm::vec4 checkCollision(const A aObj, const B bObj = NULL)
{
    // Find indexes
    int i = int((-1 * w.size.y * aObj.position[0].y) / 2 + (w.size.y / 2)) % int(w.size.y),
        j = int((w.size.x * aObj.position[0].x) / 2 + (w.size.x / 2)) % int(w.size.x);

    // Finding map indexes to check against
    int mapIndex[4] = { (((i + 1) * w.size.x) + j),
                        (((i - 1) * w.size.x) + j),
                        ((i * w.size.x) + (j - 1)),
                        ((i * w.size.x) + (j + 1)) };

        // Finding outer bounds of map tile THIS IS ONLY FOR ONE. Duplicate this for every checked tile 
        int mapCollBox[4] = { w.map[mapIndex[UP]].y,
                              w.map[mapIndex[DOWN]].y - (2 / w.size.y),
                              w.map[mapIndex[LEFT]].x,
                              w.map[mapIndex[RIGHT]].x + (2 / w.size.x) };

        // Map pacman collition points
        int pacCollBox[4] = { aObj.position[0].y,
                              aObj.position[1].y,
                              aObj.position[0].x,
                              aObj.position[3].x };

    // Map or object collision testing?
    if (bObj != NULL)
    {
        // Give current collision state for aObj and bObj
        return glm::vec4(1/*Coll state up*/,
                         1/*Coll state down*/,
                         1/*Coll state left*/,
                         1/*Coll state right*/);
    }
    else
    {
        // Give current collision state for aObj and map
        return glm::vec4( CHECK_FOR_WALL(mapIndex[UP],    (pacCollBox[UP]    - mapCollBox[UP])),
                          CHECK_FOR_WALL(mapIndex[DOWN],  (pacCollBox[DOWN]  - mapCollBox[DOWN])),
                          CHECK_FOR_WALL(mapIndex[LEFT],  (pacCollBox[LEFT]  - mapCollBox[LEFT])),
                          CHECK_FOR_WALL(mapIndex[RIGHT], (pacCollBox[RIGHT] - mapCollBox[RIGHT])));
    }
}

// Set Annimation direction
template <class animatable>
void rotate(animatable &ani){


    // checks for pm.direction == 1.0f, but use > 0.5 to alleviate floating point imprecision
    if (ani.direction.x > 0.5f){ // Right
    
       ani.texOffset.y = (textureSize.y * 0) + (textureMargin.y * 0);
    }
    else if(ani.direction.x < -0.5f){ // Left
       ani.texOffset.y = (textureSize.y * 1) + (textureMargin.y * 2);   
    }
    else if(ani.direction.y > 0.5f){ // Up
       ani.texOffset.y = (textureSize.y * 2) + (textureMargin.y * 4);
    }
    else if(ani.direction.y < -0.5f){ // Down
       ani.texOffset.y = (textureSize.y * 3) + (textureMargin.y * 6);
    }
}