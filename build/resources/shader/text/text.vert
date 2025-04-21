#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uv;

out vec2 oTexCoord;

uniform mat4 projection;
uniform mat4 model;

void main() {
    oTexCoord = uv;
    gl_Position = projection * model * vec4(vertex, 1);
}