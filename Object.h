#pragma once
#include "Model.h"
#include <glm/glm.hpp>



class Object
{
public:
	LODmodel* modelType;
	glm::vec3 modelPosition;
	glm::vec3 modelScale;
	glm::vec3 modelRotation;
	bool breakable;
	bool sway;
	float radius;


	Object(LODmodel *m, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,bool _breakable,bool _sway)
	{
		modelType = m;
		modelPosition = position;
		modelScale = scale;
		modelRotation = rotation;
		breakable = _breakable;
		radius = m->near->modelRendering_coefficient * max(modelScale.x, modelScale.z);
		sway = _sway;
		//border.min = m->border.min * scale + position;
		//border.max = m->border.max * scale + position;
	}

	Object(){}

	void drawObject(Shader& shader,bool near)
	{
		glm::mat4 transformation_matrix = glm::mat4(1.0f);
		transformation_matrix = glm::translate(transformation_matrix, modelPosition);

		transformation_matrix = glm::scale(transformation_matrix, modelScale);

		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(modelRotation.x), glm::vec3(1, 0, 0));
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(modelRotation.y), glm::vec3(0, 1, 0));
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(modelRotation.z), glm::vec3(0, 0, 1));

		shader.setMat4("model", transformation_matrix);
		Model* target;
		if (near) target = modelType->near;
		else target = modelType->far;
		target->Draw(shader);
	}
};

