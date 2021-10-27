#include "common.h"



/* Structs */

typedef struct MVP {

	Mat4 proj;
	Mat4 view;
	Mat4 mvp;

} MVP;



typedef struct Vertex {

	Vec2 pos;
	Color color;
	Vec2 tex_coord;
	float index;

} Vertex;



typedef struct Renderer {
	
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;

	Vertex* quad_buffer;
	Vertex* quad_buffer_ptr;
	
} Renderer;



typedef struct BatchRenderer {
	
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;
	
	uint32_t index_count;

	Vertex* quad_buffer;
	Vertex* quad_buffer_ptr;
	
	uint32_t* texture_slots;
	uint32_t texture_index;

	// Renderer Stats
	uint32_t rs_ender_count;
	uint32_t rs_quad_count;
	
} BatchRenderer;



/* Renderers */

static Renderer _SBRE_renderer;



/* Renderer Properties */

static const uint32_t MAX_QUAD = 10000;
static const uint32_t MAX_VERTEX = MAX_QUAD * 4;
static const uint32_t MAX_INDEX = MAX_QUAD * 6;


static Texture* _SBRE_default_texture;
static uint32_t _SBRE_default_shader;


int32_t MAX_TEXTURE_SLOTS;


static MVP _SBRE_mvp;
uint32_t _SBRE_active_shader = 0;



/* Initialization */

void _SBRE_init_render_systems(void) {

	/* Compile and Use the Default Shader */

	_SBRE_default_shader = _SBRE_create_shader_from_string(_SBRE_vertex_shader_source, _SBRE_fragment_shader_source);

	if (!(_SBRE_default_shader)) {

		printf("[SBRE Error][Default shader coult not compile.]\n");
		return;
	}

	SBRE_use_shader(_SBRE_default_shader);	


	/* Set MAX_TEXTURE_SLOTS avalible in this machine */
	
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_SLOTS);


	/* Init MVP */
	
	/* Default view matrix is initialized here, this is not the same for projection matrix */

	_SBRE_mvp = (MVP) {

		.proj = (Mat4) { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		.view = (Mat4) { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} },
		.mvp  = (Mat4) { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
	};


	/* Generate and Set Default Texture */

	_SBRE_default_texture = (Texture*) calloc(1, sizeof(Texture));

	glGenTextures(1, &_SBRE_default_texture->texture_id);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	uint32_t tex_color = 0xffffffff;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex_color);
	glBindTexture(GL_TEXTURE_2D, 0);
	

	/* Initialize Renderers */
	
	_SBRE_renderer = (Renderer) {
		
		.vao = 0,
		.vbo = 0,
		.ebo = 0,
		
		.quad_buffer = NULL,
		.quad_buffer_ptr = NULL,
	};

	/*_SBRE_renderer = {
		
		.vao = 0;
		.vbo = 0;
		.ebo = 0;
		
		.index_count = 0;
		
		.quad_buffer = NULL;
		.quad_buffer_ptr = NULL;
		
		.texture_slots;
		.texture_index = 0;
		
		.rs_ender_count = 0;
		.rs_quad_count = 0;
	}*/

	_SBRE_renderer.quad_buffer = (Vertex*) calloc(4, sizeof(Vertex));
	_SBRE_renderer.quad_buffer_ptr = _SBRE_renderer.quad_buffer;

	glGenVertexArrays(1, &_SBRE_renderer.vao);
	glGenBuffers(1, &_SBRE_renderer.vbo);


	/* Initialize Element Buffer Object it will only be used once. */

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 1
	};

	glGenBuffers(1, &_SBRE_renderer.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);
}



void _SBRE_terminate_render_systems(void) {
	
	glDeleteVertexArrays(1, &_SBRE_renderer.vao);
	glDeleteBuffers(1, &_SBRE_renderer.vbo);
	glDeleteBuffers(1, &_SBRE_renderer.ebo);

	glDeleteTextures(1, &_SBRE_default_texture->texture_id);
	free(_SBRE_default_texture);
	_SBRE_default_texture = NULL;

	free(_SBRE_renderer.quad_buffer);
	_SBRE_renderer.quad_buffer = NULL;
}



/* MVP Matrix */

static Mat4 _SBRE_calculate_mvp() {

	glm_mat4_mul(&_SBRE_mvp.proj.r1, &_SBRE_mvp.view.r1, &_SBRE_mvp.mvp.r1);

	return _SBRE_mvp.mvp;
} 



void _SBRE_set_projection_marix(Mat4 proj) {

	memcpy(_SBRE_mvp.proj.r1, proj.r1, sizeof(proj));
}



void _SBRE_set_view_matrix(Mat4 view) {

	memcpy(_SBRE_mvp.view.r1, view.r1, sizeof(view));
}



/* Renderer */

static void _SBRE_set_vertex(Vec2 pos, float width, float height, Color color) {

	float index = 0.0f;

	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x, pos.y };
	_SBRE_renderer.quad_buffer_ptr->color = (Color){ color.r, color.g, color.b, color.a };
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ 0.0f, 1.0f };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;
	
	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y };
	_SBRE_renderer.quad_buffer_ptr->color = (Color){ color.r, color.g, color.b, color.a };
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ 1.0f, 1.0f };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x, pos.y + height};
	_SBRE_renderer.quad_buffer_ptr->color = (Color){ color.r, color.g, color.b, color.a };
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ 0.0f, 0.0f };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y + height };
	_SBRE_renderer.quad_buffer_ptr->color = (Color){ color.r, color.g, color.b, color.a };
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ 1.0f, 0.0f };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr = _SBRE_renderer.quad_buffer;
}



void SBRE_draw_quad(Vec2 pos, float width, float height, Color color) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_shader)
		SBRE_use_shader(_SBRE_default_shader);


	/* Send default texture to the shader */

	int sampler[16];
	for (int i = 0; i < 16; ++i)
		sampler[i] = i;
	int32_t location = glGetUniformLocation(_SBRE_active_shader, "u_textures");
	glUniform1iv(location, 16, sampler);


	/* Send the default mvp */

	Mat4 mvp = _SBRE_calculate_mvp();
	SBRE_set_uniform_mat4f(_SBRE_active_shader, "u_mvp", mvp);


	/* Set Vertices */

	_SBRE_set_vertex(pos, width, height, color);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_renderer.vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), _SBRE_renderer.quad_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, index));
    glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



void SBRE_draw_texture(Vec2 pos, Texture* texture) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_shader)
		SBRE_use_shader(_SBRE_default_shader);


	/* Send default texture to the shader */

	int sampler[16];
	for (int i = 0; i < 16; ++i)
		sampler[i] = i;
	int32_t location = glGetUniformLocation(_SBRE_active_shader, "u_textures");
	glUniform1iv(location, 16, sampler);


	/* Send the default mvp */

	Mat4 mvp = _SBRE_calculate_mvp();
	SBRE_set_uniform_mat4f(_SBRE_active_shader, "u_mvp", mvp);


	/* Set Vertices */

	_SBRE_set_vertex(pos, texture->width, texture->height, texture->color);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_renderer.vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), _SBRE_renderer.quad_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, index));
    glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
