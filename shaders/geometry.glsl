#version 150


layout(points) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 Color;

out vec3 outColor;

void main()
{
	for(int i = 0; i < 3; i++){
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
		EndPrimitive();
 }