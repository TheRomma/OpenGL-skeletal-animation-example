#include "camera.hpp"

//Encapsulation for view and projection calculations.
Camera::Camera(
	Vec3 position,
	float yaw,
	float pitch,
	float vFov,
	float sensitivity
):
	position(position),
	yaw(0.0f),
	pitch(0.0f),
	vFov(vFov),
	sensitivity(sensitivity)
{
	updateDirection(yaw, pitch);
}

//Cleanup.
Camera::~Camera(){}

//Update camera direction.
void Camera::updateDirection(float dYaw, float dPitch){
	//Update yaw and pitch.
	yaw -= dYaw * sensitivity;
	pitch -= dPitch * sensitivity;

	if(pitch > 1.56f){pitch = 1.56f;}
	else if(pitch < -1.56f){pitch = -1.56f;}

	//Calculate camera direction.
	direction.x = -sin(yaw) * cos(pitch);
	direction.y = cos(yaw) * cos(pitch);
	direction.z = sin(pitch);

	//Calculate camera forward.
	forward = Vec3::normalize({direction.x, direction.y, 0.0f});

	//Calculate camera right.
	right = Vec3::cross(forward, {0.0f, 0.0f, 1.0f});	
}

//Calculate a projView matrix.
Mat4 Camera::calcProjView(float aspect){
	return {
		Mat4::perspective(vFov, aspect, 0.01f, 1000.0f) *
		Mat4::lookTowards(position, direction, {0.0f, 0.0f, 1.0f})
	};

}
