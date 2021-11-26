#include "common.h"

#include "util/util.h"



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



typedef struct Vertex_Ext {

	Vec2 pos;
	Color color;
	Vec2 tex_coord;
	float index;
	float draw_type;
	float thickness;

} Vertex_Ext;



typedef struct Renderer {
	
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;

	Vertex* quad_buffer;
	Vertex* quad_buffer_ptr;


	uint32_t text_vao;
	uint32_t text_vbo;
	uint32_t text_ebo;

	Vertex* text_buffer;
	Vertex* text_buffer_ptr;
	uint32_t text_char_count;
	
} Renderer;



typedef struct BatchRenderer {
	
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;
	
	uint32_t index_count;

	Vertex_Ext* quad_buffer;
	Vertex_Ext* quad_buffer_ptr;
	
	uint32_t* texture_slots;
	uint32_t texture_index;

	// Renderer Stats
	uint32_t rs_render_count;
	uint32_t rs_quad_count;
	
} BatchRenderer;



/* Draw Types */

#define DRAW_TYPE_QUAD		0
#define DRAW_TYPE_TEXT		1
#define DRAW_TYPE_CIRCLE	2



/* Renderers */

static Renderer _SBRE_renderer;
static BatchRenderer _SBRE_batch_renderer;


/* Renderer Properties */

static const uint32_t MAX_QUAD = 10000;
static const uint32_t MAX_VERTEX = MAX_QUAD * 4;
static const uint32_t MAX_INDEX = MAX_QUAD * 6;


static Texture* _SBRE_default_texture;


static uint32_t _SBRE_default_shader;
static uint32_t _SBRE_default_circle_shader;

static uint32_t _SBRE_default_batch_shader;


int32_t MAX_TEXTURE_SLOTS;


static MVP _SBRE_mvp;
uint32_t _SBRE_active_shader = 0;



/* Initialization */

