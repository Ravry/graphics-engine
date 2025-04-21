#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 PositionColor;
layout (location = 2) out vec4 NormalColor;
layout (location = 3) out vec4 MaterialColor;

in vec2 oTexCoord;
in vec4 oPosition;
in vec3 oNormal;

uniform sampler2D tex;
uniform sampler2D normalMap;

uniform samplerCube cubemap;
uniform vec2 texTiling;
uniform vec3 texTint;
uniform vec3 cameraPos;
uniform mat4 view;

uniform float metallic;

uniform vec3 lightDirection = normalize(vec3(0, 1, 0));
uniform vec3 lightColor = vec3(1, 1, 1);

vec3 reflection() {
    vec3 worldPosition = vec3(inverse(view) * vec4(oPosition.xyz, 1.0));
    vec3 I = normalize(worldPosition - cameraPos);
    vec3 R = reflect(I, vec3(0, 1, 0)); 
    return texture(cubemap, R).rgb;
}

void main() {
    vec3 normal = normalize(oNormal);
    vec3 normal_m = texture(normalMap, oTexCoord).xyz;

    vec3 albedo_color = texture(tex, fract(oTexCoord * texTiling)).rgb * texTint;
    vec3 reflection_color = reflection();
    vec3 final_color = mix(albedo_color, reflection_color, .5 * metallic);

    float diffuse = max(dot(normal, lightDirection), 0.);
    final_color  *= diffuse * lightColor;

    vec3 worldPos = vec3(inverse(view) * vec4(oPosition.xyz, 1.0));

   // Large grid (10 units)
    float largeGridSize = 20.0;
    vec2 largeGrid = abs(fract(worldPos.xz / largeGridSize - 0.5) - 0.5) / fwidth(worldPos.xz / largeGridSize);
    float largeLine = min(largeGrid.x, largeGrid.y);
    float largeGridLine = 1.0 - min(largeLine, 1.0);

    // Small grid (1 unit)
    float smallGridSize = 2.5;
    vec2 smallGrid = abs(fract(worldPos.xz / smallGridSize - 0.5) - 0.5) / fwidth(worldPos.xz / smallGridSize);
    float smallLine = min(smallGrid.x, smallGrid.y);
    float smallGridLine = 1.0 - min(smallLine, 1.0);
        
    vec3 backgroundColor = vec3(0.0);
    vec3 smallGridColor = vec3(0.88);   // dimmer
    vec3 largeGridColor = vec3(1.0);   // bright
    
    if (smallGridLine < .5)
        discard;

    vec3 finalColor = mix(backgroundColor, smallGridColor, smallGridLine);
    finalColor = mix(finalColor, largeGridColor, largeGridLine);

    FragColor = vec4(finalColor, 1.0);
    PositionColor = oPosition;
    NormalColor = vec4(normal, 1);
    MaterialColor = vec4(metallic, 0, 0, 1);
}