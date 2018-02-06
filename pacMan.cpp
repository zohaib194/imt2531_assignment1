#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

World w;

void readFile() {
	std::ifstream inputFile;
	inputFile.open("levels/level0");
	int x, y;
	inputFile >> x; inputFile.ignore();
	inputFile >> y;
	// allocate memory for map
	w.map = new unsigned int*[x];
	for (int c = 0; c < x; c++) {
		w.map[c] = new unsigned int[y];
	}
	while (!inputFile.eof()) {

		// read the map data from file
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				inputFile >> w.map[i][j];
			}
			inputFile.ignore();
		}
	}
	// display the map in commandline
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << w.map[i][j];
		}
	}

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