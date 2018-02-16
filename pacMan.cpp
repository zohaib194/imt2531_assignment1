#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include "shaderload.h"
//#include "SOIL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
//#include <iostream>
#include <iomanip>
#include <time.h>

enum { VB_POSITION, VB_COLOR, VB_TEXTURE, NUM_BUFFERS };

GLuint vaoMap;
GLuint vbo[NUM_BUFFERS];

GLuint vaoObj;
GLuint veoObj;
GLuint vboObj; // [NUM_BUFFERS];
GLuint texture;

// Global varaibles
GLuint shaderProgram;
GLuint textureShaderProg;

// Function declarations
void readFile();
void setupOpengl();
void display();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void pause_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
				w.map.push_back(glm::vec3((j * (2 / w.size.x) - 1), (1 - i * (2 / w.size.y)), x));

				// Find tile of type 2 and set it to pacmans starting pos
				if (x == 2)
					pm.position[0] = glm::vec2((j * (2 / w.size.x) - 1), (1 - i * (2 / w.size.y)));
			}
			inputFile.ignore();
		}
	}
	inputFile.close();
}

void setupOpengl() {
	try {
		if (!glfwInit()) {	// Initialize GLFW
			throw 1;
		}
	}
	catch (int e) {
		std::cout << "Failed to initialize GLFW! Exception nr: " << e << '\n';
	}
	// Window data
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1);

	// map vertices
	glm::vec2* vertices_position;
	glm::vec4* vertices_color;

	glm::vec4 tileColor;

	vertices_position = new glm::vec2[int(w.size.x * w.size.y * 6)];
	vertices_color = new glm::vec4[int(w.size.x * w.size.y * 6)];

	for (size_t i = 0; i < w.size.x * w.size.y; i++)
	{
		tileColor = ((w.map[i].z < 1 || w.map[i].z == 2) ? glm::vec4(0.98f, 0.15f, 0.45f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

		vertices_position[6 * i + 0] = glm::vec2(w.map[i].x, w.map[i].y);
		vertices_color[6 * i + 0] = tileColor;

		vertices_position[6 * i + 1] = glm::vec2(w.map[i].x, w.map[i].y - (2 / w.size.y));
		vertices_color[6 * i + 1] = tileColor;

		vertices_position[6 * i + 2] = glm::vec2(w.map[i].x + (2 / w.size.x), w.map[i].y);
		vertices_color[6 * i + 2] = tileColor;

		vertices_position[6 * i + 3] = glm::vec2(w.map[i].x, w.map[i].y - (2 / w.size.y));
		vertices_color[6 * i + 3] = tileColor;

		vertices_position[6 * i + 4] = glm::vec2(w.map[i].x + (2 / w.size.x), w.map[i].y);
		vertices_color[6 * i + 4] = tileColor;

		vertices_position[6 * i + 5] = glm::vec2(w.map[i].x + (2 / w.size.x), w.map[i].y - (2 / w.size.y));
		vertices_color[6 * i + 5] = tileColor;
	}

	shaderProgram = create_program("./shaders/vertex.vert", "./shaders/fragment.frag");

	textureShaderProg = create_program("./shaders/vertexTex.vert", "./shaders/fragmentTex.frag");

	// Load texture

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);

	int texWidth, texHeight, numComponents;
	unsigned char* image;
	stbi_set_flip_vertically_on_load(true);
	image = stbi_load("./assets/pacman.png", &texWidth, &texHeight, &numComponents, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	if (!image) {
		std::cout << "you suck";
	}
	stbi_image_free(image);

	
	//image = SOIL_load_image("./assets/pacman.png", &texWidth, &texHeight, 0, SOIL_LOAD_AUTO);
	//stbi_set_flip_vertically_on_load(1);
	//SOIL_free_image_data(image);

	//std::cout << texHeight << " " << texWidth << " " << numComponents << '\n';

	glUniform1i(glGetUniformLocation(textureShaderProg, "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	pm.texCoord[0] = glm::vec2(0.02f, 0.03f);
	pm.texCoord[1] = glm::vec2(0.02f, 0.245f);
	pm.texCoord[2] = glm::vec2(0.15f, 0.03f);
	pm.texCoord[3] = glm::vec2(0.15f, 0.245f);


	pm.position[1] = glm::vec2(pm.position[0].x, pm.position[0].y - (2 / w.size.y));
	pm.position[2] = glm::vec2(pm.position[0].x + (2 / w.size.y), pm.position[0].y);
	pm.position[3] = glm::vec2(pm.position[0].x + (2 / w.size.y), pm.position[0].y - (2 / w.size.y));

	glGenVertexArrays(1, &vaoMap);

	glBindVertexArray(vaoMap);

	glGenBuffers(NUM_BUFFERS, vbo);

	// !!!! Make a buffer for VERTEX POSITIONS !!!!
	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VB_POSITION]);

	// Put the given data in buffer
	glBufferData(GL_ARRAY_BUFFER, w.size.x * w.size.y * 6 * sizeof(glm::vec2), &vertices_position[0], GL_STATIC_DRAW);

	// Enable buffer 'VB_POSITION' for use
	glEnableVertexAttribArray(VB_POSITION);

	// Tell OpenGL how to use the enabled buffer
	glVertexAttribPointer(VB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// !!!! Make a buffer for the COLORS !!!!
	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VB_COLOR]);

	// Put the given data in buffer
	glBufferData(GL_ARRAY_BUFFER, w.size.x * w.size.y * 6 * 16, &vertices_color[0], GL_STATIC_DRAW);

	// Enable buffer 'VB_COLOR' for use
	glEnableVertexAttribArray(VB_COLOR);

	// Tell OpenGL how to use the enabled buffer
	glVertexAttribPointer(VB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);



	glGenVertexArrays(1, &vaoObj);
	glBindVertexArray(vaoObj);

	glGenBuffers(1, &vboObj);


}

