#pragma once

#include <cinttypes>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

uint32_t compileShader(uint32_t type, const char** source);
uint32_t linkShader(uint32_t vertex, uint32_t fragment);

uint32_t compileGenericVertexShader();
uint32_t albedoShader();
