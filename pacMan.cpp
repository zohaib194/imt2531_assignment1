#include "template.hpp"
#include "shaderload.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <time.h>


enum { VB_POSITION, VB_COLOR, VB_TEXTURE, NUM_BUFFERS };

GLuint vaoMap;
GLuint vbo[NUM_BUFFERS];

GLuint vaoObj;
GLuint veoObj;
GLuint vboObj; // [NUM_BUFFERS];
GLuint texture[2];
//GLuint fontTexture;
GLuint vaoFont;
GLuint vboFont;
GLuint veoFont;

// Global varaibles
GLuint shaderProgram;
GLuint textureShaderProg;
GLuint fontTextureShaderProg;

// Function declarations
void readFile();
void setupOpengl();
void display();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void pause_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


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



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "mouse button clicked\n";
		glfwSetKeyCallback(window, key_callback);
	}

}

void textData(const std::string txt, const glm::vec2 button) {
	int k = 0;
	for (auto i : txt)
	{

		// Position
		txtPos.push_back(button.x + textCharacterSize * (k + 0));
		txtPos.push_back(button.y - textCharacterSize);
		// Color
		txtPos.push_back(textColor.x);
		txtPos.push_back(textColor.y);
		txtPos.push_back(textColor.z);
		// Texture Coordinates
		txtPos.push_back(uv[i][1].x);
		txtPos.push_back(uv[i][1].y);
		
		

		txtPos.push_back(button.x + textCharacterSize * (k + 0));
		txtPos.push_back(button.y);
		txtPos.push_back(textColor.x);
		txtPos.push_back(textColor.y);
		txtPos.push_back(textColor.z);
		txtPos.push_back(uv[i][0].x);
		txtPos.push_back(uv[i][0].y);


		txtPos.push_back(button.x + textCharacterSize *(k + 1));
		txtPos.push_back(button.y - textCharacterSize);
		txtPos.push_back(textColor.x);
		txtPos.push_back(textColor.y);
		txtPos.push_back(textColor.z);
		txtPos.push_back(uv[i][3].x);
		txtPos.push_back(uv[i][3].y);

		


		txtPos.push_back(button.x + textCharacterSize * (k + 1));
		txtPos.push_back(button.y);
		txtPos.push_back(textColor.x);
		txtPos.push_back(textColor.y);
		txtPos.push_back(textColor.z);
		txtPos.push_back(uv[i][2].x);
		txtPos.push_back(uv[i][2].y);

		
		k++;


	}
}

