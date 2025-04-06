#pragma once

#include <cinttypes>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

struct Renderer{
	Renderer(
		uint32_t width,
		uint32_t height
	);
	~Renderer();

	void beginFrame();
	void endFrame();
	float calcAspect();

	private:
	SDL_Window* window;
	SDL_GLContext context;
};
