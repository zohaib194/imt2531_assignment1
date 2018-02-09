#version 150

//in vec3 Color;
//in vec2 Texcoord;
out vec4 outColor;
//uniform sampler2D texOne;
//uniform sampler2D texTwo;
void main()
 {
    // outColor = mix(texture(texOne, Texcoord), texture(texTwo, Texcoord), 0.5);
	// outColor = texture(texOne, Texcoord); 
	outColor = vec4(1,1,1,0);
 }