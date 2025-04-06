#pragma once

#include <cinttypes>
#include <vector>

#include "3Dmath.hpp"

struct AnimationHeader{
	uint32_t numJoints;
	uint32_t numFrames;
	uint32_t frameInterval;
	uint32_t animOffset;
	uint32_t animLength;
};

struct Joint{
	uint32_t parent;
	Vec3 translation;
	Quat rotation;
	Vec3 scale;
};

struct Animation{
	Animation(const char* filename);
	~Animation();

	void calcPose(
		float animTime,
		Mat4* transforms
	);

	AnimationHeader header;
	std::vector<Joint> joints;
};
