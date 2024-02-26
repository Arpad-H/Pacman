#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;
layout(location = 3) in vec4 InstancePosition;
layout(location =4) in mat4 InstanceRotation;

out vec3 Normal;
out vec3 Position;
out vec2 Texcoord;

uniform mat4 ModelViewProjMat;
uniform mat4 ModelMat;

void main()
{
	vec4 worldPos = InstanceRotation*vec4(VertexPos.xyz, 1.0) ; 
     Position =  worldPos.xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * vec4(worldPos.xyz,1.0);
}  

