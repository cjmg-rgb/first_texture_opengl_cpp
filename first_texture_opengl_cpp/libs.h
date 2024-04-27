#pragma once

#include "iostream"
#include "string"
#include "fstream"

#include "glew.h"
#include "glfw3.h"
#include "SOIL2.h"

#include "glm.hpp"
#include "mat4x4.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

struct Vertex
{
	glm::vec3 vertex_position;
	glm::vec3 vertex_color;
	glm::vec2 vertex_texcoord;
};

const char* vertexShaderSrc = R"glsl(
	
	#version 440

	layout (location = 0) in vec3 vertex_position;
	layout (location = 1) in vec3 vertex_color;
	layout (location = 2) in vec2 vertex_texcoord;

	out vec3 vs_color;
	out vec2 vs_texcoord;
	
	void main()
	{
		vs_color = vertex_color;
		vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);	

		gl_Position = vec4(vertex_position, 1.0);
	}
)glsl";

const char* fragmentShaderSrc = R"glsl(
	#version 440

	in vec3 vs_color;
	in vec2 vs_texcoord;

	out vec4 fs_color;

	uniform sampler2D texture0;
	uniform sampler2D texture1;

	void main()
	{
		//fs_color = vec4(vs_color, 1.0);	
		fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.0);
	}
)glsl";