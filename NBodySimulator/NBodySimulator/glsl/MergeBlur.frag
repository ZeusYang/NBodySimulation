#version 330 core

in vec2 Texcoord;

out vec4 fragColor;

uniform sampler2D Color;
uniform sampler2D BrightColor;

void main(){
	vec3 hdrColor = (texture(Color, Texcoord).rgb + texture(BrightColor, Texcoord).rgb);
    fragColor = vec4(hdrColor, 1.0f);
}