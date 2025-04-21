#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 2) out vec4 NormalColor;
in vec4 oColor;
in vec3 oNormal;

uniform vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));
uniform vec3 lightColor = vec3(1.0);
uniform mat4 view;
uniform float shininess = 32.0;

void main() {
    vec3 normal = normalize(oNormal);
    vec3 viewDir = normalize(vec3(view[0][2], view[1][2], view[2][2]));
    vec3 ambient = 0.2 * lightColor;
    float diff = max(dot(normal, -lightDir), 0.0);  // Correct - using negative lightDir for diffuse
    vec3 diffuse = diff * lightColor;
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = 0.5 * spec * lightColor;
    vec3 finalColor = (ambient + diffuse + specular) * oColor.rgb;
    FragColor = vec4(finalColor, 1.0);
    NormalColor = vec4(normal, 1);
}