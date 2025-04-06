#include "renderer.hpp"

//Renderer constructor.
Renderer::Renderer(
	uint32_t width,
	uint32_t height
){
	//Init SDL.
	SDL_Init(SDL_INIT_EVERYTHING);

	//Set window hints.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Setup SDL window.
	window = SDL_CreateWindow(
		"Computer Graphics 2025",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
	);

	//Setup opengl context.
	context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	//Load modern opengl extensions.
	glewExperimental = true;
	glewInit();

	//Set adaptive vsync.
	SDL_GL_SetSwapInterval(-1);
}

//Renderer cleanup.
Renderer::~Renderer(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//Prepare drawing a frame.
void Renderer::beginFrame(){
	//Set viewport to match the window dimentions.
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);

	//Bind default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Set depth testing and face culling.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	//Clear color.
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

//Submit the frame to be displayed.
void Renderer::endFrame(){
	SDL_GL_SwapWindow(window);
}

//Returns the windows current aspect ratio.
float Renderer::calcAspect(){
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return (float)w/(float)h;
}