void _SBRE_init_render_systems(void) {

	/* Enable OpenGL Attributes */

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	/* Compile and Use the Default Shaders */

	_SBRE_default_shader = _SBRE_create_shader_from_string(_SBRE_vertex_shader_source, _SBRE_fragment_shader_source);

	if (!(_SBRE_default_shader)) {

		printf("[SBRE Error][Default shader coult not compile.]\n");
		return;
	}


	_SBRE_default_circle_shader = _SBRE_create_shader_from_string(_SBRE_vertex_shader_source, _SBRE_circle_fragment_shader_source);

	if (!(_SBRE_default_circle_shader)) {

		printf("[SBRE Error][Default circle shader could not compile.]\n");
		return;
	}


	_SBRE_default_batch_shader = _SBRE_create_shader_from_string(_SBRE_batch_vertex_shader_source, _SBRE_batch_fragment_shader_source);

	if (!(_SBRE_default_batch_shader)) {

		printf("[SBRE Error][Default batch shader could not compile.]\n");
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
	

	/* Create Renderer */
	
	_SBRE_renderer = (Renderer) {
		
		.vao = 0,
		.vbo = 0,
		.ebo = 0,
		
		.quad_buffer = NULL,
		.quad_buffer_ptr = NULL,


		.text_vao = 0,
		.text_vbo = 0,
		.text_ebo = 0,

		.text_buffer = NULL,
		.text_buffer_ptr = NULL,
		.text_char_count = 0,
	};

	_SBRE_renderer.quad_buffer = (Vertex*) calloc(4, sizeof(Vertex));
	_SBRE_renderer.quad_buffer_ptr = _SBRE_renderer.quad_buffer;

	_SBRE_renderer.text_buffer = (Vertex*) calloc(MAX_VERTEX, sizeof(Vertex));
	_SBRE_renderer.text_buffer_ptr = _SBRE_renderer.text_buffer;


	_SBRE_initalize_renderer();	


	/* Create Batch Renderer */

	_SBRE_batch_renderer = (BatchRenderer) {
		
		.vao = 0,
		.vbo = 0,
		.ebo = 0,
		
		.index_count = 0,
		
		.quad_buffer = NULL,
		.quad_buffer_ptr = NULL,
		
		.texture_slots = NULL,
		.texture_index = 1,
		
		.rs_render_count = 0,
		.rs_quad_count = 0,
	};

	_SBRE_batch_renderer.quad_buffer = (Vertex_Ext*) calloc(MAX_VERTEX, sizeof(Vertex_Ext));
	_SBRE_batch_renderer.quad_buffer_ptr = _SBRE_batch_renderer.quad_buffer;


	_SBRE_initialize_batch_renderer();
}



void _SBRE_terminate_render_systems(void) {

	/* Free the default texture */

	glDeleteTextures(1, &_SBRE_default_texture->texture_id);
	free(_SBRE_default_texture);
	_SBRE_default_texture = NULL;


	/* Cleanup the renderer */

	glDeleteVertexArrays(1, &_SBRE_renderer.vao);
	glDeleteBuffers(1, &_SBRE_renderer.vbo);
	glDeleteBuffers(1, &_SBRE_renderer.ebo);

	glDeleteVertexArrays(1, &_SBRE_renderer.text_vao);
	glDeleteBuffers(1, &_SBRE_renderer.text_vbo);
	glDeleteBuffers(1, &_SBRE_renderer.text_ebo);

	free(_SBRE_renderer.quad_buffer);
	_SBRE_renderer.quad_buffer = NULL;

	free(_SBRE_renderer.text_buffer);
	_SBRE_renderer.text_buffer = NULL;


	/* Cleanup the Batch Renderer */

	glDeleteVertexArrays(1, &_SBRE_batch_renderer.vao);
	glDeleteBuffers(1, &_SBRE_batch_renderer.vbo);
	glDeleteBuffers(1, &_SBRE_batch_renderer.ebo);

	free(_SBRE_batch_renderer.quad_buffer);
	_SBRE_batch_renderer.quad_buffer = NULL;

	free(_SBRE_batch_renderer.texture_slots);
	_SBRE_batch_renderer.texture_slots = NULL;
}



void _SBRE_initalize_renderer(void) {

	/* Create and Initialize VAO and VBO */

	glGenVertexArrays(1, &_SBRE_renderer.vao);
	glBindVertexArray(_SBRE_renderer.vao);


	glGenBuffers(1, &_SBRE_renderer.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_renderer.vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, index));
    glEnableVertexAttribArray(3);

	glBindVertexArray(0);


	glGenVertexArrays(1, &_SBRE_renderer.text_vao);
	glBindVertexArray(_SBRE_renderer.text_vao);

	glGenBuffers(1, &_SBRE_renderer.text_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_renderer.text_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, index));
    glEnableVertexAttribArray(3);

	glBindVertexArray(0);


	/* Initialize Element Buffer Object it will only be used once. */

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 1
	};

	glGenBuffers(1, &_SBRE_renderer.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);


	/* Initialize the Element Buffer Object for text */

	unsigned int text_indices[MAX_INDEX];
	unsigned int offset = 0;
	for (uint32_t i = 0; i < MAX_INDEX; i += 6) {

		text_indices[0 + i] = 0 + offset;
        text_indices[1 + i] = 1 + offset;
        text_indices[2 + i] = 2 + offset;
        text_indices[3 + i] = 2 + offset;
        text_indices[4 + i] = 3 + offset;
        text_indices[5 + i] = 1 + offset;

        offset += 4;
	}

	glGenBuffers(1, &_SBRE_renderer.text_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.text_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(text_indices), text_indices, GL_STATIC_DRAW);
}



