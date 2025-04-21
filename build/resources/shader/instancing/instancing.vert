#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aInstanceMatrix;

out vec4 oColor; // Color passed to fragment shader
out vec3 oNormal; // Normal passed to fragment shader

uniform mat4 projection;
uniform mat4 view;
uniform float time;
uniform float rotationSpeed;
uniform vec3 offset;


mat4 rotationMatrix(float angle, vec3 axis) {
    float c = cos(angle);
    float s = sin(angle);
    float oneMinusC = 1.0 - c;

    vec3 n = normalize(axis);
    float x = n.x, y = n.y, z = n.z;

    return mat4(
        c + x * x * oneMinusC,      x * y * oneMinusC - z * s,  x * z * oneMinusC + y * s,  0.0,
        y * x * oneMinusC + z * s,  c + y * y * oneMinusC,      y * z * oneMinusC - x * s,  0.0,
        z * x * oneMinusC - y * s,  z * y * oneMinusC + x * s,  c + z * z * oneMinusC,      0.0,
        0.0,                        0.0,                        0.0,                        1.0
    );
}

void main()
{
    int packedColor = int(aInstanceMatrix[3].w);
    int red = (packedColor >> 16) & 0xFF;
    int green = (packedColor >> 8) & 0xFF;
    int blue = packedColor & 0xFF;
    vec3 unpackedColor = vec3(float(red) / 255.0, float(green) / 255.0, float(blue) / 255.0);

    mat4 instanceMatrix = aInstanceMatrix;
    instanceMatrix[3].w = 1.0;

    vec3 rotationAxis = vec3(0, 1, 0);
    float angle = time * rotationSpeed;
    mat4 globalRotation = rotationMatrix(angle, rotationAxis);
    mat4 finalModel = globalRotation * instanceMatrix;
 
    vec4 worldPos = finalModel * vec4(aPos, 1.0f);
    worldPos.xyz += offset;
    oNormal = mat3(transpose(inverse(finalModel))) * aNormal;
    oColor = vec4(unpackedColor, 1);
    gl_Position = projection * view * worldPos;     
}