#include "3Dmath.hpp"

#include <cstring>
#include <cstdio>

//Vectors.

//Null constructor.
Vec3::Vec3(){
	memset(&x, 0, 12);
}

//Constructor.
Vec3::Vec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

//Constructor.
Vec3::Vec3(float s){
	memset(&x, *(int*)&s, 12);
}

//Constructor.
Vec3::Vec3(float* arr){
	memcpy(&x, arr, 12);
}

//Vec3 + Vec3.
Vec3 Vec3::operator+(const Vec3& a){
	return {x + a.x, y + a.y, z + a.z};
}

//Vec3 - Vec3.
Vec3 Vec3::operator-(const Vec3& a){
	return {x - a.x, y - a.y, z - a.z};
}

//Vec3 * float.
Vec3 Vec3::operator*(const float& s){
	return {x * s, y * s, z * s};
}

//Vec3 * float.
Vec3 Vec3::operator/(const float& s){
	return {x / s, y / s, z / s};
}

//Vec3 * Vec3.
float Vec3::dot(Vec3 a, Vec3 b){
	return {a.x * b.x + a.y * b.y + a.z * b.z};
}

//Vec3 x Vec3.
Vec3 Vec3::cross(Vec3 a, Vec3 b){
	return {
		a.y * b.z - a.z * b.y,
		-(a.x * b.z - a.z * b.x),
		a.x * b.y - a.y * b.x
	};
}

//Vec3 length.
float Vec3::length(Vec3 a){
	return sqrt(Vec3::dot(a, a));
}

//Vec3 square length.
float Vec3::length2(Vec3 a){
	return Vec3::dot(a, a);
}

//Vec3 lerp Vec3.
Vec3 Vec3::lerp(Vec3 a, Vec3 b, float t){
	return a + (b - a) * t;
}

//Normalize Vec3.
Vec3 Vec3::normalize(Vec3 a){
	return a / Vec3::length(a);
}

//Vec3 abs.
Vec3 Vec3::absolute(Vec3 a){
	return Vec3(abs(a.x), abs(a.y), abs(a.z));
}

//Vec3 min.
Vec3 Vec3::min(Vec3 a, Vec3 b){
	return Vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
}

//Vec3 max.
Vec3 Vec3::max(Vec3 a, Vec3 b){
	return Vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
}

//Print values.
void Vec3::print(){
	printf("[%f, %f, %f]\n", x, y, z);
}

//Matrices.

//Null constructor.
Mat4::Mat4(){
	memset(&mat[0][0], 0, 64);
}

//Constructor.
Mat4::Mat4(float* arr){
	memcpy(&mat[0][0], arr, 64);
}

//Identity matrix.
Mat4 Mat4::identity(){
	float arr[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return {arr};
}

//Perspective matrix.
Mat4 Mat4::perspective(float vFov, float aspect, float near, float far){
	float top = tan(vFov * 0.5) * near;
	float right = aspect * top;
	float inv = 1.0f/(far-near);

	float arr[16] = {
		(2.0f*near)/(right+right), 0.0, 0.0, 0.0,
		0.0, (2.0f*near)/(top+top), 0.0, 0.0,
		0.0, 0.0, -(far+near)*inv, -1.0f,
		0.0, 0.0, -(2.0f*far*near)*inv, 0.0
	};
	return {arr};	
}

//View matrix.
Mat4 Mat4::lookTowards(Vec3 pos, Vec3 d, Vec3 up){
	d = Vec3::normalize(d);
	d = d * -1.0f;
	Vec3 r = Vec3::normalize(Vec3::cross(up, d));
	Vec3 u = Vec3::normalize(Vec3::cross(d, r));

	float arr[16] = {
		r.x, u.x, d.x, 0.0f,
		r.y, u.y, d.y, 0.0f,
		r.z, u.z, d.z, 0.0f,
		-Vec3::dot(pos, r), -Vec3::dot(pos, u), -Vec3::dot(pos, d), 1.0f
	};
	return {arr};
}

//Translation.
Mat4 Mat4::translate(Vec3 a){
	float arr[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		a.x , a.y , a.z , 1.0f
	};
	return {arr};
}

//Rotation.
Mat4 Mat4::rotate(Quat q){
	float arr_a[16] = {
		 q.w, q.z,-q.y, q.x,
		-q.z, q.w, q.x, q.y,
		 q.y,-q.x, q.w, q.z,
		-q.x,-q.y,-q.z, q.w
	};
	Mat4 a(arr_a);

	float arr_b[16] = {
		 q.w, q.z,-q.y,-q.x,
		-q.z, q.w, q.x,-q.y,
		 q.y,-q.x, q.w,-q.z,
		 q.x, q.y, q.z, q.w
	};
	Mat4 b(arr_b);

	return a * b;
}

//Scale.
Mat4 Mat4::scale(Vec3 a){
	float arr[16] = {
		a.x , 0.0f, 0.0f, 0.0f,
		0.0f, a.y , 0.0f, 0.0f,
		0.0f, 0.0f, a.z , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return {arr};
}

//Mat4 * Mat4.
Mat4 Mat4::operator*(const Mat4& a){
	float arr[4][4];
	memset(&arr[0][0], 0, 64);
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				arr[i][j] += mat[k][j]*a.mat[i][k];
			}
		}
	}
	return {&arr[0][0]};
}

//Mat4 * Vec3.
Vec3 Mat4::operator*(const Vec3& a){
	Vec3 vec;
	for(int i=0;i<3;i++){
		((float*)&vec.x)[i] = mat[0][i]*a.x + mat[1][i]*a.y + mat[2][i]*a.z + mat[3][i];
	}
	return vec;
}

//Print values.
void Mat4::print(){
	for(int i=0;i<4;i++)
		printf("[%f, %f, %f, %f]\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
}

//Quaternions.

//Null constructor.
Quat::Quat(){
	memset(&w, 0, 16);
}

//Constructor.
Quat::Quat(Vec3 axis, float angle){
	angle /= 2.0f;
	w = cos(angle);
	x = sin(angle) * axis.x;
	y = sin(angle) * axis.y;
	z = sin(angle) * axis.z;
}

//Constructor.
Quat::Quat(float w, float x, float y, float z){
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

//Constructor.
Quat::Quat(float* arr){
	memcpy(&w, arr, 16);
}

//Quat slerp Quat.
Quat Quat::slerp(Quat& a, Quat& b, float& t){
	float dot = fmin(a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z, 1.0);
	float theta = acos(dot);
	float sTheta = sin(theta);

	if(fabs(sTheta) < 0.001f){
		Quat result = Quat(
			0.5f * a.w + 0.5f * b.w,
			0.5f * a.x + 0.5f * b.x,
			0.5f * a.y + 0.5f * b.y,
			0.5f * a.z + 0.5f * b.z
		);

		return Quat::normalize(result);
	}

	float ta = sin((1.0f - t) * theta) / sTheta;
	float tb = sin(t * theta) / sTheta;

	Quat result = {
		a.w * ta + b.w * tb,
		a.x * ta + b.x * tb,
		a.y * ta + b.y * tb,
		a.z * ta + b.z * tb
	};

	return Quat::normalize(result);
}

//Quat length.
float Quat::len(Quat& a){
	return sqrt(a.w *a.w + a.x * a.x + a.y * a.y + a.z * a.z);
}

//Normalize Quat.
Quat Quat::normalize(Quat& a){
	float l = Quat::len(a);
	return {
		a.w / l,
		a.x / l,
		a.y / l,
		a.z / l
	};
}

//Print values.
void Quat::print(){
	printf("[%f, %f, %f, %f]\n", w, x, y, z);
}
