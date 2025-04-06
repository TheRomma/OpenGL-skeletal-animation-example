#include "animation.hpp"

#include <fstream>
#include <cmath>

//Contains animation related calculations.
Animation::Animation(const char* filename){
	//Load animation.
	std::ifstream file(filename, std::ios::binary);
	if(file.is_open()){
		//Extract header.
		file.seekg(0);
		file.read((char*)(&header), sizeof(AnimationHeader));

		//Extract joints.
		joints.resize(header.numJoints * header.numFrames);
		file.seekg(header.animOffset);
		file.read((char*)joints.data(), header.animLength);

		//Close file.
		file.close();
	}
}

//Cleanup.
Animation::~Animation(){}

//Calculate joint trasforms of a pose.
void Animation::calcPose(
	float animTime,
	Mat4* transforms
){
	//Convert time from seconds to animation frames.
	float frameTime = animTime * (24.0f / (float)header.frameInterval);

	//Find the closest frame.
	uint32_t frameNumber = static_cast<uint32_t>(floor(frameTime)) % (header.numFrames - 1);

	//Find offsets to starting joints.
	uint32_t prev_joint = frameNumber * header.numJoints;
	uint32_t next_joint = (frameNumber + 1) * header.numJoints;

	//Fractional part of frameTime is used as the interpolation value.
	frameTime -= floor(frameTime);

	//Interpolate all joints.
	Vec3 translation, scale;
	Quat rotation;
	Mat4 composed;

	for(uint32_t j=0;j<header.numJoints;j++){
			translation = Vec3::lerp(
				joints[j + prev_joint].translation,
				joints[j + next_joint].translation,
				frameTime
			);

			rotation = Quat::slerp(
				joints[j + prev_joint].rotation,
				joints[j + next_joint].rotation,
				frameTime
			);

			scale = Vec3::lerp(
				joints[j + prev_joint].scale,
				joints[j + next_joint].scale,
				frameTime
			);

			composed = (
				Mat4::translate(translation) *
				Mat4::rotate(rotation) *
				Mat4::scale(scale)
			);

			//The joint transforms represent movement in relation to their
			//parent joint so each joint transform of a pose must be 
			//multiplied with their parent. Root joint has no parent.
			if(joints[j + prev_joint].parent == 0xFFFFFFFF){
				transforms[j] = composed;
			}else{
				transforms[j] = composed * transforms[joints[j + prev_joint].parent];
			}
		}
}
