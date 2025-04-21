#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPos;

out vec2 oTexCoord;
out vec4 oPosition;
out vec3 oNormal;

void main() {
    oTexCoord = uv;
    oNormal = normalize(transpose(inverse(mat3((model)))) * normal);
    vec4 worldPos = model * vec4(vertex, 1.0);
    worldPos.xz += cameraPos.xz;
    oPosition = view * worldPos;
    gl_Position = projection * view * worldPos;
}