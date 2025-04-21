#version 330 core

out vec4 FragColor;
in vec2 oTexCoord;

uniform sampler2D fontTexture;
uniform vec3 fontColor;
uniform float time;

vec3 HSVtoRGB(float h, float s, float v) {
    vec3 c = vec3(abs(mod(h * 6.0, 6.0) - 3.0) - 1.0, 2.0 - abs(mod(h * 6.0, 6.0) - 2.0), 2.0 - abs(mod(h * 6.0, 6.0) - 4.0));
    return v * mix(vec3(1.0), clamp(c, 0.0, 1.0), s);
}

void main() {
    float alpha = texture(fontTexture, oTexCoord).r;
    float hue = mod(((time + oTexCoord.x) * .2) + oTexCoord.y * .4, 1.0);
    vec3 rainbowColor = HSVtoRGB(hue, 1.0, 1.0);
    FragColor = vec4(rainbowColor * fontColor, alpha);
}