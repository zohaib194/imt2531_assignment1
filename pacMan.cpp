#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include "glew\glew.h"
#include "glfw\glfw3.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

World w;

void readFile() {
	std::ifstream inputFile;
	inputFile.open("levels/level0");
	int x, y;
	inputFile >> w.size.x; inputFile.ignore();
	inputFile >> w.size.y;
	// allocate memory for map
	w.map = new unsigned int*[w.size.x];
	for (int c = 0; c < w.size.x; c++) {
		w.map[c] = new unsigned int[w.size.y];
	}
	while (!inputFile.eof()) {

		// read the map data from file
		for (int i = 0; i < w.size.x; i++) {
			for (int j = 0; j < w.size.y; j++) {
				inputFile >> w.map[i][j];
			}
			inputFile.ignore();
		}
	}
	// display the map in commandline
	for (int i = 0; i < w.size.x; i++) {
		for (int j = 0; j < w.size.y; j++) {
			std::cout << w.map[i][j];
		}
	}
	std::cout << w.size.x << "X" << w.size.y << "\n";

	inputFile.close();
}


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
	
	
	readFile();

	// Display the map on window
//	GLuint vao;
	//glBindVertexArray(vao);

	std::cin >> score;
	glfwDestroyWindow(window);
	return 0;
}