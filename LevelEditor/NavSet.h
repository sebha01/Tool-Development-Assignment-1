#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <iostream>

#include "NavPoint.h"

using namespace std;

class NavSet
{
public:
	vector<NavPoint> navPoints;
	Model* navModel = nullptr;
    bool open = false;

	void init() //Loads a Sphere.obj model
	{
		navModel = new Model("../../Resources/Models/Sphere.obj", "Sphere");
	};
	
	void draw(glm::mat4 viewProjectionMatrix)
	{
		//For each navPoint in navPoints
        for (NavPoint& navPoint : navPoints)
        {
            // Create a model matrix from the position, rotation, and scale of the navPoint
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1), navPoint.pos);

            // Set glStencilFunc to the navPoint's ID
            glStencilFunc(GL_ALWAYS, navPoint.ID, 0xFF);

            // Draw the sphere model with the model matrix
            if (navModel)
            {
                navModel->draw(viewProjectionMatrix * modelMatrix);
            }
        }
	}
};