#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include "shaderload.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <time.h>

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
		for (int i = 0; i < w.size.y; i++) {
			for (int j = 0; j < w.size.x; j++) {
				inputFile >> x;
				w.map.push_back(glm::vec3((i * (2/w.size.y) - 1), (j * (2/w.size.x) - 1), x));
			}
			inputFile.ignore();
		}
	}
	// display the map in commandline
	/*for (int i = 0; i < w.size.x * w.size.y; i++) {
		std::cout << "y: " << w.map[i].y << " x: " <<  w.map[i].x << " /*value: " << w.map[i].z << '\n'; 
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

	GLfloat* vertices_position;/*[] = {
		0.1, 0.1,
		0.5, 0.5,
		0.1, 0.5,

		-0.1, 0.1,
		-0.1, 0.5,
		-0.5, 0.5,

		0.1, -0.1,
		0.1, -0.5,
		0.5, -0.5,

		-0.1, -0.1,
		-0.5, -0.5,
		-0.1, -0.5,
	};*/

	vertices_position = new GLfloat[w.size.x*w.size.y * 8];
	
	for (size_t i = 0; i < w.size.x*w.size.y; i++)
	{
		vertices_position[8 * i + 0] = w.map[i].x;
		vertices_position[8 * i + 1] = w.map[i].y;

		vertices_position[8 * i + 2] = w.map[i].x + (2 / w.size.x);
		vertices_position[8 * i + 3] = w.map[i].y;

		vertices_position[8 * i + 4] = w.map[i].x + (2 / w.size.x);
		vertices_position[8 * i + 5] = w.map[i].y + (2 / w.size.y);

		vertices_position[8 * i + 6] = w.map[i].x;
		vertices_position[8 * i + 7] = w.map[i].y + (2 / w.size.y);
	}
	for (int k = 0; k < w.size.x*w.size.y * 2; k++) {
		std::cout << vertices_position[k*2 + 0] << " " << vertices_position[k * 2 + 1] << "\n";
	}
	GLfloat colors[36];
	srand(time(NULL));

	// Fill colors with random numbers from 0 to 1, use continuous polynomials for r,g,b:
	int k = 0;
	for (int i = 0; i < sizeof(colors) / sizeof(float) / 3; ++i) {
		float t = (float)rand() / (float)RAND_MAX;
		colors[k] = 9 * (1 - t)*t*t*t;
		k++;
		colors[k] = 15 * (1 - t)*(1 - t)*t*t;
		k++;
		colors[k] = 8.5*(1 - t)*(1 - t)*(1 - t)*t;
		k++;

	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(colors), &vertices_position, GL_STATIC_DRAW);

		// Transfer the vertex colors:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(colors), colors);

	// Transfer the vertex colors:
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(mapVertices), sizeof(mapColors), mapColors);

	GLuint shaderProgram = create_program("./shaders/vertex.vert", "./shaders/fragment.frag");

	GLint position = glGetAttribLocation(shaderProgram, "position");

	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(position);




	// Color attribute
	GLint color_attribute = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(color_attribute, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	glEnableVertexAttribArray(color_attribute);

	//glBindVertexArray(0);

}


void display(GLuint &vao) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
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