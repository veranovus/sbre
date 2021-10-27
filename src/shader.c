#include "common.h"



/* Uniforms */

static int _SBRE_check_uniform_location(uint32_t shader, const char* name) {
	
	int location = glGetUniformLocation(shader, name);
	if (location == -1) {
		printf("[OpenGL Warning][\"%s\" is not a valid uniform location]\n", name);
	    return -1;
	}
	
	return location;
}



bool SBRE_set_uniform_1f(uint32_t shader, const char* name, float value) {
	
	int location = _SBRE_check_uniform_location(shader, name);
	if (location == -1) return false;
	
	glUniform1f(location, value);
	return true;
}



bool SBRE_set_uniform_2f(uint32_t shader, const char* name, Vec2 floats) {

	int location = _SBRE_check_uniform_location(shader, name);
	if (location == -1) return false;

    glUniform2f(location, floats.x, floats.y);
    return true;
}



bool SBRE_set_uniform_3f(uint32_t shader, const char* name, Vec3 floats) {

	int location = _SBRE_check_uniform_location(shader, name);
	if (location == -1) return false;

    glUniform3f(location, floats.x, floats.y, floats.z);
    return true;
}



bool SBRE_set_uniform_4f(uint32_t shader, const char* name, Vec4 floats) {

	int location = _SBRE_check_uniform_location(shader, name);
	if (location == -1) return false;

    glUniform4f(location, floats.x, floats.y, floats.z, floats.w);
    return true;
}



bool SBRE_set_uniform_mat4f(uint32_t shader, const char* name, Mat4 matrix) {

	int location = _SBRE_check_uniform_location(shader, name);
	if (location == -1) return false;

    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.r1);
    return true;
}



/* Shaders */

void SBRE_use_shader(uint32_t shader) {
	
	_SBRE_active_shader = shader;
	glUseProgram(shader);
}



uint32_t _SBRE_compile_shader(uint32_t type, const char* shader_source) {

	if (shader_source == NULL)
		return 0;

    uint32_t shader = glCreateShader(type);
    const char* src = shader_source;
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
		
		char* shader_type = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
		printf("[OpenGL Error][Shader failed to fompile : %s]\n", shader_type);
		printf("[Error Output : %s]\n", message);
        
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}



const char* _SBRE_parse_shader(const char* shader_source) {
	
	size_t read = 0;
	size_t read_at_once = 256;
	size_t total_size = read_at_once;
	
	char* buffer = (char*) calloc(total_size, sizeof(char));
	char* current_buffer = buffer;
	char* line = (char*) calloc(256, sizeof(char)); 
	
	FILE* fp = NULL;
	fp = fopen(shader_source, "r");

	if (fp == NULL) {

		printf("[SBRE Error][File \"%s\" could not be opened.]\n", shader_source);
		return NULL;
	}
	
	while((read = fread(line, sizeof(char), 256, fp)) != 0) {
		
		memcpy(current_buffer, line, read * sizeof(char));
		
		if (read == read_at_once) 
			total_size += read;
		else
			total_size -= read_at_once - read;

		buffer = realloc(buffer, total_size);
		current_buffer = buffer + (total_size - read);
	}
	
	buffer = realloc(buffer, (total_size + 1) * sizeof(char));
	buffer[total_size] = '\0';
	
	fclose(fp);
	
	free(line);
	
	return buffer;
}



uint32_t SBRE_create_shader(const char* vertex_source, const char* fragment_source) {

    uint32_t program = glCreateProgram();
    uint32_t vs = _SBRE_compile_shader(GL_VERTEX_SHADER, _SBRE_parse_shader(vertex_source));
    uint32_t fs = _SBRE_compile_shader(GL_FRAGMENT_SHADER, _SBRE_parse_shader(fragment_source));

	if (!(vs) || !(fs)) 
		return 0;

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {

        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("[OpenGL Warning][Shader program failed to link : %s]\n", infoLog);
        return 0;
    }

    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



uint32_t _SBRE_create_shader_from_string(const char* vertex_source, const char* fragment_source) {

	uint32_t program = glCreateProgram();
    uint32_t vs = _SBRE_compile_shader(GL_VERTEX_SHADER, vertex_source);
    uint32_t fs = _SBRE_compile_shader(GL_FRAGMENT_SHADER, fragment_source);

	if (!(vs) || !(fs)) 
		return 0;

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {

        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("[OpenGL Warning][Shader program failed to link : %s]\n", infoLog);
        return 0;
    }

    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


