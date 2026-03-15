#version 420 core

in vec3 FragPos;
out vec4 FragColor;

uniform float time;
uniform ivec2 ScreenResolution;

void main() {
    // ivec2 ScreenResolution = ivec2(1920, 1080);
    // FragColor = vec4(sin(time * 2.0) * 0.5 + 0.5, 0.8, 0.64, 1.0);
    // return;
    float width = max(float(ScreenResolution.x), 1.0);
    float height = max(float(ScreenResolution.y), 1.0);
    float x = FragPos.x, y = FragPos.y * height / width;
    FragColor = vec4(0.64 + sin(time) * 0.64, 0.16, 0.12, 1.0);
    // FragColor = vec4(FragPos, 1.0);

    vec3 ResColor;
    ResColor = vec3(sin(x * 16 + time * 4) - y * 16, 0, 0.3);
    FragColor = vec4(ResColor, 1.0);
    return;
}