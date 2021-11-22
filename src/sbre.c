
/* GLEW and GLFW include point */

#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>


/* Window */

uint32_t _SBRE_WINDOW_HEIGHT;
uint32_t _SBRE_WINDOW_HEIGHT;



// TODO : Remove u_color from these shaders
/* Default Shaders */

const char* _SBRE_vertex_shader_source = "#version 330 core\n"
                                         "layout (location = 0) in vec4 v_pos;\n"
                                         "layout (location = 1) in vec4 v_color;\n"
                                         "layout (location = 2) in vec2 v_texPos;\n"
                                         "layout (location = 3) in float v_texIndex;\n"
                                         "\n"
                                         "out vec4 fragColor;\n"
                                         "out vec2 fragTexPos;\n"
                                         "out float fragTexIndex;\n"
                                         "\n"
                                         "uniform mat4 u_mvp;\n"
                                         "\n"
                                         "void main() {\n"
                                         "\n"
                                         "    fragColor = v_color;\n"
                                         "    fragTexPos = v_texPos;\n"
                                         "    fragTexIndex = v_texIndex;\n"
                                         "\n"
                                         "    gl_Position = u_mvp * v_pos;\n"
                                         "}\n";



const char* _SBRE_fragment_shader_source = "#version 330 core\n"
                                           "\n"
                                           "in vec4 fragColor;\n"
                                           "in vec2 fragTexPos;\n"
                                           "in float fragTexIndex;\n"
                                           "\n"
                                           "out vec4 color;\n"
                                           "\n"
                                           "uniform sampler2D u_textures[16];\n"
                                           "uniform float u_istext;\n"
                                           "\n"
                                           "void main() {\n"
                                           "\n"
                                           "    int index = int(fragTexIndex);\n"
                                           "    int istext = int(u_istext);\n"
                                           "    \n"
                                           "    vec4 tex_color = texture(u_textures[index], fragTexPos);\n"
                                           "    \n"
                                           "    if (istext == 0)\n"
                                           "        color = tex_color * fragColor;\n"
                                           "    else {\n"
                                           "        if (tex_color.r > 0) {\n"
                                           "            color = fragColor * tex_color.r;\n"
                                           "        }\n"
                                           "    }\n"
                                           "}\n";



const char* _SBRE_circle_fragment_shader_source = "#version 330 core\n"
                                                  "\n"
                                                  "in vec4 fragColor;\n"
                                                  "in vec2 fragTexPos;\n"
                                                  "in float fragTexIndex;\n"
                                                  "\n"
                                                  "out vec4 color;\n"
                                                  "\n"
                                                  "uniform float u_thickness;\n"
                                                  "uniform sampler2D u_textures[16];\n"
                                                  "\n"
                                                  "void main() {\n"
                                                  "\n"
                                                  "    int index = int(fragTexIndex);\n"
                                                  "    float fade = 0.005;\n"
                                                  "\n"
                                                  "    vec2 uv = fragTexPos;\n"
                                                  "    uv = uv * 2 - 1;\n"
                                                  "    float distance = 1 - length(uv);\n"
                                                  "    vec4 col = vec4(smoothstep(0.0, fade, distance));\n"
                                                  "    col *= vec4(smoothstep(u_thickness + fade, u_thickness, distance));\n"
                                                  "\n"
                                                  "    if (distance > 0) {\n"
                                                  "        if (distance <= u_thickness + fade)"
                                                  "            color = texture(u_textures[index], fragTexPos) * fragColor * col;\n"
                                                  "        else\n"
                                                  "            discard;\n"
                                                  "    }"
                                                  "    else\n"
                                                  "        discard;\n"
                                                  "}\n";
