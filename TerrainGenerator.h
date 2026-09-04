#pragma once

#include "Mesh.h"
#include <vector>

float noise_2D(float x, float z,float std,float repeat)
{
	return sin(x * 0.3f) * cos(z * 0.3f) + sin(x * 0.1f + z * 0.15f) * 0.5f * std / repeat;
}

float getY_heightMap(float x, float z,float TERRAIN_MAX, float std, float repeat, unsigned char* data,int widthImage,int heightImage,int nrComponents)
{
	int imageX = (int)((x / TERRAIN_MAX) * (widthImage - 1));
	int imageZ = (int)((z / TERRAIN_MAX) * (heightImage - 1));
	int pixelIndex = (imageZ * widthImage + imageX) * nrComponents;
	float y = (data[pixelIndex] / 255.0f) * std;
	return y;
}

unsigned int load(const char* path)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

Mesh generateTerrain_Formula(float width, float depth, float repeat,float std,const char* path)
{
	vector<Vertex> vertices;
	vector<unsigned int> indeces;
	vector<Texture> texturesV;

											// vertex value
	for (int z = 0; z <= depth; z++)
	{
		for (int x = 0; x <= width; x++)
		{
			Vertex vertex;
			float wx = x * repeat;
			float wz = z * repeat;
			float y = noise_2D(wx, wz, std, repeat);


			vertex.position = glm::vec3(wx, y, wz);
			vertex.texCoords = glm::vec2(wx / width, wz / depth);
			vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);

			vertices.push_back(vertex);
		}
	}

											// index value
	int rowSize = width + 1;
	for (int z = 0; z < depth; z++)
	{
		for (int x = 0; x < width; x++)
		{
			int topLeft = z * rowSize + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * rowSize + x;
			int bottomRight = bottomLeft + 1;

			indeces.push_back(topLeft);
			indeces.push_back(bottomLeft);
			indeces.push_back(topRight);

			indeces.push_back(topRight);
			indeces.push_back(bottomLeft);
			indeces.push_back(bottomRight);
		}
	}

											// texture value
	Texture texture;
	texture.id = load(path);
	texture.path = path;
	texture.type = "texture_diffuse";
	texturesV.push_back(texture);
	return Mesh(vertices, indeces, texturesV);
}


Mesh generateTerrain_HeightMap(float width, float depth, float repeat, float std, const char* path,const char* heightMap)
{
	vector<Vertex> vertices;
	vector<unsigned int> indeces;
	vector<Texture> texturesV;

	int widthImage, heightImage, nrComponents;
	unsigned char* data = stbi_load(heightMap, &widthImage, &heightImage, &nrComponents,0);
	if (!data)
	{
		std::cout << "Heightmap failed to load at path: " << heightMap << std::endl;
		stbi_image_free(data);
		return Mesh(vertices, indeces, texturesV);
	}

											// vertex value
	for (int z = 0; z <= depth; z++)
	{
		for (int x = 0; x <= width; x++)
		{
			Vertex vertex;
			float wx = x * repeat;
			float wz = z * repeat;
			int imageX = (int)((wx / width) * (widthImage - 1));
			int imageZ = (int)((wz / depth) * (heightImage - 1));
			int pixelIndex = (imageZ * widthImage + imageX) * nrComponents;
			float y = (data[pixelIndex] / 255.0f) * std;

			vertex.position = glm::vec3(wx, y, wz);
			vertex.texCoords = glm::vec2(wx / width, wz / depth);
			vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);

			vertices.push_back(vertex);
		}
	}
	stbi_image_free(data);

											// index value
	int rowSize = width + 1;
	for (int z = 0; z < depth; z++)
	{
		for (int x = 0; x < width; x++)
		{
			int topLeft = z * rowSize + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * rowSize + x;
			int bottomRight = bottomLeft + 1;

			indeces.push_back(topLeft);
			indeces.push_back(bottomLeft);
			indeces.push_back(topRight);

			indeces.push_back(topRight);
			indeces.push_back(bottomLeft);
			indeces.push_back(bottomRight);
		}
	}

											// texture value
	Texture texture;
	texture.id = load(path);
	texture.path = path;
	texture.type = "texture_diffuse";
	texturesV.push_back(texture);
	return Mesh(vertices, indeces, texturesV);
}

