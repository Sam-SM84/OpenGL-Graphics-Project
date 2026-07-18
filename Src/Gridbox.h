#pragma once
#include "commonLibraries.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
class Gridbox
{
public:
	unsigned int itemID;
	int count;
	glm::vec2 maxPos;
	glm::vec2 minPos;
	glm::vec3 color;

	Gridbox(glm::vec2 min,glm::vec2 max,glm::vec3 _color);
	Gridbox();
	void setup(glm::vec2 min, glm::vec2 max, glm::vec3 _color);
	static void initialize();
	void draw(Shader& shader);
	void setItem(unsigned int _itemID, int _count);
	bool hovers(float mouseX, float mouseY);

private:
	static unsigned int VAO, VBO, EBO;
	static unsigned int indeces[];
	static float vertices[];

};


