#include <GL/glew.h>
#include "GLFW/glfw3.h"

const unsigned int HEIGHT = 700; //1024;
const unsigned int WIDTH = 700; //768; 
unsigned int score;
const std::string textureFile = "./assets/pacman.png";
const std::string mapFile = "./levels/level0";
GLFWwindow* window;