void setupOpengl() {
    try {
        if (!glfwInit()) {  // Initialize GLFW
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOs. 
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	fontTextureShaderProg = create_program("./shaders/vertexTexFont.vert", "./shaders/fragmentTexFont.frag");

	// Load texture
	glGenTextures(2, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	int texWidth, texHeight, numComponents;
	unsigned char* image;
	stbi_set_flip_vertically_on_load(true);
	image = stbi_load("./assets/pacman.png", &texWidth, &texHeight, &numComponents, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	if (!image) {
		std::cout << "Unable to load pacman texture\n";
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(image);
	glUniform1i(glGetUniformLocation(textureShaderProg, "tex"), 0);

	

	// font
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	int fontTexWidth, fontTexHeight, numComponentsFont;
	unsigned char* fontImage;
	fontImage = stbi_load("./assets/font.png", &fontTexWidth, &fontTexHeight, &numComponentsFont, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontTexWidth, fontTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, fontImage);
	if (!fontImage) {
		std::cout << "Unable to load font image\n";
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(fontImage);
	glUniform1i(glGetUniformLocation(fontTextureShaderProg, "texFont"), 0);

    // Pacman text initial coordiantes
    pm.texCoord[0] = glm::vec2(0.02f, 0.03f);
    pm.texCoord[1] = glm::vec2(0.02f, 0.245f);
    pm.texCoord[2] = glm::vec2(0.15f, 0.03f);
    pm.texCoord[3] = glm::vec2(0.15f, 0.245f);


    // Pacman initial position
    pm.position[0] = glm::vec2(pm.position[0].x, pm.position[0].y - (2 / w.size.y));
    pm.position[1] = glm::vec2(pm.position[0].x, pm.position[0].y + (2 / w.size.y));
    pm.position[2] = glm::vec2(pm.position[0].x + (2 / w.size.y), pm.position[0].y);
    pm.position[3] = glm::vec2(pm.position[0].x + (2 / w.size.y), pm.position[0].y + (2 / w.size.y));

    // Reference position for ghost
    for (int i = 0; i < ghostCount; ++i)
    {
        gh[i].position[0] = glm::vec2(0.0f + (textureSize.x/2)*i, 0.0f);

        switch(i%4){
            case 0:
                gh[i].direction = glm::vec2(0.0f, 0.1f);
                break;

            case 1:
                gh[i].direction = glm::vec2(0.1f, 0.0f);
                break;

            case 2:
                gh[i].direction = glm::vec2(0.0f, -0.1f);
                break;

            case 3:
                gh[i].direction = glm::vec2(-0.1f, 0.0f);
                break;
        }
    }

    // Position relative to reference for ghost
    for (int i = 0; i < ghostCount; ++i)
    {
        gh[i].position[1] = glm::vec2(gh[i].position[0].x, gh[i].position[0].y + (2 / w.size.y));
        gh[i].position[2] = glm::vec2(gh[i].position[0].x + (2 / w.size.y), gh[i].position[0].y);
        gh[i].position[3] = glm::vec2(gh[i].position[0].x + (2 / w.size.y), gh[i].position[0].y + (2 / w.size.y));
    }

    // texture for ghost
    for (int i = 0; i < ghostCount; ++i)
    {
        gh[i].texCoord[0] = glm::vec2(0.69f, 0.03f);
        gh[i].texCoord[1] = glm::vec2(0.69f, 0.245f);
        gh[i].texCoord[2] = glm::vec2(0.81f, 0.03f);
        gh[i].texCoord[3] = glm::vec2(0.81f, 0.245f);

        // offset animation
        gh[i].animationTime = i * 0.03f;
    }
    


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


	


	/* fonts vao and vbo  */
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			uv[int((i * 16) + j)][0] = glm::vec2(j      * (1.0f / 16.0f), (16.0f - i) * (1.0f / 16.0f));
			uv[int((i * 16) + j)][1] = glm::vec2(j      * (1.0f / 16.0f), (16.0f - (i + 1)) * (1.0f / 16.0f));
			uv[int((i * 16) + j)][2] = glm::vec2((j + 1.0f) * (1.0f / 16.0f), (16.0f - i) * (1.0f / 16.0f));
			uv[int((i * 16) + j)][3] = glm::vec2((j + 1.0f) * (1.0f / 16.0f), (16.0f - (i + 1)) * (1.0f / 16.0f));
		}
	}

	std::cout << playTextPos.x + textCharacterSize << '\n';

	/*
	
	for (size_t i = 0; i < 256; i++)
	{	
		for (size_t j = 0; j < 4; j++)
		{
			std::cout << uv[i][j].x << "  " << uv[i][j].y << '\n';
		}
	}
	*/
	
	textData(txt, playTextPos);
	textData(txt2, exitTextPos);

	glGenVertexArrays(1, &vaoFont);
	glBindVertexArray(vaoFont);

	glGenBuffers(1, &vboFont);

	glBindBuffer(GL_ARRAY_BUFFER, vboFont);


	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * txtPos.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * txtPos.size(), &txtPos[0]);

	glGenBuffers(1, &veoFont);


	GLuint order[] = {
		0, 1, 2,
		1, 2, 3,
	
		4, 5, 6,
		5, 6, 7,
		
		8, 9, 10,
		9, 10, 11,
		
		12, 13, 14,
		13, 14, 15, 

		16, 17, 18,
		17, 18, 19,

		20, 21, 22,
		21, 22, 23,

		24, 25, 26,
		25, 26, 27,

		28, 29, 30,
		29, 30, 31,

		32, 33, 34,
		33, 34, 35,

		36, 37, 38,
		37, 38, 39,

		40, 41, 42,
		41, 42, 43,

		44, 45, 46,
		45, 46, 47,

	};
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veoFont);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * 24, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(order), &order);


	glEnableVertexAttribArray(VB_POSITION);

	glVertexAttribPointer(VB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void *)0);

	glEnableVertexAttribArray(VB_COLOR);

	glVertexAttribPointer(VB_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void *)(2 * sizeof(GLfloat)));

	glEnableVertexAttribArray(VB_TEXTURE);

	glVertexAttribPointer(VB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void *)(5 * sizeof(GLfloat)));

	


}


