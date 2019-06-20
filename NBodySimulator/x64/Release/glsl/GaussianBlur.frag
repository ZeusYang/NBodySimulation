#version 330 core

in vec2 Texcoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

uniform int horizontal;
uniform sampler2D Color;
uniform sampler2D Bright;
//const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//const float weight[6] = float[] (0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093);

//const float weight[7] = float[] (0.197641,0.174868,0.121117,0.065666,0.027867,0.009255,0.002406);
const float weight[8] = float[] (0.197448, 0.174697, 0.120999, 0.065602, 0.02784, 0.009246, 0.002403, 0.000489);

void main(){
    vec2 tex_offset = 1.0 / textureSize(Bright, 0); // gets size of single texel
    vec3 result = texture(Bright, Texcoord).rgb * weight[0];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 1 * horizontal, tex_offset.y * 1 * (1-horizontal))).rgb * weight[1];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 1 * horizontal, tex_offset.y * 1 * (1-horizontal))).rgb * weight[1];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 2 * horizontal, tex_offset.y * 2 * (1-horizontal))).rgb * weight[2];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 2 * horizontal, tex_offset.y * 2 * (1-horizontal))).rgb * weight[2];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 3 * horizontal, tex_offset.y * 3 * (1-horizontal))).rgb * weight[3];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 3 * horizontal, tex_offset.y * 3 * (1-horizontal))).rgb * weight[3];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 4 * horizontal, tex_offset.y * 4 * (1-horizontal))).rgb * weight[4];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 4 * horizontal, tex_offset.y * 4 * (1-horizontal))).rgb * weight[4];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 5 * horizontal, tex_offset.y * 5 * (1-horizontal))).rgb * weight[5];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 5 * horizontal, tex_offset.y * 5 * (1-horizontal))).rgb * weight[5];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 6 * horizontal, tex_offset.y * 6 * (1-horizontal))).rgb * weight[6];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 6 * horizontal, tex_offset.y * 6 * (1-horizontal))).rgb * weight[6];

	result += texture(Bright, Texcoord + vec2(tex_offset.x * 7 * horizontal, tex_offset.y * 7 * (1-horizontal))).rgb * weight[7];
    result += texture(Bright, Texcoord - vec2(tex_offset.x * 7 * horizontal, tex_offset.y * 7 * (1-horizontal))).rgb * weight[7];

    brightColor = vec4(result, 1.0);
	fragColor = texture(Color, Texcoord);	
}