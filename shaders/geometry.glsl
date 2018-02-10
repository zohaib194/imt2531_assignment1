#version 400


layout(points) in;
layout(triangle_strip, max_vertices=3) out;

in vec4 vColor;

out vec4 gColor;

void main()
{
/*
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	gl_Position.x = gl_in[1].gl_Position.x + 0.5;
	gl_Position.y = gl_in[1].gl_Position.y + 0.1;
	EmitVertex();

	gl_Position.x = gl_in[2].gl_Position.x + 0.5;
	gl_Position.y = gl_in[2].gl_Position.y - 0.1;
	EmitVertex();
*/
	gl_Position = vec4(-0.5, 0.0, 0.0, 0.0);
	gColor = vColor;
	EmitVertex();

	gl_Position = vec4(0.0, 0.5, 0.0, 0.0);
	gColor = vColor;
	EmitVertex();

	gl_Position = vec4(0.5, 0.0, 0.0, 0.0);
	gColor = vColor;
	EmitVertex();

	EndPrimitive();
 
 }