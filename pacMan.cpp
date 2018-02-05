#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <../glew/glew.h>
#include <../glfw/glfw3.h>

//holding the screen up
#include <iostream>

int main() {

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW!");
		return -1;
	}
	// Window data
	glfwWindowHint(GLFW_SAMPLES, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //	MacOs. 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Old OpenGL is not acceptable.

	GLFWwindow* window;
	window = glfwCreateWindow(height, width, "Pacman", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open window");
		glfwTerminate();
		return -1;
	}
	
	std::ifstream inputFile; 
	inputFile.open("./levels/level0");


	
	std::cin >> score;
	glfwDestroyWindow(window);
	return 0;
}