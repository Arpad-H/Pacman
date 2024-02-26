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
out vec2 id;
uniform mat4 ModelViewProjMat;
uniform mat4 ModelMat;
void main()
{
	int instanceID = gl_InstanceID; 
	id.x = instanceID;
    vec4 worldPos =(VertexPos + vec4(offsets[instanceID].xyz,1));
	//vec3 worldPos = (ModelMat * VertexPos).xyz;
     Position =  offsets[instanceID].xyz;
	 Normal = worldPos.xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * worldPos;
}  

