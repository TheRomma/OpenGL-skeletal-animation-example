#include "scene.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "animation.hpp"

//Contains the scene for the assignment.
uint32_t testScene(Renderer* renderer){
	//Create a shader program.
	uint32_t program = albedoShader();

	//Create model resources.
	Model model("res/models/test.am");
	Animation anim("res/anims/test.anim");
	Mat4 joints[anim.header.numJoints];

	//Get keyboard state.
	const uint8_t* keyboard = SDL_GetKeyboardState(NULL);

	//Create camera variables.
	Camera camera(
		{0.0f, -5.0f, 2.5f},
		0.0f,
		0.0f,
		1.2f,
		0.01f
	);

	//Program loop.
	bool alive = true;
	bool mouseLocked = false;
	uint64_t frameCount = 0;
	SDL_Event event;
	while(alive){

		//Event polling.
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					alive = false;
					break;

				case SDL_KEYUP:
					switch(event.key.keysym.scancode){
						//Quit if ESC is pressed.
						case SDL_SCANCODE_ESCAPE:
							alive = false;
							break;

						//Lock mouse cursor to window.
						case SDL_SCANCODE_M:
							mouseLocked = !mouseLocked;
							if(mouseLocked)
								SDL_SetRelativeMouseMode(SDL_TRUE);
							else
								SDL_SetRelativeMouseMode(SDL_FALSE);
							break;

						default:
							break;
					}
					break;
				
				case SDL_MOUSEMOTION:
					camera.updateDirection(event.motion.xrel, event.motion.yrel);
					break;
			}
		}

		//Update scene.
		if(keyboard[SDL_SCANCODE_W]){camera.position = camera.position + camera.direction * 0.08f;}
		if(keyboard[SDL_SCANCODE_S]){camera.position = camera.position - camera.direction * 0.08f;}
		if(keyboard[SDL_SCANCODE_A]){camera.position = camera.position - camera.right * 0.08f;}
		if(keyboard[SDL_SCANCODE_D]){camera.position = camera.position + camera.right * 0.08f;}
		if(keyboard[SDL_SCANCODE_C]){camera.position.z -= 0.08f;}
		if(keyboard[SDL_SCANCODE_V]){camera.position.z += 0.08f;}

		//Draw scene.
		renderer->beginFrame();

		glUseProgram(program);

		Mat4 projView = camera.calcProjView(renderer->calcAspect());
		glUniformMatrix4fv(0, 1, false, &projView.mat[0][0]);

		anim.calcPose((float)frameCount * 0.01f, joints);
		glUniformMatrix4fv(2, anim.header.numJoints, false, &joints[0].mat[0][0]);

		model.draw(Mat4::rotate({{0.0f, 0.0f, 1.0f}, (float)frameCount * 0.01f}));

		renderer->endFrame();

		frameCount++;
	}
	
	//Scene cleanup.
	glDeleteProgram(program);

	return 0;
}
