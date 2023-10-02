#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _CatTexture;
uniform float _Time;

void main(){
	FragColor = texture(_CatTexture, UV);
}