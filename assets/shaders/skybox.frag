#version 400 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, texCoords);
   // FragColor = vec4(1.0f,1.0f,1.0f,1);
} 