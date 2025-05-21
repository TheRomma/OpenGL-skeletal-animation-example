#define SDL_MAIN_HANDLED

#include <cstdio>

#include "scene.hpp"
#include "3Dmath.hpp"

int main(int argc, char** argv){
	Renderer renderer(1280, 720);

	testScene(&renderer);

	return 0;
}
