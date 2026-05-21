#version 430 core

out vec2 TexCoords;

uniform float scale;

void main() {
    // vec2 uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);

    gl_Position = vec4(4 * ivec2(gl_VertexID & 1, (gl_VertexID & 2)>>1) - 1, 0.0, 1.0);
    TexCoords = (gl_Position.xy + 1) / 2;
    if (scale < 1 && scale > 0) TexCoords /= scale;
    return;
}