#include "shader.hpp"

#include <cstdio>

//Compile a shader from glsl source.
uint32_t compileShader(uint32_t type, const char** source){
	uint32_t shader = glCreateShader(type);
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		char log[256];
		glGetShaderInfoLog(shader, 256, NULL, log);
		printf("Shader compilation failed: %s", log);
	}
	return shader;	
}

//Link compiled vertex and fragment shaders into a complete shader program.
uint32_t linkShader(uint32_t vertex, uint32_t fragment){
	uint32_t program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

//Compile a basic vertex shader.
uint32_t compileGenericVertexShader(){
	const char* vertex_source = R"(#version 430 core
		layout(location = 0) in vec3 POSITION;
		layout(location = 1) in uvec3 UV_NORM;
		layout(location = 2) in uvec4 JOINT;

		#define MAX_JOINT_TRANSFORMS 50

		out VS_OUT{
			vec3 position;
			vec3 uv_coord;
			vec3 normal;
		} F;

		layout(location = 0) uniform mat4 u_projView;
		layout(location = 1) uniform mat4 u_model;
		layout(location = 2) uniform mat4 u_joints[MAX_JOINT_TRANSFORMS];

		void main(){
			F.position = POSITION;
			
			F.uv_coord = vec3(
				unpackHalf2x16(UV_NORM.x),
				float(uint(UV_NORM.y & 0x0000FFFF))
			);

			F.normal = vec3(
				unpackHalf2x16(UV_NORM.y).g,
				unpackHalf2x16(UV_NORM.z)
			);

			uvec4 joints = {
				JOINT.r & 0x0000FFFF,
				JOINT.g & 0x0000FFFF,
				JOINT.b & 0x0000FFFF,
				JOINT.a & 0x0000FFFF
			};

			vec4 weights = {
				unpackHalf2x16(JOINT.r).g,
				unpackHalf2x16(JOINT.g).g,
				unpackHalf2x16(JOINT.b).g,
				unpackHalf2x16(JOINT.a).g
			};

			vec4 transformed = u_model * (
				u_joints[joints.r] * vec4(POSITION, 1.0f) * weights.r +
				u_joints[joints.g] * vec4(POSITION, 1.0f) * weights.g +
				u_joints[joints.b] * vec4(POSITION, 1.0f) * weights.b +
				u_joints[joints.a] * vec4(POSITION, 1.0f) * weights.a
			);

			F.normal = normalize(mat3(transpose(inverse(u_model))) * (
				mat3(transpose(inverse(u_joints[joints.r]))) * F.normal * weights.r +
				mat3(transpose(inverse(u_joints[joints.g]))) * F.normal * weights.g +
				mat3(transpose(inverse(u_joints[joints.b]))) * F.normal * weights.b +
				mat3(transpose(inverse(u_joints[joints.a]))) * F.normal * weights.a
			));

			gl_Position = u_projView * transformed;
		}
	)";
	uint32_t shader = compileShader(GL_VERTEX_SHADER, &vertex_source);
	return shader;
}

//Creates a shader program for displaying vertex normals.
uint32_t albedoShader(){
	const char* fragment_source = R"(#version 430 core
		out vec4 outColor;

		#define SUN_DIR normalize(vec3(1.0f, -1.0f, 1.0f))
		#define SUN_COLOR vec3(0.9f, 0.9f, 0.8f)
		#define AMBIENT_COLOR vec3(0.4f, 0.4f, 0.4f)

		in VS_OUT{
			vec3 position;
			vec3 uv_coord;
			vec3 normal;
		} F;

		layout(binding = 0) uniform sampler2DArray u_color;
		layout(binding = 1) uniform sampler2DArray u_PBR;

		vec3 calcColor(){
			vec3 albedo = texture(u_color, F.uv_coord).rgb;
			vec3 diffuse = SUN_COLOR * max(dot(F.normal, SUN_DIR), 0.0f);

			return (AMBIENT_COLOR + diffuse) * albedo;
		}

		void main(){
			outColor = vec4(calcColor(), 1.0f);
		}
	)";

	Uint32 vertex = compileGenericVertexShader();
	Uint32 fragment = compileShader(GL_FRAGMENT_SHADER, &fragment_source);

	return linkShader(vertex, fragment);
}
