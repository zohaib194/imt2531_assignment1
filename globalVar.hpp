#include <iostream>
#include <vector>
#include "structs.hpp"
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Window info
GLFWwindow* window;
const unsigned int HEIGHT = 700; //1024;
const unsigned int WIDTH = 700; //768;

// Score info 
unsigned int score;

// World
float dt = 0.0f;
bool shouldRun = true;
bool pause = true;

// Collision toleranse
float collTolerance = 0.05f;

// Character info
World w;
PacMan pm;

const GLuint ghostCount=3;
Ghost gh[ghostCount];

// File info
const std::string textureFile = "./assets/pacman.png";
const std::string mapFile = "./levels/level0";

// Font info
const float textCharacterSize = 2.0f/10.0f;
const glm::vec2 playTextPos(-0.6f, 0.2f);
const glm::vec2 exitTextPos(-0.6f, 0.0f);
const glm::vec2 pauseTextPos(-1.0f, float(1 - textCharacterSize));
const glm::vec3 textColor(1.0f, 0.7f, 0.30f);
const std::string txt = "PLAY-P";
const std::string txt2 = "EXIT-E";
std::vector<GLfloat> txtPos;
glm::vec2 uv[256][4];

// Texture info
const glm::vec2 textureSize = glm::vec2(0.13f, 0.215f);		// Size of each figure in the texture
const glm::vec2 textureMargin = glm::vec2(0.015f, 0.03f);	// Margin around each figure.

// Animation info
const float animationDuration = 0.5;

