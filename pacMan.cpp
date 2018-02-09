#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include "shaderload.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

World w;



void readFile() {
	std::ifstream inputFile;
	inputFile.open("levels/level0");
	unsigned int x;
	inputFile >> w.size.x; inputFile.ignore();
	inputFile >> w.size.y;

	while (!inputFile.eof()) {
		// read the map data from file
		for (int i = 0; i < w.size.x; i++) {
			for (int j = 0; j < w.size.y; j++) {
				inputFile >> x;
				w.map.push_back(x);
			}
			inputFile.ignore();
		}
	}
	// display the map in commandline
	for (std::vector<int>::size_type i = 0; i < w.map.size(); i++) {
		std::cout << w.map[i];
	}
	std::cout << w.size.x << "X" << w.size.y << "\n";
	std::cout << w.map.size();
	inputFile.close();
}

void setupOpengl(GLuint &vao, GLuint &vbo) {
	try {
		if (!glfwInit()) {	// Initialize GLFW
			throw 1;
		}
	}
	catch (int e){
		std::cout << "Failed to initialize GLFW! Exception nr: " << e << '\n';
	}
	// Window data
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //	MacOs. 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Old OpenGL is not acceptable.

	window = glfwCreateWindow(height, width, "Pacman", NULL, NULL);
	try {
		if (window == NULL) {
			glfwTerminate();
			throw 2;
		}
	}
	catch (int e) {
		std::cout << "Failed to open window! Exception nr: " << e << '\n';
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	try {
		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			getchar();
			glfwTerminate();
			throw 3;
		}
	}
	catch (int e) {
		std::cout << "Failed to initialize GLEW! Exception nr: " << e << '\n';
	}
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1);
	const int size = w.map.size();
	GLfloat mapVertices[1008];
	
	for (std::vector<GLfloat>::size_type i = 0; i < w.map.size(); i++) {
		mapVertices[i] = w.map[i];
	}

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mapVertices), mapVertices);

	GLuint shaderProgram = create_program("./shaders/vertex.vert", "./shaders/fragment.frag");

	GLint position = glGetAttribLocation(shaderProgram, "position");
	
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
	
	glEnableVertexAttribArray(position);

	glBindVertexArray(0);

}

void display(GLuint &vao) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);


}

int main() {
	GLuint vao;
	GLuint vbo;
	readFile();
	
	setupOpengl(vao, vbo);

	glClearColor(0.5f, 1.0f, 1.0f, 0.0f);
	// Display the map on window
	do {
		
		display(vao);
		glDisableVertexAttribArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//glBindVertexArray(vao);
	int i;
	std::cin >> i;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}