#version 400

in vec4 vColor;
in vec2 vTexture;

out vec4 outColor;

uniform sampler2D tex;

void main()
 {
	//outColor = vColor;
	outColor = texture(tex, vTexture);
	
 }