void _SBRE_initialize_batch_renderer(void) {

	/* Create and Initialize VAO and VBO */

	glGenVertexArrays(1, &_SBRE_batch_renderer.vao);
	glBindVertexArray(_SBRE_batch_renderer.vao);


	glGenBuffers(1, &_SBRE_batch_renderer.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_batch_renderer.vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX * sizeof(Vertex_Ext), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Ext), (const void*) offsetof(Vertex_Ext, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Ext), (const void*) offsetof(Vertex_Ext, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Ext), (const void*) offsetof(Vertex_Ext, tex_coord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_Ext), (const void*) offsetof(Vertex_Ext, index));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_Ext), (const void*) offsetof(Vertex_Ext, draw_type));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_Ext), (const void*) offsetof(Vertex_Ext, thickness));
	glEnableVertexAttribArray(5);


	/* Create and Initialize the EBO */

	unsigned int text_indices[MAX_INDEX];
	unsigned int offset = 0;
	for (uint32_t i = 0; i < MAX_INDEX; i += 6) {

		text_indices[0 + i] = 0 + offset;
        text_indices[1 + i] = 1 + offset;
        text_indices[2 + i] = 2 + offset;
        text_indices[3 + i] = 2 + offset;
        text_indices[4 + i] = 3 + offset;
        text_indices[5 + i] = 1 + offset;

        offset += 4;
	}

	glGenBuffers(1, &_SBRE_batch_renderer.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_batch_renderer.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(text_indices), text_indices, GL_STATIC_DRAW);


	/* Initialize the Texture Slots */

	_SBRE_batch_renderer.texture_slots = (uint32_t*) calloc(MAX_TEXTURE_SLOTS, sizeof(uint32_t));
	_SBRE_batch_renderer.texture_slots[0] = _SBRE_default_texture->texture_id;
	for (int i = 1; i < MAX_TEXTURE_SLOTS; ++i)
		_SBRE_batch_renderer.texture_slots[i] = i;
}



/* MVP Matrix */

static Mat4 _SBRE_calculate_mvp(void) {

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

static void _SBRE_set_vertices(Vec2 pos, float width, float height, Color color, float index, Rectangle text_coord) {

	Color normalized_color = NORMALIZE_RGBA(color.r, color.g, color.b ,color.a);


	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x, pos.y };
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y + text_coord.height };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;
	
	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y };
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y + text_coord.height };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x, pos.y + height};
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y + height };
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;


	_SBRE_renderer.quad_buffer_ptr = _SBRE_renderer.quad_buffer;
}



static void _SBRE_set_vertices_rotated(Vec2 pos, float width, float height, Color color, float index, Rectangle text_coord, float rotation) {

	Color normalized_color = NORMALIZE_RGBA(color.r, color.g, color.b ,color.a);


	float mat2[4];
	mat2[0] =  cos(rotation);
	mat2[1] = -sin(rotation);
	mat2[2] =  sin(rotation);
	mat2[3] =  cos(rotation);

	Vec2 ref_pos = SBRE_VEC2(pos.x + width / 2, pos.y + height / 2);

	Vec2 ref_dir = SBRE_VEC2(0, 1);
	Vec2 rot_direction = SBRE_VEC2(mat2[0] * ref_dir.x + mat2[1] * ref_dir.y,
								   mat2[2] * ref_dir.x + mat2[3] * ref_dir.y);


	_SBRE_renderer.quad_buffer_ptr->pos = util_add_vec2(util_add_vec2(ref_pos, util_multiply_vec2(rot_direction, -height / 2)), util_multiply_vec2(util_vec2_normal(rot_direction), -width / 2));
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y + text_coord.height };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;
	
	_SBRE_renderer.quad_buffer_ptr->pos = util_add_vec2(util_add_vec2(ref_pos, util_multiply_vec2(rot_direction, -height / 2)), util_multiply_vec2(util_vec2_normal(rot_direction),  width / 2));
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y + text_coord.height };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr->pos = util_add_vec2(util_add_vec2(ref_pos, util_multiply_vec2(rot_direction,  height / 2)), util_multiply_vec2(util_vec2_normal(rot_direction), -width / 2));	
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;

	_SBRE_renderer.quad_buffer_ptr->pos = util_add_vec2(util_add_vec2(ref_pos, util_multiply_vec2(rot_direction,  height / 2)), util_multiply_vec2(util_vec2_normal(rot_direction),  width / 2));
	_SBRE_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y };
	_SBRE_renderer.quad_buffer_ptr->index = index;
	_SBRE_renderer.quad_buffer_ptr++;


	_SBRE_renderer.quad_buffer_ptr = _SBRE_renderer.quad_buffer;
}



