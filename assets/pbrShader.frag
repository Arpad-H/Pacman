#version 400 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;
uniform vec3 CameraPos; 
uniform vec3 LightPos; 
//uniform samplerCube EnviromentCube;
uniform sampler2D BaseColorTexture;  
uniform sampler2D RoughnessTexture;  
uniform sampler2D AmbientOcclusionTexture;
uniform sampler2D NormalTexture;
uniform sampler2D MetallicTexture;
uniform float tilingFactor;
uniform float enviromentEffect;

const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(NormalTexture, Texcoord* tilingFactor).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(Position);
    vec3 Q2  = dFdy(Position);
    vec2 st1 = dFdx(Texcoord);
    vec2 st2 = dFdy(Texcoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
void main()
{   
	//sample texures  
	
	
	
	vec3 albedo = texture(BaseColorTexture, Texcoord* tilingFactor).rgb;
	vec3 normal = getNormalFromMap();
	float roughness = texture(RoughnessTexture, Texcoord* tilingFactor).r;
	float ambientOcclusion = texture(AmbientOcclusionTexture, Texcoord* tilingFactor).r;
	float metallic  = texture(MetallicTexture, Texcoord* tilingFactor).r;
	vec3 I = normalize(Position - CameraPos);
	vec3 R = reflect(I, normalize(Normal));
	//vec3 reflectedColor = texture(EnviromentCube, R).rgb;
	
	vec3 N = normalize(Normal);
    vec3 V = normalize(CameraPos - Position);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    
    vec3 Lo = vec3(0.0);
   
      
    vec3 L = normalize(LightPos - Position);
    vec3 H = normalize(V + L);
    float distance    = length(LightPos - Position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = vec3(1,1,1) * attenuation;        
        
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
            
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
      
 
    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
    //color = mix(color,reflectedColor,enviromentEffect);
    FragColor = vec4(color, 1.0);
}