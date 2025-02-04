#pragma once

#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace std;

class NavPoint
{
public:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;

	string name;
	int ID = -1;

	bool open = false;
	bool selected = false;

	NavPoint(string _name)
	{
		this->name = _name;
		this->pos = glm::vec3(0);
		this->rot = glm::vec3(0);
		this->scale = glm::vec3(1);
		this->ID = -1;
	}
};