#version 330 core

in vec4 fragColor;
in vec2 fragTexPos;
in float fragTexIndex;

out vec4 color;

uniform vec4 u_color;
uniform sampler2D u_textures[16];

void main() {

    int index = int(fragTexIndex);
    color = texture(u_textures[index], fragTexPos) * fragColor;
}
