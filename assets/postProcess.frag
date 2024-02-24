#version 400 core


in vec2 texCoords;

out vec4 FragColor;
uniform sampler2D screenTexture;
uniform sampler2D uiTexture;

void main()
{
	float curvature = 5;
	float vignetteWidth = 30;
	vec2 screenSize = vec2(800,600);
	vec2 uvCoord = texCoords * 2.0 - vec2(1.0);
	vec2 offset = uvCoord.yx / curvature;
	uvCoord = uvCoord + uvCoord * offset * offset;
    uvCoord = uvCoord * 0.5 + 0.5;
	
	vec4 sceneColor = texture(screenTexture, uvCoord);
	vec4 uiColor = texture(uiTexture, uvCoord);
	vec4 col = mix(sceneColor, uiColor, uiColor.a);
	if (uvCoord.x <= 0.0 || uvCoord.x >= 1.0 || uvCoord.y <= 0.0 || uvCoord.y >= 1.0){
        col = vec4(0.0);
	}
	
	uvCoord = uvCoord * 2.0 - vec2(1.0);
    vec2 vignette = vignetteWidth / screenSize;
    vignette.x = smoothstep(0.0, vignette.x, 1.0 - abs(uvCoord.x));
	vignette.y = smoothstep(0.0, vignette.y, 1.0 - abs(uvCoord.y));
    vignette = clamp(vignette, 0.0, 1.0);

	col.g *= (sin(texCoords.y * screenSize.y * 2.0) + 1.0) * 0.15 + 1.0;
    col.rb *= (cos(texCoords.y * screenSize.y * 2.0) + 1.0) * 0.135 + 1.0;
	
	FragColor = col* vignette.x * vignette.y;
}