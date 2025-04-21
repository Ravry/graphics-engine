#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;

void main() {
    float size = 0.1;
    vec3 center = gl_in[0].gl_Position.xyz;

    vec3 right = vec3(view[0][0], view[1][0], view[2][0]); // column 0
    vec3 up    = vec3(view[0][1], view[1][1], view[2][1]); // column 1

    vec2 offsets[4] = vec2[](
        vec2(-1, -1),
        vec2( 1, -1),
        vec2(-1,  1),
        vec2( 1,  1)
    );

    for (int i = 0; i < 4; ++i) {
        vec3 offset = right * offsets[i].x * size + up * offsets[i].y * size;
        vec4 worldPos = vec4(center + offset, 1.0);
        gl_Position = projection * view * worldPos;
        EmitVertex();
    }

    EndPrimitive();
}
