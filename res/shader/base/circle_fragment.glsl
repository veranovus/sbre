#version 330 core

in vec4 fragColor;
in vec2 fragTexPos;
in float fragTexIndex;

out vec4 color;

uniform float u_thickness;
uniform sampler2D u_textures[16];

void main() {

    int index = int(fragTexIndex);
    float fade = 0.005;

    vec2 uv = fragTexPos;
    uv = uv * 2 - 1;
    float distance = 1 - length(uv);
    vec3 col = vec3(smoothstep(0.0, fade, distance));
    col *= vec3(smoothstep(u_thickness + fade, u_thickness, distance));

    if (distance > 0) {
        if (distance > u_thickness)
            color = texture(u_textures[index], fragTexPos) * fragColor * vec4(col, 1);
        else 
            discard;
    }
    else
        discard;
}
