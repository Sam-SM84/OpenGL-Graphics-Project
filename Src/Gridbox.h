#pragma once
#include "commonLibraries.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
class Gridbox
{
public:
	unsigned int itemID;
	int count;
	glm::vec2 position;

	Gridbox();
	void setup();
	void draw(Shader& shader);
	void setItem(unsigned int _itemID, int _count);

private:
	unsigned int VAO, VBO, EBO;

};