static void _SBRE_set_vertex_buffer(void) {

	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_renderer.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), _SBRE_renderer.quad_buffer);
}



static void SBRE_set_text_vertices(Vec2 pos, float width, float height, Color color, float index, Rectangle text_coord) {

	Color normalized_color = NORMALIZE_RGBA(color.r, color.g, color.b ,color.a);

	_SBRE_renderer.text_buffer_ptr->pos = (Vec2){ pos.x, pos.y };
	_SBRE_renderer.text_buffer_ptr->color = normalized_color;
	_SBRE_renderer.text_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y + text_coord.height };
	_SBRE_renderer.text_buffer_ptr->index = index;
	_SBRE_renderer.text_buffer_ptr++;
	
	_SBRE_renderer.text_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y };
	_SBRE_renderer.text_buffer_ptr->color = normalized_color;
	_SBRE_renderer.text_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y + text_coord.height };
	_SBRE_renderer.text_buffer_ptr->index = index;
	_SBRE_renderer.text_buffer_ptr++;

	_SBRE_renderer.text_buffer_ptr->pos = (Vec2){ pos.x, pos.y + height};
	_SBRE_renderer.text_buffer_ptr->color = normalized_color;
	_SBRE_renderer.text_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y };
	_SBRE_renderer.text_buffer_ptr->index = index;
	_SBRE_renderer.text_buffer_ptr++;

	_SBRE_renderer.text_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y + height };
	_SBRE_renderer.text_buffer_ptr->color = normalized_color;
	_SBRE_renderer.text_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y };
	_SBRE_renderer.text_buffer_ptr->index = index;
	_SBRE_renderer.text_buffer_ptr++;
}



void SBRE_draw_quad(Vec2 pos, float width, float height, Color color) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 0);


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};


	/* Set Vertices */

	_SBRE_set_vertices(pos, width, height, color, 0, text_rect);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



void SBRE_draw_quad_ext(Vec2 pos, float width, float height, float rotation, Color color) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 0);


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};


	/* Set Vertices */

	_SBRE_set_vertices_rotated(pos, width, height, color, 0, text_rect, rotation);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



void SBRE_draw_quad_outline(Vec2 pos, float width, float height, float border, Color quad_color, Color border_color) {

	/* TODO : Make it so that this function works with border values bellow 0 */

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 0);


	/* Bind the Defualt Texture */

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id); 


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};


	/* Draw the Mask Rectangle */

	/* Set Vertices */

	// NOTE : Added border twice to the width and height because, its also being added to the position.
	_SBRE_set_vertices(SBRE_VEC2(pos.x + border, pos.y + border), width - border * 2, height - border * 2, quad_color, 0, text_rect);


	/* Set the Stencil Buffer to Write */

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	/* Draw the Outline */ 

	/* Set Vertices */

	_SBRE_set_vertices(pos, width, height, border_color, 0, text_rect);


	/* Set the Stencil Buffer to Only Draw When not 1 */

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	/* Set the Stencil Buffer to Normal Again */

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
}



void SBRE_draw_quad_outline_ext(Vec2 pos, float width, float height, float border, float rotation, Color quad_color, Color border_color) {

	/* TODO : Make it so that this function works with border values bellow 0 */

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 0);


	/* Bind the Defualt Texture */

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id); 


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};


	/* Draw the Mask Rectangle */

	/* Set Vertices */

	// NOTE : Added border twice to the width and height because, its also being added to the position.
	_SBRE_set_vertices_rotated(SBRE_VEC2(pos.x + border, pos.y + border), width - border * 2, height - border * 2, quad_color, 0, text_rect, rotation);


	/* Set the Stencil Buffer to Write */

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	/* Draw the Outline */ 

	/* Set Vertices */

	_SBRE_set_vertices_rotated(pos, width, height, border_color, 0, text_rect, rotation);


	/* Set the Stencil Buffer to Only Draw When not 1 */

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	/* Set the Stencil Buffer to Normal Again */

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
}



