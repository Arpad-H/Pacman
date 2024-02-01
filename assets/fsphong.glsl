#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

in vec3 Tangent;
in vec3 BiTangent;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

const int MAX_LIGHTS=14;
struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
	int ShadowIndex;
};

uniform Lights 
{
	int LightCount;
	Light lights[MAX_LIGHTS];
};


float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{

    vec4 DiffTex = texture( DiffuseTexture, Texcoord);
    if(DiffTex.a <0.3f) discard;
	
	
	//Aufgabe 3
	vec4 NormalTex = texture(NormalTexture, Texcoord);
    vec3 normalCoords = normalize(vec3((NormalTex.r*2)-1, (NormalTex.g*2)-1, (NormalTex.b*2)-1));

	
	vec4 Color = vec4(0,0,0,0);	
	vec3 DiffuseComponent;
	vec3 SpecularComponent;
	float attenuation =0;
	vec3 L;
	vec3 H;
	vec3 N = normalize(normalCoords*mat3(Tangent, BiTangent, Normal));
    vec3 E = normalize(EyePos-Position);
	FragColor = vec4(AmbientColor*DiffTex.rgb,DiffTex.a);
	for(int i=0; i<LightCount;i++){
	
		Light light = lights[i];
	
		//POINT
        if(light.Type == 0){  
			L = normalize(light.Position-Position);
			H = normalize(E + L);	
			float dist = distance(Position,light.Position);
			attenuation = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
		}
		
		//DIRECTIONAL
		if(light.Type == 1){  
			L = -light.Direction;
			H = normalize(E+L);
			// DiffuseComponent += light.Color * DiffuseColor * sat(dot(N,L));
			//SpecularComponent += light.Color * SpecularColor * pow( sat(dot(N,H)), SpecularExp);
			//Color += vec4((DiffuseComponent + AmbientColor)*DiffTex.rgb + SpecularComponent ,DiffTex.a);
			attenuation = 1;
		}
		
		//SPOT
		if(light.Type == 2){
			 L = -normalize(Position-light.Position);
			 H = normalize(E + L);
		
			//angle between light dir verctor and fragment dir to light source
			float cosTheta = dot(-L, normalize(light.Direction));
			float dist = distance(Position,light.Position);
			float spotEffect = clamp((cosTheta - light.SpotRadius.x) / (light.SpotRadius.y - light.SpotRadius.x), 0.0, 1.0);
			attenuation = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist)*spotEffect;
			
			
			//DiffuseComponent += light.Color * DiffuseColor * sat(dot(N, L)) * spotEffect;
			//SpecularComponent += light.Color * SpecularColor * pow(sat(dot(N, H)), SpecularExp) *spotEffect;
		}
		
	DiffuseComponent = light.Color * DiffuseColor * sat(dot(N, L));
	SpecularComponent = light.Color * SpecularColor * pow(sat(dot(N, H)), SpecularExp);
	FragColor +=  vec4(attenuation*(DiffuseComponent *DiffTex.rgb) + attenuation*SpecularComponent ,0) ;
	}
	
		
}
