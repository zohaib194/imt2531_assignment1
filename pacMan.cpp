#include "structs.hpp"
#include "globalVar.hpp"
#include "template.hpp"
#include "shaderload.h"
#include "SOIL.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <time.h>

enum { VB_POSITION, VB_COLOR, NUM_BUFFERS };

bool shouldRun = true;

GLuint vaoMap;
GLuint vbo[NUM_BUFFERS];

GLuint vaoObj;
GLuint vboObj; // [NUM_BUFFERS];

World w;
PacMan pm;

// Global varaibles
GLuint shaderProgram;
GLuint textureShaderProg;

float dt = 0.0f;

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
                    pm.position = glm::vec2((j * (2 / w.size.x) - 1), (1 - i * (2 / w.size.y)));
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
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);

    // map vertices
    glm::vec2* vertices_position;
    glm::vec4* vertices_color;

    glm::vec4 tileColor;

    vertices_position = new glm::vec2[w.size.x * w.size.y * 6];
    vertices_color = new glm::vec4[w.size.x * w.size.y * 6];

    for (size_t i = 0; i < w.size.x * w.size.y; i++)
    {
        tileColor = ((w.map[i].z < 1 || w.map[i].z == 2) ? glm::vec4(0.98f, 0.15f, 0.45f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        vertices_position[6 * i + 0] = glm::vec2(w.map[i].x, w.map[i].y);
        vertices_color[6 * i + 0] = tileColor;

        vertices_position[6 * i + 1] = glm::vec2(w.map[i].x, w.map[i].y + (2 / w.size.y));
        vertices_color[6 * i + 1] = tileColor;

        vertices_position[6 * i + 2] = glm::vec2(w.map[i].x + (2 / w.size.x), w.map[i].y);
        vertices_color[6 * i + 2] = tileColor;

        vertices_position[6 * i + 3] = glm::vec2(w.map[i].x, w.map[i].y + (2 / w.size.y));
        vertices_color[6 * i + 3] = tileColor;

        vertices_position[6 * i + 4] = glm::vec2(w.map[i].x + (2 / w.size.x), w.map[i].y);
        vertices_color[6 * i + 4] = tileColor;

        vertices_position[6 * i + 5] = glm::vec2(w.map[i].x + (2 / w.size.x), w.map[i].y + (2 / w.size.y));
        vertices_color[6 * i + 5] = tileColor;
    }

    shaderProgram = create_program("./shaders/vertex.vert", "./shaders/fragment.frag");

    textureShaderProg = create_program("./shaders/vertexTex.vert", "./shaders/fragmentTex.frag");

    // Load texture
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int texWidth, texHeight;
    unsigned char* image;
    image = SOIL_load_image("./assets/pacman.png", &texWidth, &texHeight, 0, SOIL_LOAD_AUTO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glGenVertexArrays(1, &vaoMap);

    glBindVertexArray(vaoMap);

    glGenBuffers(NUM_BUFFERS, vbo);

    // !!!! Make a buffer for VERTEX POSITIONS !!!!
    // Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VB_POSITION]);

    // Put the given data in buffer
    glBufferData(GL_ARRAY_BUFFER, w.size.x * w.size.y * 6 * 16, &vertices_position[0], GL_STATIC_DRAW);

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

    // !!!! Make a buffer for VERTEX POSITIONS !!!!
    // Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboObj);

    // Put the given data in buffer
    glBufferData(GL_ARRAY_BUFFER, w.size.x * w.size.y * 6 * 16, NULL, GL_STATIC_DRAW);

    // Enable buffer 'VB_POSITION' for use
    glEnableVertexAttribArray(0);

    // Tell OpenGL how to use the enabled buffer
    glVertexAttribPointer(VB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vaoMap);

    //glDrawElements(GL_TRIANGLES, 200, GL_UNSIGNED_INT, indices);
    //glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

    // GL_TRIANGLE_STRIP WAS THE PROBLEM
    glDrawArrays(GL_TRIANGLES, 0, 6 * w.size.x * w.size.y);

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
        pm.position += pm.direction * dt;
        std::cout << "PM pos: " << "(" << pm.position.x << ", " << pm.position.y << "). dt = " << dt << "\n";
    }
    else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN))
    {
        // Set direction to down
        pm.direction = glm::vec2(0.0f, -pm.speed);

        // Move pacman a little in that direction
        pm.position += pm.direction * dt;
        std::cout << "PM pos: " << "(" << pm.position.x << ", " << pm.position.y << "). dt = " << dt << "\n";
    }
    else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT))
    {
        // Set direction to up
        pm.direction = glm::vec2(-pm.speed, 0.0f);

        // Move pacman a little in that direction
        pm.position += pm.direction * dt;
        std::cout << "PM pos: " << "(" << pm.position.x << ", " << pm.position.y << "). dt = " << dt << "\n";
    }
    else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT))
    {
        // Set direction to up
        pm.direction = glm::vec2(pm.speed, 0.0f);

        // Move pacman a little in that direction
        pm.position += pm.direction * dt;
        std::cout << "PM pos: " << "(" << pm.position.x << ", " << pm.position.y << "). dt = " << dt << "\n";
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