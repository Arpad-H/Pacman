#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Normal;
out vec3 Position;
out vec2 Texcoord;

uniform mat4 ModelViewProjMat;
uniform mat4 ModelMat;

void main()
{
     Position = (ModelMat * VertexPos).xyz;
    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
    //Tangent = (ModelMat * vec4(inTangent.xyz,0)).xyz;
    //BiTangent = (ModelMat * vec4(inBiTangent.xyz,0)).xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * VertexPos;
}  

