#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

out vec2 oTexCoords;

void main() {
    oTexCoords = TexCoords;
    gl_Position = projection * model * vec4(vertex, 0, 1);
}