#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
}fs_in;

struct Light {
	vec4 position;
	vec3 color;
};
#define MAX_LIGHTS 4

uniform sampler2D _Texture;
uniform Light _Lights[MAX_LIGHTS];

void main(){
	FragColor = texture(_Texture,fs_in.UV);
}