void dynamic_code(){
	
	/* PACMAN, GHOSTS and Special food*/
    animate<PacMan>(pm, 2);
    //std::cout << pm.texCoord[0].x + pm.texOffset.x <<" " << pm.texCoord[0].y + pm.texOffset.y << std::endl;
	GLfloat pacmanData[] = {
		// pacman position			colors			texture coord
		pm.position[0].x, pm.position[0].y, 1.0f, 1.0f, 1.0f, pm.texCoord[0].x + pm.texOffset.x, pm.texCoord[0].y + pm.texOffset.y,													// 0,1
		pm.position[1].x, pm.position[1].y, 1.0f, 1.0f, 1.0f, pm.texCoord[1].x + pm.texOffset.x, pm.texCoord[1].y + pm.texOffset.y,								// 0,0
		pm.position[2].x, pm.position[2].y, 1.0f, 1.0f, 1.0f, pm.texCoord[2].x + pm.texOffset.x, pm.texCoord[2].y + pm.texOffset.y,								// 1,1
		pm.position[3].x, pm.position[3].y, 1.0f, 1.0f, 1.0f, pm.texCoord[3].x + pm.texOffset.x, pm.texCoord[3].y + pm.texOffset.y,		// 1,0
	};

	GLuint order[] = {
		0, 1, 2,
		1, 2, 3
	};

	
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(pacmanData), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pacmanData), &pacmanData);
	
	
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pacman_vertices), sizeof(texCoord), &texCoord);
	
	
	glGenBuffers(1, &veoObj);
	
	// !!!! Make a buffer for VERTEX POSITIONS !!!!
	// Bind the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veoObj);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * 2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(order), &order);
	
	
	// Put the given data in buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pacman_vertices) + sizeof(texCoord), NULL, GL_STATIC_DRAW);
	
	
	// Enable buffer 'VB_POSITION' for use
	glEnableVertexAttribArray(VB_POSITION);
	
	// Tell OpenGL how to use the enabled buffer
	glVertexAttribPointer(VB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void *)0);
	
	glEnableVertexAttribArray(VB_COLOR);
	
	glVertexAttribPointer(VB_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void *)(2 * sizeof(GLfloat)));
	
	glEnableVertexAttribArray(VB_TEXTURE);
	
	glVertexAttribPointer(VB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void *)(5 * sizeof(GLfloat)));
	
	
}



void display() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(vaoMap);



	// GL_TRIANGLE_STRIP WAS THE PROBLEM
	glDrawArrays(GL_TRIANGLES, 0, 6 * w.size.x * w.size.y);

	glUseProgram(textureShaderProg);


	//glBindVertexArray(vaoObj);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	glBindVertexArray(vaoObj);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veoObj);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        shouldRun = false;
    else if ((key == GLFW_KEY_W || key == GLFW_KEY_UP))
    {
        // Set direction to up
        pm.direction = glm::vec2(0.0f, pm.speed);

        // Move pacman a little in that direction
		pm.position[0] += pm.direction * dt;
		pm.position[1] += pm.direction * dt;
		pm.position[2] += pm.direction * dt;
		pm.position[3] += pm.direction * dt;

		std::cout << "PM pos: " << "(" << pm.position[0].x << ", " << pm.position[0].y << "). dt = " << dt << "\n";
    }
    else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN))
    {
        // Set direction to down
        pm.direction = glm::vec2(0.0f, -pm.speed);

        // Move pacman a little in that direction
        pm.position[0] += pm.direction * dt;
		pm.position[1] += pm.direction * dt;
		pm.position[2] += pm.direction * dt;
		pm.position[3] += pm.direction * dt;

        std::cout << "PM pos: " << "(" << pm.position[0].x << ", " << pm.position[0].y << "). dt = " << dt << "\n";
    }
    else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT))
    {
        // Set direction to up
        pm.direction = glm::vec2(-pm.speed, 0.0f);

        // Move pacman a little in that direction
        pm.position[0] += pm.direction * dt;
		pm.position[1] += pm.direction * dt;
		pm.position[2] += pm.direction * dt;
		pm.position[3] += pm.direction * dt;

        std::cout << "PM pos: " << "(" << pm.position[0].x << ", " << pm.position[0].y << "). dt = " << dt << "\n";
    }
    else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT))
    {
        // Set direction to up
        pm.direction = glm::vec2(pm.speed, 0.0f);

        // Move pacman a little in that direction
        pm.position[0] += pm.direction * dt;
		pm.position[1] += pm.direction * dt;
		pm.position[2] += pm.direction * dt;
		pm.position[3] += pm.direction * dt;

		std::cout << "PM pos: " << "(" << pm.position[0].x << ", " << pm.position[0].y << "). dt = " << dt << "\n";
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        // Register a keyboard event callback function
        glfwSetKeyCallback(window, pause_callback);
    }
}

void pause_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        // Register a keyboard event callback function
        glfwSetKeyCallback(window, key_callback);
    }
}





int main() {
	readFile();

    setupOpengl();

    // Register a keyboard event callback function
    glfwSetKeyCallback(window, key_callback);

    // Get time at start
    dt = glfwGetTime();

    // Display the map on window	
    do {
        //glDisableVertexAttribArray(0);
        glfwPollEvents();


        glClearColor(0.5f, 1.0f, 1.0f, 0.0f);

		dynamic_code();

        // Get elapsed time
        dt = glfwGetTime() - dt;
        display();
		
        glfwSwapBuffers(window);
        dt = glfwGetTime();

    } while (shouldRun && glfwWindowShouldClose(window) == 0);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