void SBRE_draw_circle(Vec2 pos, float radius, Color color) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
		SBRE_use_shader(_SBRE_default_circle_shader);


	/* Send default texture to the shader */

	int sampler[16];
	for (int i = 0; i < 16; ++i)
		sampler[i] = i;
	int32_t location = glGetUniformLocation(_SBRE_active_shader, "u_textures");
	glUniform1iv(location, 16, sampler);


	SBRE_set_uniform_1f(_SBRE_active_shader, "u_thickness", 1);


	/* Send the default mvp */

	Mat4 mvp = _SBRE_calculate_mvp();
	SBRE_set_uniform_mat4f(_SBRE_active_shader, "u_mvp", mvp);


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};


	/* Set Vertices */

	_SBRE_set_vertices(pos, radius * 2, radius * 2, color, 0, text_rect);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




void SBRE_draw_circle_outline(Vec2 pos, float radius, float border, Color fill_color, Color outline_color) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
		SBRE_use_shader(_SBRE_default_circle_shader);


	/* Send default texture to the shader */

	int sampler[16];
	for (int i = 0; i < 16; ++i)
		sampler[i] = i;
	int32_t location = glGetUniformLocation(_SBRE_active_shader, "u_textures");
	glUniform1iv(location, 16, sampler);


	/* Send the default mvp */

	Mat4 mvp = _SBRE_calculate_mvp();
	SBRE_set_uniform_mat4f(_SBRE_active_shader, "u_mvp", mvp);


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};	


	/* Activate the Texture */

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _SBRE_default_texture->texture_id);



	/* Draw the Fill */

	/* Set the fill thickness */

	SBRE_set_uniform_1f(_SBRE_active_shader, "u_thickness", 1);


	/* Set Vertices */

	_SBRE_set_vertices(pos, radius * 2, radius * 2, fill_color, 0, text_rect);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



	/* Draw the Outline */

	/* Set the outline thickness */

	SBRE_set_uniform_1f(_SBRE_active_shader, "u_thickness", border);


	/* Set Vertices*/

	_SBRE_set_vertices(pos, radius * 2, radius * 2, outline_color, 0, text_rect);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




void SBRE_draw_texture(Vec2 pos, Texture* texture, Rectangle* texture_rect) {
	
	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 0);


	/* Calculate Texture Position */

	Rectangle text_rect;

	if (texture_rect) {

		text_rect = (Rectangle) {
			.position = (Vec2) { texture_rect->position.x / texture->initial_width,  ((texture->initial_height - texture_rect->height) - texture_rect->position.y) / texture->initial_height},
			.width 	= texture_rect->width  / texture->initial_width,
			.height = texture_rect->height / texture->initial_height
		};
	}
	else {

		text_rect = (Rectangle) {
			.position = (Vec2) { 0.0f, 0.0f },
			.width 	= 1.0f,
			.height = 1.0f
		};
	}


	/* Set Vertices */

	_SBRE_set_vertices(pos, texture->width, texture->height, texture->color, 1, text_rect);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}



void SBRE_draw_texture_ext(Vec2 pos, Texture* texture, Rectangle* texture_rect, float rotation) {
	
	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 0);


	/* Calculate Texture Position */

	Rectangle text_rect;

	if (texture_rect) {

		text_rect = (Rectangle) {
			.position = (Vec2) { texture_rect->position.x / texture->initial_width,  ((texture->initial_height - texture_rect->height) - texture_rect->position.y) / texture->initial_height},
			.width 	= texture_rect->width  / texture->initial_width,
			.height = texture_rect->height / texture->initial_height
		};
	}
	else {

		text_rect = (Rectangle) {
			.position = (Vec2) { 0.0f, 0.0f },
			.width 	= 1.0f,
			.height = 1.0f
		};
	}


	/* Set Vertices */

	_SBRE_set_vertices_rotated(pos, texture->width, texture->height, texture->color, 1, text_rect, rotation);


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.vao);

	_SBRE_set_vertex_buffer();

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}



