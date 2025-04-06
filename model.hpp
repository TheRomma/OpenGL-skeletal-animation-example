#pragma once

#include <cinttypes>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "3Dmath.hpp"

struct ModelHeader{
	uint32_t numVertices;
	uint32_t meshOffset;
	uint32_t meshLength;
	uint32_t texWidth;
	uint32_t texHeight;
	uint32_t texDepth;
	uint32_t colorOffset;
	uint32_t colorLength;
	uint32_t materialOffset;
	uint32_t materialLength;
};

struct Model{
	Model(const char* filename);
	~Model();

	void draw(Mat4 transform);

	private:
	ModelHeader header;
	uint32_t vao, vbo, color, PBR;
};
