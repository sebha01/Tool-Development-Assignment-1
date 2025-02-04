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

	void setData(string& data)
	{
		pos = vecFromStr(data);
		rot = vecFromStr(data);
		scale = vecFromStr(data);
	}

	glm::vec3 vecFromStr(string& str)
	{
		glm::vec3 vec3out(0);
		string vec = str.substr(str.find("(") + 1, str.find(")"));
		str = str.substr(str.find(")") + 1);
		for (int j = 0; j < 3; j++)
		{
			vec3out[j] = std::stof(vec.substr(0, vec.find(",")));
			vec = vec.substr(vec.find(",") + 1);
		}
		return vec3out;
	}

	std::string toXML() 
	{
		std::stringstream ss;
		ss << "<Name>" << name << "</Name>";
		ss << "<Data>" << glm::to_string(pos) << glm::to_string(rot) << glm::to_string(scale) << "</Data>";
		return ss.str();
	}
};