void dynamic_code(){
	
	/* PACMAN, GHOSTS and Special food*/
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE0);
	
    
    /* PACMAN, GHOSTS and Special food*/
    animate<PacMan>(pm, 2);

    for (int i = 0; i < ghostCount; ++i)
    {
        animate<Ghost>(gh[i], 1);
    }
    
    GLfloat pacmanData[(7*4)*(ghostCount+1)];

    // Adding pacman to pacmanData
    for (int i = 0; i < 4; ++i)
    {   // possition x                          y
        pacmanData[7*i+0] = pm.position[i].x; pacmanData[7*i+1] = pm.position[i].y;

        // Color R                  G                           B
        pacmanData[7*i+2] = 1.0f; pacmanData[7*i+3] = 1.0f; pacmanData[7*i+4] = 1.0f;

        // Texture      x                       y
        pacmanData[7*i+5] = pm.texCoord[i].x + pm.texOffset.x; pacmanData[7*i+6] = pm.texCoord[i].y + pm.texOffset.y;
    }

        // ghost position           color           texture coord
    for (int j = 0; j < ghostCount; ++j)
    {
        
        for (int i = 0; i < 4; ++i)
        {   // possition x                          y
            pacmanData[(j+1)*(7*4)+(7*i+0)] = gh[j].position[i].x; pacmanData[(j+1)*(7*4)+(7*i+1)] = gh[j].position[i].y;

            // Color R                  G                           B
            pacmanData[(j+1)*(7*4)+(7*i+2)] = 1.0f; pacmanData[(j+1)*(7*4)+(7*i+3)] = 1.0f; pacmanData[(j+1)*(7*4)+(7*i+4)] = 1.0f;

            // Texture      x                       y
            pacmanData[(j+1)*(7*4)+(7*i+5)] = gh[j].texCoord[i].x + gh[j].texOffset.x; pacmanData[(j+1)*(7*4)+(7*i+6)] = gh[j].texCoord[i].y + gh[j].texOffset.y;
        }
    }

    GLuint order[] = {
        0, 1, 2,
        1, 2, 3,

        4, 5, 6,
        5, 6, 7,

        8, 9, 10,
        9, 10, 11,

        12, 13, 14,
        13, 14, 15
    };

	/* Pacman vao, vbo and veo init */
	glGenVertexArrays(1, &vaoObj);
	glBindVertexArray(vaoObj);

	glGenBuffers(1, &vboObj);


    glBindBuffer(GL_ARRAY_BUFFER, vboObj);

    glBufferData(GL_ARRAY_BUFFER, sizeof(pacmanData), NULL, GL_STATIC_DRAW);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pacmanData), &pacmanData);
    
    
    //glBufferSubData(GL_ARRAY_BUFFER, sizeof(pacman_vertices), sizeof(texCoord), &texCoord);
    
    
    
    // !!!! Make a buffer for VERTEX POSITIONS !!!!
    // Bind the buffer
	glGenBuffers(1, &veoObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veoObj);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(GLuint) * 3) * ((ghostCount+1)*2), NULL, GL_STATIC_DRAW);
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
	
	
	/* Map */
	glUseProgram(shaderProgram);
	glBindVertexArray(vaoMap);
	glDrawArrays(GL_TRIANGLES, 0, 6 * w.size.x * w.size.y);

	/* Pacman and Ghost*/
	glUseProgram(textureShaderProg);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vaoObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veoObj);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (const GLvoid*)0);
	
	if (pause) {
		/* Font */
		glUseProgram(fontTextureShaderProg);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glActiveTexture(GL_TEXTURE1);
		glBindVertexArray(vaoFont);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veoFont);
		glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, (const GLvoid*)0);
	}

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);


}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //glm::vec4 coll = checkCollision<PacMan>(pm);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        shouldRun = false;

    // Can pacman move up AND was UP or W pressed?
    else if ((key == GLFW_KEY_W || key == GLFW_KEY_UP))// && (!coll.y))
    {
        // Set direction to up
        pm.direction = glm::vec2(0.0f, pm.speed);
    }

    // Can pacman move down AND was DOWN or S pressed?
    else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN)) // && (!coll.w))
    {
        // Set direction to down
        pm.direction = glm::vec2(0.0f, -pm.speed);
    }

    // Can pacman move left AND was LEFT or A pressed?
    else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT)) // && (!coll.x))
    {
        // Set direction to left
        pm.direction = glm::vec2(-pm.speed, 0.0f);
    }

    // Can pacman move right AND was RIGHT or D pressed?
    else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)) // && (!coll.z))
    {
        // Set direction to right
        pm.direction = glm::vec2(pm.speed, 0.0f);
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        // Register a keyboard event callback function
        glfwSetKeyCallback(window, pause_callback);

        // The game should pause
        pause = true;
    }
}

void pause_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        // Register a keyboard event callback function
        glfwSetKeyCallback(window, key_callback);

        // The game should unpause
        pause = false;
    }
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		shouldRun = false;
	}
}


	

int main() {
    // Read map data from file
    readFile();

    // Setup OpenGL
    setupOpengl();

    // Register a keyboard event callback function
    glfwSetKeyCallback(window, key_callback);

	// Register mouse button event callback function
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set pacman's direction to right
    pm.direction = glm::vec2(pm.speed, 0.0f);

    glm::vec4 coll;

    // Display the map on window    
    do {

        // Get elapsed time
        dt = glfwGetTime();

        // Reset glfw time
        glfwSetTime(0.0);

        // Poll events from queue, so OS won't kill me
        glfwPollEvents();

        glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

        dynamic_code();


        // Should I pause
        if (!pause)
        {
            coll = checkCollision<PacMan, int>(pm);
            /*if ((!coll.y) && pm.direction.y > 0 ||
                (!coll.w) && pm.direction.y < 0 ||
                (!coll.x) && pm.direction.x < 0 ||
                (!coll.y) && pm.direction.x > 0)*/
            {
                // Move pacman a little in that direction
                pm.position[0] += pm.direction * dt;
                pm.position[1] += pm.direction * dt;
                pm.position[2] += pm.direction * dt;
                pm.position[3] += pm.direction * dt;
            }

            for (int i = 0; i < ghostCount; ++i)
            {
                gh[i].position[0] += gh[i].direction * dt;
                gh[i].position[1] += gh[i].direction * dt;
                gh[i].position[2] += gh[i].direction * dt;
                gh[i].position[3] += gh[i].direction * dt;
            }
        }

        // Display everything
        display();

        // SWAP THE BUFFERS!
        glfwSwapBuffers(window);


    } while (shouldRun && glfwWindowShouldClose(window) == 0);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
