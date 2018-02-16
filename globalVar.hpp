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

// Character info
World w;
PacMan pm;

// File info
const std::string textureFile = "./assets/pacman.png";
const std::string mapFile = "./levels/level0";

// Texture info
const glm::vec2 textureSize = glm::vec2(0.13f, 0.215f);		// Size of each figure in the texture
const glm::vec2 textureMargin = glm::vec2(0.015f, 0.03f);	// Margin around each figure.

// Animation info
const float animationDuration = 0.5;