#version 400 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;

uniform vec3 CameraPos;
uniform samplerCube EnviromentCube;
uniform int override;

void main()
{             
    vec3 I = normalize(Position - CameraPos);
    vec3 R = reflect(I, normalize(I));
	 vec3 reflectedColor = texture(EnviromentCube, R).rgb;
	 
	 vec4 col;
	 if (override == 1){
	  col = vec4(clamp(Position,0.0,1.0), 1.0);
	 }
	 else{
	   col = vec4(reflectedColor*0.3, 1.0); 
	 }
	FragColor = col;
	//FragColor = vec4(1,1,1, 1.0); 
	//FragColor =  vec4(reflectedColor*0.3, 1.0); 
}