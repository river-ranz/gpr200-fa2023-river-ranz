#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
}fs_in;

struct Light {
	vec4 position;
	vec3 color;
};
#define MAX_LIGHTS 4

uniform sampler2D _Texture;
uniform Light _Lights[MAX_LIGHTS];
uniform float _AmbientK, _DiffuseK, _Specular, _Shininess;

void main(){
	vec3 normal = normalize(fs_in.worldNormal);
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	for (int i = 0; i < MAX_LIGHTS; i++) {
		vec3 ambVal = _Lights[i].color * _AmbientK;
		ambient += ambVal;
		float direction = max(dot(_Lights[i].position.xyz, normal), 0.0f);
		vec3 diffVal = _Lights[i].color * (direction * _DiffuseK);
		diffuse += diffVal;
	}
	vec3 textureColor = texture(_Texture, fs_in.UV).xyz;
	vec3 lightColor = vec3(textureColor * (ambient + diffuse));
	FragColor = vec4(lightColor, 1.0f);
}