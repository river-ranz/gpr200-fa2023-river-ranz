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
uniform vec3 _CameraPos;
uniform int _Phong, _NumLights;

void main(){
	vec3 normal = normalize(fs_in.worldNormal);
	vec3 cameraPos = normalize(_CameraPos - fs_in.worldPosition);
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	vec3 lightColor = vec3(0);
	
	for (int i = 0; i < _NumLights; i++) {
		vec3 lightPos = normalize(_Lights[i].position.xyz - fs_in.worldPosition);
		ambient = _Lights[i].color * _AmbientK;
		
		float direction = max(dot(normal, lightPos), 0.0f);
		diffuse = _Lights[i].color * direction * _DiffuseK;
		
		if (_Phong == 0) {
			vec3 halfV = normalize(lightPos + cameraPos);
			float angle = max(dot(halfV, normal), 0.0f);
			specular = _Lights[i].color * _Specular * pow(angle, _Shininess);
		}
		else {
			vec3 reflection = reflect(-lightPos, normal);
			float spec = pow(max(dot(reflection, cameraPos), 0.0f), _Shininess);
			specular = _Specular * spec * _Lights[i].color;
		}
		
		lightColor += ambient + diffuse + specular;
	}
	vec4 textureColor = texture(_Texture, fs_in.UV);
	FragColor = vec4(textureColor.rgb * lightColor, 1.0f);
}