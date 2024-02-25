#version 400 core


out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;

uniform sampler2D diffuseTex;

void main()
{             
	
	 vec4 textureColor = texture(diffuseTex, Texcoord);
    // Adjust the intensity of the emissive effect by multiplying with a factor
    vec3 emissiveColor = textureColor.rgb * 2.0; // Adjust intensity by changing the multiplier
    
    // Output emissive color
  //  FragColor = vec4(emissiveColor, 1.0);
    FragColor = vec4(1,1,0, 1.0);
}