void SBRE_draw_text(Vec2 pos, const char* text, Font* font, Color color) {

	/* Default Shader */

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
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
	SBRE_set_uniform_1f(_SBRE_active_shader, "u_istext", 1);


	uint32_t quad_count = 0;
	_SBRE_renderer.text_buffer_ptr = _SBRE_renderer.text_buffer;

	Vec2 crnt_char_pos = pos;

	uint32_t text_len = strlen(text);
	for (int i = 0; i < text_len; ++i) {
		
		SBRE_Character crnt_char = font->_characters[text[i]];

		Rectangle text_coord = (Rectangle) {
			.position = (Vec2) { crnt_char.render_offset.x / font->font_atlas->initial_width, crnt_char.render_offset.y / font->font_atlas->initial_height},
			.width 	= crnt_char.size.x / font->font_atlas->initial_width,
			.height = crnt_char.size.y / font->font_atlas->initial_height
		};
		

		Vec2 char_pos = SBRE_VEC2(
			crnt_char_pos.x + crnt_char.bearing.x,
			crnt_char_pos.y + (font->biggest_char.y - crnt_char.size.y) + (crnt_char.size.y - crnt_char.bearing.y)
		);


		SBRE_set_text_vertices(char_pos, crnt_char.size.x, crnt_char.size.y, color, 1, text_coord);
		crnt_char_pos.x = char_pos.x + (crnt_char.advance >> 6); /* NOTE : Bitshift the value by 6 to get value in pixels */


		quad_count += 1;
		if (quad_count >= MAX_QUAD) {
			
			printf("[SBRE ERROR][Upper bound for not batched text rendering has been reached, SBRE were not able to render the whole text.\n");
			printf("\tSuggestion : Consider using batched text rendering, or render the text in multiple render calls.]\n");
			quad_count = MAX_QUAD;
			break;
		}
	}
	

	/* Index and Vertex Count */

	uint32_t vertex_count = quad_count * 4;
	uint32_t index_count  = quad_count * 6;


	/* Set Buffers */

	glBindVertexArray(_SBRE_renderer.text_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_renderer.text_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * sizeof(Vertex), (void*) _SBRE_renderer.text_buffer);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, font->font_atlas->texture_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_renderer.text_ebo);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}



/* Batch Renderer */

void SBRE_reset_batch_stats(void) {

	_SBRE_batch_renderer.rs_quad_count = 0;
	_SBRE_batch_renderer.rs_render_count = 0;
}



void SBRE_begin_batch(void) {

	if (_SBRE_active_shader == _SBRE_default_batch_shader || _SBRE_active_shader == _SBRE_default_shader || _SBRE_active_shader == _SBRE_default_circle_shader)
		SBRE_use_shader(_SBRE_default_batch_shader);


	/* Send the Samplers */
	int sampler[16];
	for (int i = 0; i < 16; ++i)
		sampler[i] = i;
	int32_t location = glGetUniformLocation(_SBRE_active_shader, "u_textures");
	glUniform1iv(location, 16, sampler);


	/* Send the default mvp */	
	Mat4 mvp = _SBRE_calculate_mvp();
	SBRE_set_uniform_mat4f(_SBRE_active_shader, "u_mvp", mvp);


	/* Move the pointer to the start of the buffer */
	_SBRE_batch_renderer.quad_buffer_ptr = _SBRE_batch_renderer.quad_buffer;
}



void SBRE_end_batch(void) {

	GLsizeiptr size = (uint8_t*) _SBRE_batch_renderer.quad_buffer_ptr - (uint8_t*) _SBRE_batch_renderer.quad_buffer;
	glBindBuffer(GL_ARRAY_BUFFER, _SBRE_batch_renderer.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, _SBRE_batch_renderer.quad_buffer);
}



