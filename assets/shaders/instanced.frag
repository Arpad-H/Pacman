#version 400 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;

void main()
{             

	vec3 normPos = normalize(Position.xyz);
FragColor = vec4(0,0,abs(normPos.z), 1.0); 
}