#version 400

layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;
uniform vec3 Scaling;

void main()
{
    // Apply scaling on terrain model
    vec4 scaledPosition = vec4(VertexPos.xyz * Scaling, 1.0);
    Position = (ModelMat * scaledPosition).xyz;

    // Apply scaling to normal vector
    vec3 scaledNormal = normalize((ModelMat * vec4(VertexNormal.xyz, 0)).xyz/Scaling);
    Normal = scaledNormal;

    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * scaledPosition;
}