void SBRE_render_batch(bool clear_stats) {

	/* Bind all textures */

	for (int i = 0; i < _SBRE_batch_renderer.texture_index; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _SBRE_batch_renderer.texture_slots[i]);
	}


	/* Draw the current batch */

	glBindVertexArray(_SBRE_batch_renderer.vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _SBRE_batch_renderer.ebo);
	glDrawElements(GL_TRIANGLES, (int) _SBRE_batch_renderer.index_count, GL_UNSIGNED_INT, NULL);

	_SBRE_batch_renderer.rs_render_count++;


	/* Reset the texture index and index count */

	_SBRE_batch_renderer.index_count = 0;
	_SBRE_batch_renderer.texture_index = 1;


	/* Unbind all the textures */

	for (int i = 0; i < _SBRE_batch_renderer.texture_index; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (clear_stats)
		SBRE_reset_batch_stats();
}



/* Batch Renderer Render Functions */

static void _SBRE_batch_set_vertex_buffer(Vec2 pos, float width, float height, Color color, float index, Rectangle text_coord) {

	Color normalized_color = NORMALIZE_RGBA(color.r, color.g, color.b ,color.a);


	_SBRE_batch_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x, pos.y };
	_SBRE_batch_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_batch_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y + text_coord.height };
	_SBRE_batch_renderer.quad_buffer_ptr->index = index;
	_SBRE_batch_renderer.quad_buffer_ptr->draw_type = DRAW_TYPE_QUAD;
	_SBRE_batch_renderer.quad_buffer_ptr->thickness = 0;
	_SBRE_batch_renderer.quad_buffer_ptr++;
	
	_SBRE_batch_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y };
	_SBRE_batch_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_batch_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y + text_coord.height };
	_SBRE_batch_renderer.quad_buffer_ptr->index = index;
	_SBRE_batch_renderer.quad_buffer_ptr->draw_type = DRAW_TYPE_QUAD;
	_SBRE_batch_renderer.quad_buffer_ptr->thickness = 0;
	_SBRE_batch_renderer.quad_buffer_ptr++;

	_SBRE_batch_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x, pos.y + height};
	_SBRE_batch_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_batch_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x, text_coord.position.y };
	_SBRE_batch_renderer.quad_buffer_ptr->index = index;
	_SBRE_batch_renderer.quad_buffer_ptr->draw_type = DRAW_TYPE_QUAD;
	_SBRE_batch_renderer.quad_buffer_ptr->thickness = 0;
	_SBRE_batch_renderer.quad_buffer_ptr++;

	_SBRE_batch_renderer.quad_buffer_ptr->pos = (Vec2){ pos.x + width, pos.y + height };
	_SBRE_batch_renderer.quad_buffer_ptr->color = normalized_color;
	_SBRE_batch_renderer.quad_buffer_ptr->tex_coord = (Vec2){ text_coord.position.x + text_coord.width, text_coord.position.y };
	_SBRE_batch_renderer.quad_buffer_ptr->index = index;
	_SBRE_batch_renderer.quad_buffer_ptr->draw_type = DRAW_TYPE_QUAD;
	_SBRE_batch_renderer.quad_buffer_ptr->thickness = 0;
	_SBRE_batch_renderer.quad_buffer_ptr++;


	_SBRE_batch_renderer.index_count += 6;
	_SBRE_batch_renderer.rs_quad_count++;
}



void SBRE_batch_render_quad(Vec2 pos, float width, float height, Color color) {

	if (_SBRE_batch_renderer.index_count >= MAX_INDEX || _SBRE_batch_renderer.texture_index > MAX_TEXTURE_SLOTS) {

		SBRE_end_batch();
		SBRE_render_batch(false);
		SBRE_begin_batch();
	}


	/* Calculate Texture Position */

	Rectangle text_rect = (Rectangle) {
		.position = (Vec2) { 0.0f, 0.0f },
		.width 	= 1.0f,
		.height = 1.0f
	};


	/* Set Vertices */

	_SBRE_batch_set_vertex_buffer(pos, width, height, color, 0, text_rect);


	/* Send Vertex Data to Batch Buffer*/

	_SBRE_set_vertex_buffer();
}