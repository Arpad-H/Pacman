#version 400
//inherited from phong
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

//bloom uniforms
uniform sampler2D MixTex;
uniform vec2 MainTex_TexelSize;

//set in glowshader.h
uniform float Threshold;
uniform float SoftThreshold;
uniform float DownDelta;
uniform float UpDelta;
uniform sampler2D OriginalTex;
uniform float Intensity;

//input form the vertex shader
in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;


//gl_FragColor is enough as opengls defualt write  to location
out vec4 FragColor;

/*vec3 Sample(vec2 uv) {
    return texture(_MainTex, uv).rgb;
}

vec3 SampleBox(vec2 uv, float delta) {
    vec4 o = _MainTex_TexelSize.xyxy * vec4(-delta, delta, -delta, delta);
    vec3 s = Sample(uv + o.xy) + Sample(uv + o.zy) + Sample(uv + o.xw) + Sample(uv + o.zw);

    return s * 0.25;
}

vec4 Prefilter(vec4 col) {
    float brightness = max(col.r, max(col.g, col.b));
    float knee = Threshold * SoftThreshold;
    float soft = brightness - Threshold + knee;
    soft = clamp(soft, 0.0, 2.0 * knee);
    soft = soft * soft / (4.0 * knee * 0.00001);
    float contribution = max(soft, brightness - Threshold);
    contribution /= max(contribution, 0.00001);

    return col * contribution;
}

vec4 PrefilterPower(vec4 col, float power) {
    return vec4(pow(col.rgb, vec3(power)), col.a);
}

vec4 ApplyBloom(vec2 uv) {
    vec4 col = texture(OriginalTex, uv);
    col.rgb += Intensity * PrefilterPower(SampleBox(uv, UpDelta), 1.0 / 2.2);

    return col;
}
*/
void main() {
   // gl_FragColor = Prefilter(PrefilterPower(texture2D(_MainTex, Texcoord), 2.2));
   FragColor = vec4(1,1,1,0);
}
/*
void downsample() {
    gl_FragColor = vec4(SampleBox(Texcoord, DownDelta), 1.0);
}

void upsample() {
    gl_FragColor = vec4(SampleBox(Texcoord, UpDelta), 1.0);
}

void additiveBlendBloom() {
    gl_FragColor = ApplyBloom(Texcoord);
}*/
