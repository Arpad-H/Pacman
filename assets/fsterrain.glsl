#version 400
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D MixTex; // for exercise 3
uniform sampler2D DetailTex[4]; // for exercise 3
uniform vec3 Scaling;
uniform int FactorK;

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
out vec4 FragColor;

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}


void main()
{
    vec3 N      = normalize(Normal);
    vec3 L      = normalize(LightPos); // light is treated as directional source
    vec3 D      = EyePos-Position;
    float Dist  = length(D);
    vec3 E      = D/Dist;
    vec3 R      = reflect(-L,N);
    
    vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N,L));
    vec3 SpecularComponent = LightColor * SpecularColor * pow( sat(dot(R,E)), SpecularExp);
    
    // Exercise 3
  
    vec4 alpha = texture(MixTex, Texcoord);
 
    vec2 Texcoord2 = Texcoord * FactorK;
    vec4 t1 = texture(DetailTex[1], Texcoord2);
    vec4 t0;
    vec4 t2;

    vec4 reflexionColor = vec4(((DiffuseComponent + AmbientColor) + SpecularComponent),1);
	
	//Exercise 5
	float BeginSand = 0;
	float EndSand = 10;
	float BeginSnow = 20;
	float EndSnow = 30;
	float alpha2;
	//
	//0=Grass
	//1=rock
	//2=sand
	//3=snow
	//
	if (Position.y <= BeginSand){                         //only sand
	     t0 = texture(DetailTex[2], Texcoord2);
		 FragColor = mix(t0,t1,alpha) * reflexionColor;
	}
	else if (Position.y > BeginSand && Position.y < EndSand){ //sand grass mix
	     t0 = texture(DetailTex[0], Texcoord2);
	     t2 = texture(DetailTex[2], Texcoord2);
		 alpha2 = (Position.y-EndSand)/(BeginSand-EndSand);
		 FragColor =mix(mix(t0,t1,alpha),t2,alpha2) * reflexionColor;
	}
	else if (Position.y < BeginSnow && Position.y > EndSand ){ //only gras
		t0 = texture(DetailTex[0], Texcoord2);
		FragColor = mix(t0,t1,alpha) * reflexionColor;
	}
	else if (Position.y > BeginSnow && Position.y < EndSnow){						//gras snow mix
		 t0 = texture(DetailTex[3], Texcoord2);
	     t2 = texture(DetailTex[0], Texcoord2);
		 alpha2 = (Position.y-BeginSnow)/(EndSnow-BeginSnow);
		 FragColor =mix(mix(t2,t0,alpha2),t1,alpha) * reflexionColor;
	}
	else if (Position.y > EndSnow){						//snow only
		 t0 = texture(DetailTex[3], Texcoord2);
		 FragColor =mix(t0,t1,alpha) * reflexionColor;
	}
    

	//Exercise 4
    //Atmospheric fog
	float dmin = 0;
	float dmax = 50;
	float a = 3;
	vec3 cfog = vec3(0.95, 0.95 ,1);
	
    float s = clamp(pow( (Dist - dmin) / (dmax - dmin), a), 0, 1);
    FragColor = (1-s)*FragColor + s * vec4(cfog,1);

}
