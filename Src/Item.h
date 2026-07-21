#pragma once
#include "commonLibraries.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
class Item
{
public:
	unsigned int ID, itemTexture;
	const char* path;
	Item();
	Item(unsigned int _ID, const char* _path);
	unsigned int loadTexture();
	

};

