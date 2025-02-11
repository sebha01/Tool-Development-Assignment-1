#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "Model.h"
#include "Camera.h"
#include "Scene.h"
#include "FBO.h"

struct rect
{
	float posx, posy, width, height;
};

using namespace std;

struct VIEWPORT
{
	float left, bottom, width, height;
};


class GUIMgr
{
public:
	FBO* fbo;
	Scene scene;
	bool showFileBrowser = false;
	void init(GLFWwindow* window);
	void createFrame();
	void drawMenu();
	void drawPropertiesWindow(Model* model);
	void draw();//Finalise ImGUI rendering
	void shutDown();//cleanup ImGUI
	void drawConsoleWindow(std::stringstream* buffer);
	void drawAll(std::stringstream* buffer);
	void DrawModel(Model* node);
	void drawOpenGLWindow(Camera camera[], Camera_settings* camera_settings, rect* GLWINDOW_POS);
	FBO* getFBO() { return fbo; };
};

