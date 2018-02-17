#version 400

in vec4 vColor;
in vec2 vTexture;

out vec4 outColor;

uniform sampler2D texFont;

void main()
 {
//outColor = vColor;
	outColor = vColor * texture(texFont, vTexture);
	
 }