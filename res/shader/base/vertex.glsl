#version 330 core

layout (location = 0) in vec4 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec2 v_texPos;
layout (location = 3) in float v_texIndex;

out vec4 fragColor;
out vec2 fragTexPos;
out float fragTexIndex;

uniform mat4 u_mvp;

void main() {

    fragColor = v_color;
    fragTexPos = v_texPos;
    fragTexIndex = v_texIndex;

    gl_Position = u_mvp * v_pos;
}
