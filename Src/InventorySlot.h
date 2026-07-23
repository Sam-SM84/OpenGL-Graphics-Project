#pragma once
#include "commonLibraries.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Item.h"

class InventorySlot
{
public:
	Item* item;
	int count;
	glm::vec2 maxPos;
	glm::vec2 minPos;
	bool selected;

	InventorySlot(glm::vec2 min,glm::vec2 max);
	InventorySlot();
	void setup(Item* _item, unsigned int _count, glm::vec2 min,glm::vec2 max);
	void setItem(Item* item, unsigned int _count);
	static void initialize(const char* path,const char* usedPath);
	void draw(Shader& shader, float mouseX, float mouseY);
	bool hovers(float mouseX, float mouseY);
	

private:
	static unsigned int VAO, VBO, EBO,slotTexture,slotUsed;
	static unsigned int indeces[];
	static float vertices[];
	

};


