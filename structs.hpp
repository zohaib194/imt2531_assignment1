#include "glm/glm.hpp"
#include "GL/glew.h"
#include <vector>
#include <string>

struct PacMan
{
	glm::vec2 position[4]; 
    glm::vec2 direction;
    glm::vec2 texCoord[4];
    float speed = 2.0f;
};

struct Ghost
{
	glm::vec2 position[4]; 
    glm::vec2 direction;
    glm::vec2 texCoord[4];
};

struct World
{
	std::vector<glm::vec3> map;
	glm::vec2 size;
//	unsigned int map[];

};

struct Food
{
	glm::vec2 position;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
    float radius = 2.0f;
    int points = 1;
};
