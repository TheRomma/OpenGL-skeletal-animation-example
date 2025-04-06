#include "model.hpp"

#include <vector>
#include <fstream>

//Encapsulates resources for a triangle mesh 3D model.
Model::Model(const char* filename){
	//Read model file.
	std::ifstream file(filename, std::ios::binary);
	if(file.is_open()){
		//Extract header.
		file.seekg(0);
		file.read((char*)(&header), sizeof(ModelHeader));

		//Create vao.
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Create vbo.
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			char vertices[header.meshLength];
			file.seekg(header.meshOffset);
			file.read(vertices, header.meshLength);
			glBufferData(GL_ARRAY_BUFFER, header.meshLength, vertices, GL_STATIC_DRAW);
		}

		//Define vertex attributes.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 10 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 10 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, 10 * sizeof(float), (void*)(6 * sizeof(float)));

		//Create a color texture.
		glGenTextures(1, &color);
		glBindTexture(GL_TEXTURE_2D_ARRAY, color);
		{
			char colors[header.colorLength];
			file.seekg(header.colorOffset);
			file.read(colors, header.colorLength);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, header.texWidth, header.texHeight, header.texDepth, false, GL_RGBA, GL_UNSIGNED_BYTE, colors);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/*	Not used in this assignment for now.
		//Create a PBR texture.
		glGenTextures(1, &PBR);
		glBindTexture(GL_TEXTURE_2D_ARRAY, PBR);
		{
			char materials[header.materialLength];
			file.seekg(header.materialOffset);
			file.read(materials, header.materialLength);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, 1, 1, header.texDepth, false, GL_RGBA, GL_FLOAT, materials);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		file.close();
		*/
	}
}

//Cleanup.
Model::~Model(){
	//glDeleteTextures(1, &PBR);
	glDeleteTextures(1, &color);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

//Draw model.
void Model::draw(Mat4 transform){
	glBindVertexArray(vao);

	glUniformMatrix4fv(1, 1, false, &transform.mat[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, color);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, PBR);

	glDrawArrays(GL_TRIANGLES, 0, header.numVertices);
}
