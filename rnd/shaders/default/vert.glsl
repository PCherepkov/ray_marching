#version 430 core

out vec3 FragPos;

uniform float scale;

void main() {
    // vec2 uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);

    //if (gl_VertexID == 0)
    //    gl_Position = vec4(0.0, -1.0, 0.0, 1.0);
    //else if (gl_VertexID == 1)
    //    gl_Position = vec4(2.0, 1.0, 0.0, 1.0);
    //else if (gl_VertexID == 2)
    //    gl_Position = vec4(0.0, 1.0, 0.0, 1.0);
    // gl_Position = vec4(uv * 2.0f - 1.0f, 0.0f, 1.0f);
    gl_Position = vec4(4 * ivec2(gl_VertexID & 1, (gl_VertexID & 2)>>1) - 1, 0.0, 1.0);
    if (scale >= 1.0)
        FragPos = vec3(gl_Position.xy * scale + scale - 1, gl_Position.z);
    else if (scale > 0) {
        // gl_Position.xy /= scale;
        FragPos = vec3(gl_Position.xy * scale / scale + scale - scale + 1 - 1, gl_Position.z);
        // FragPos /= scale;
    }
    return;
}