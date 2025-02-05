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
		// First, split the data string into three parts
		// We are expecting the format to be: vec3(x, y, z)vec3(x, y, z)vec3(x, y, z)

		// Find the first vec3
		size_t startPos = data.find("(") + 1;
		size_t endPos = data.find(")", startPos);
		string posStr = data.substr(startPos, endPos - startPos);
		data = data.substr(endPos + 1); // Remove the first vec3 from the string

		// Find the second vec3 (for rotation)
		startPos = data.find("(") + 1;
		endPos = data.find(")", startPos);
		string rotStr = data.substr(startPos, endPos - startPos);
		data = data.substr(endPos + 1); // Remove the second vec3 from the string

		// Find the third vec3 (for scale)
		startPos = data.find("(") + 1;
		endPos = data.find(")", startPos);
		string scaleStr = data.substr(startPos, endPos - startPos);

		// Now parse the individual vec3 strings
		pos = vec3FromStr(posStr);
		rot = vec3FromStr(rotStr);
		scale = vec3FromStr(scaleStr);
	}

	glm::vec3 vec3FromStr(string& str)
	{
		glm::vec3 vec3out(0);
		string vec = str;

		for (int j = 0; j < 3; j++)
		{
			// Find the first number in the string
			size_t start = vec.find_first_of("0123456789");
			size_t end = vec.find_first_not_of("0123456789.-"); // Find the first non-number
			vec3out[j] = std::stof(vec.substr(start, end - start));  // Convert the number
			vec = vec.substr(end);  // Move to the next number
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