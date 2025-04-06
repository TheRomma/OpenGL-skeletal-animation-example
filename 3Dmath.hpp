#pragma once

#include <cmath>

struct Vec3{
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(float s);
	Vec3(float* arr);
	~Vec3(){};

	Vec3 operator+(const Vec3& a);
	Vec3 operator-(const Vec3& a);
	Vec3 operator*(const float& s);
	Vec3 operator/(const float& s);
	static float dot(Vec3 a, Vec3 b);
	static Vec3 cross(Vec3 a, Vec3 b);
	static float length(Vec3 a);
	static float length2(Vec3 a);
	static Vec3 lerp(Vec3 a, Vec3 b, float t);
	static Vec3 normalize(Vec3 a);
	static Vec3 absolute(Vec3 a);
	static Vec3 min(Vec3 a, Vec3 b);
	static Vec3 max(Vec3 a, Vec3 b);

	void print();

	float x, y, z;
};

struct Quat{
	Quat();
	Quat(Vec3 axis, float angle);
	Quat(float w, float x, float y, float z);
	Quat(float* arr);
	~Quat(){};

	static Quat slerp(Quat& a, Quat& b, float& t);
	static float len(Quat& a);
	static Quat normalize(Quat& a);

	void print();

	float w, x, y, z;
};

struct Mat4{
	Mat4();
	Mat4(float* arr);
	~Mat4(){};

	static Mat4 identity();
	static Mat4 perspective(
		float vFov,
		float aspect,
		float near,
		float far
	);
	static Mat4 lookTowards(
		Vec3 pos,
		Vec3 dir,
		Vec3 up
	);
	static Mat4 translate(Vec3 a);
	static Mat4 rotate(Quat q);
	static Mat4 scale(Vec3 a);

	Mat4 operator*(const Mat4& a);
	Vec3 operator*(const Vec3& a);

	void print();

	float mat[4][4];
};
