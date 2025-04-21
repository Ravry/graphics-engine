#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D albedoTex;
uniform sampler2D depthTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D materialTex;

uniform mat4 projection;
uniform int gBufferTex;

float linearizeDepth(float depth, float near, float far) {
    float ndc = depth * 2.0 - 1.0; // Convert depth from [0,1] to [-1,1] (NDC space)
    return (2.0 * near * far) / (far + near - ndc * (far - near));
}

void main() {
    vec3 normal = normalize(texture(normalTex, TexCoords).rgb);
    vec3 albedo = texture(albedoTex, TexCoords).rgb;
    vec3 position = texture(positionTex, TexCoords).rgb;
    float metallic = texture(materialTex, TexCoords).r;
    float depth = texture(depthTex, TexCoords).r;
    float roughness = 0;
   
    if (gBufferTex > 0) {
        if (gBufferTex == 1) FragColor = vec4(normal, 1.);
        if (gBufferTex == 2) FragColor = vec4(position, 1.);
        if (gBufferTex == 3) FragColor = vec4(texture(materialTex, TexCoords).rgb, 1.0);
        if (gBufferTex == 4) {
            float _depth = linearizeDepth(depth, .1, 1000.) / 1000.;
            FragColor = vec4(vec3(_depth), 1.0);
        }
        return;
    }

    FragColor = vec4(albedo, 1);
}