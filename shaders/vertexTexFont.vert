#version 400

in vec2 position;
in vec3 color;
in vec2 texture;

out vec4 vColor;
out vec2 vTexture;



void main()
 {
 	gl_Position = vec4(position, 0.0f, 1.0f);
    vColor = vec4(color, 1.0f);
	vTexture = texture;
 }