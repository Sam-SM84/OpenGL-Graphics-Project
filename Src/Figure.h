#pragma once

#ifndef FIGURE_H
#define FIGURE_H

#include <glm/glm.hpp>
#include "Gridbox.h"
class Figure
{

public:
	glm::vec3 Position;
	glm::vec3 speed;
	glm::vec3 direction;

	Gridbox bag[5];
	bool onAir;

	Figure(glm::vec3 p)
	{
		Position = p;
		speed = { 0,0,0 };
		onAir = false;
	}

	Figure(){}

	void moveFigure(glm::vec3 move)
	{
		Position.x += move.x;
		Position.y += move.y;
		Position.z += move.z;
	}
	void applyGravity(float groundLevel, float gravity, float dt)
	{
		if (this->onAir)
		{
			//cout << "works\t";
			moveFigure({ 0,speed.y * dt * 2.5 ,0 });
			speed.y -= gravity * dt * 5;
			if (Position.y <= groundLevel)
			{
				this->setCoordinate({ Position.x,groundLevel,Position.z });
				this->speed.y = 0;
				this->onAir = false;
			}
		}
	}
	void jump()
	{
		onAir = true;
		speed.y = 2;
	}

	void setCoordinate(glm::vec3 c)
	{
		Position.x = c.x;
		Position.y = c.y;
		Position.z = c.z;
	}

private:
	bool between(float m, float a, float b)
	{
		return m > a && m < b;
	}

};
#endif
