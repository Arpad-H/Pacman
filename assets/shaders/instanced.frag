#version 400 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;
in vec2 id;

void main()
{             

	vec3 normPos = normalize(Position.xyz);
	float i = clamp(id.x, 0,1);
FragColor = vec4(normPos, 1.0); 
}