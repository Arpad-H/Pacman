#version 460 core
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;
//layout(location=3) in vec4 InstancePosition;
//layout(location=4) in mat4 InstanceRotation;

layout(std430, binding = 1) buffer ssbo 
{
    vec4 offsets[]; 
};

out vec3 Normal;
out vec3 Position;
out vec2 Texcoord;

uniform mat4 ViewProjMat;
uniform mat4 ModelMat;
void main()
{
	 
    vec4 worldPos =(ModelMat*vec4(VertexPos.xyz,1) + vec4(offsets[gl_InstanceID].xyz,1));
    Position =  worldPos.xyz;
	Normal = VertexNormal.xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ViewProjMat * vec4(worldPos.xyz,1);
}  

