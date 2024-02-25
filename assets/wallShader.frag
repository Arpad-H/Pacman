#version 400 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;
uniform vec3 CameraPos;
uniform samplerCube EnviromentCube;
uniform sampler2D BaseColorTexture;  
uniform sampler2D RoughnessTexture;  
uniform sampler2D AmbientOcclusionTexture;

const float SpecularIntensity = 0.5;
void main()
{             
	// Sample base color and roughness textures
    vec4 baseColor = texture(BaseColorTexture, Texcoord);
    float roughness = texture(RoughnessTexture, Texcoord).r;
	float ambientOcclusion = texture(AmbientOcclusionTexture, Texcoord).r;
	ambientOcclusion *= 0.5;
    vec3 I = normalize(Position - CameraPos);
    vec3 R = reflect(I, normalize(Normal));
	
	// Apply roughness to the reflection vector
    R = normalize(R + vec3(roughness));  // Adjust the roughness value as needed

    vec3 reflectedColor = texture(EnviromentCube, R).rgb;
    reflectedColor *= (1.0 - ambientOcclusion);
	
	vec3 V = normalize(CameraPos - Position);
    float specularStrength = pow(max(dot(R, V), 0.0), 32.0) * SpecularIntensity;
	
	 // Blend the reflected color with the base color based on metallic factor
    vec3 finalColor = mix(baseColor.rgb, reflectedColor + vec3(specularStrength), roughness);

	FragColor = vec4(finalColor, 1.0);
}