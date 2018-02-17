#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include <iostream>
#include <vector>

const unsigned int HEIGHT = 700; //1024;
const unsigned int WIDTH = 700; //768; 
unsigned int score;
const std::string textureFile = "./assets/pacman.png";
const std::string mapFile = "./levels/level0";
GLFWwindow* window;
float textCharacterSize = 2.0f/10.0f;
glm::vec2 playTextPos(-1.0f/2.0f + 0.1f, 0.2f);
glm::vec2 exitTextPos(-1.0f/2.0f + 0.1f, 0.0f);
glm::vec2 pauseTextPos(-1.0f, float(1 - textCharacterSize));
glm::vec3 textColor(1.0f, 0.7f, 0.30f);
std::vector<GLfloat> txtPos;
glm::vec2 uv[256][4];
