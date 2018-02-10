#version 400


//in vec3 color;
in vec2 position;

out vec4 vColor;

void main()
 {
    vColor = vec4(1.0, 0.5, 0.5, 0.0);
	gl_Position = vec4(position, 1.0, 1.0);
 }