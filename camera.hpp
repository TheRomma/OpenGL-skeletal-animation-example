#pragma once

#include <cinttypes>

#include "3Dmath.hpp"

struct Camera{
	Camera(
		Vec3 position,
		float yaw,
		float pitch,
		float vFov,
		float sensitivity
	);
	~Camera();

	void updateDirection(
		float dYaw,
		float dPitch
	);

	Mat4 calcProjView(
		float aspect
	);

	Vec3 position, direction, forward, right;
	float yaw, pitch, vFov, sensitivity;
};
