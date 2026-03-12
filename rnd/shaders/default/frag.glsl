#version 420 core

in vec3 FragPos;
out vec4 FragColor;

uniform float time;

void main() {
    FragColor = vec4(0.64 + sin(time) * 0.36, 0.16, 0.12, 1.0);
    return;
}