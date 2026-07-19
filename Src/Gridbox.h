#pragma once
#include "commonLibraries.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
class InventorySlot
{
public:
	unsigned int itemID;
	int count;
	glm::vec2 maxPos;
	glm::vec2 minPos;
	glm::vec3 color;

	InventorySlot(glm::vec2 min,glm::vec2 max,glm::vec3 _color);
	InventorySlot();
	void setup(glm::vec2 min, glm::vec2 max, glm::vec3 _color);
	static void initialize(const char* path);
	void draw(Shader& shader);
	void setItem(unsigned int _itemID, int _count);

private:
	static unsigned int VAO, VBO, EBO,textureID;
	static unsigned int indeces[];
	static float vertices[];

};


