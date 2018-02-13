#version 400

in vec4 position;
in vec4 color;
in vec2 texture;

out vec4 vColor;
out vec2 vTexture;

void main()
 {
 	gl_Position = position;
    vColor = color;
	vTexture = texture;
 }