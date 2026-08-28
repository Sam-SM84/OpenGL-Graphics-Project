#pragma once
#include "commonLibraries.h"
#include <glm/gtc/matrix_transform.hpp>
class Item
{
public:
	unsigned int ID, itemTexture;
	const char* path;
	std::string name;
	Item();
	Item(unsigned int _ID,std::string _name, const char* _path);
	unsigned int loadTexture();
	

};

