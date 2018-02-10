#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include "shaderload.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

World w;

// Global varaibles

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
				w.map.push_back(glm::vec3((i * (2/w.size.x)) - 1, (j * (2/w.size.y) - 1), x));
			}
			inputFile.ignore();
		}
	}
	// display the map in commandline
/*	for (int i = 0; i < w.size.x * w.size.y; i++) {
		std::cout << "i: " << w.map[i].x << " j: " <<  w.map[i].y << " value: " << w.map[i].z << '\n'; 
	} */
	//std::cout << w.size.x << "X" << w.size.y << "\n";
	//std::cout << w.map.size();
	inputFile.close();
}

void setupOpengl(GLuint &vao, GLuint &vbo) {
	try {
		if (!glfwInit()) {	// Initialize GLFW
			throw 1;
		}
	}
	catch (int e) {
		std::cout << "Failed to initialize GLFW! Exception nr: " << e << '\n';
	}
	// Window data
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //	MacOs. 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Old OpenGL is not acceptable.

	window = glfwCreateWindow(HEIGHT, WIDTH, "Pacman", NULL, NULL);
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


	//const int size = w.map.size();




	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::vec2 test[] = {
		glm::vec2(-0.5, 0.5),
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(test[0]), &test[0], GL_STATIC_DRAW);

	// Transfer the map vertices position
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(test[0]), &test[0]);

	// Transfer the vertex colors:
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(mapVertices), sizeof(mapColors), mapColors);

	GLuint shaderProgram = create_program("./shaders/vertex.vert", "./shaders/fragment.frag", "./shaders/geometry.glsl");

	GLint position = glGetAttribLocation(shaderProgram, "position");
	
	glEnableVertexAttribArray(position);

	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, 0);
	

	// Color attribute
//	GLint color = glGetAttribLocation(shaderProgram, "color");
//	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
//	glEnableVertexAttribArray(color);

	//glBindVertexArray(0);

}


void display(GLuint &vao) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glBindVertexArray(vao);
		//glDrawArrays(GL_POINTS, 0, 1);
		glUseProgram(0);
	
	glBindVertexArray(0);



}

int main() {
	GLuint vao;
	GLuint vbo;
	readFile();
	
	setupOpengl(vao, vbo);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1);

	glClearColor(0.5f, 1.0f, 1.0f, 0.0f);
	// Display the map on window
	do {
		
		//display(vao);
		//glDisableVertexAttribArray(0);
		display(vao);
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);

		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);



	//glBindVertexArray(vao);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}