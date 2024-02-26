#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;
//layout(location=3) in vec4 InstancePosition;
//layout(location=4) in mat4 InstanceRotation;

layout(std430, binding = 1) buffer OffsetBuffer 
{
    vec3 offsets[]; 
};

out vec3 Normal;
out vec3 Position;
out vec2 Texcoord;

uniform mat4 ModelViewProjMat;

void main()
{
	int instanceID = gl_InstanceID; 
    vec4 worldPos = VertexPos + offsets[instanceID];
     Position =  (VertexPos + InstancePosition).xyz;
	 Normal = worldPos.xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * worldPos;
}  

