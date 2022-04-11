//#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Core/App.hpp"

#define _CRTDBG_MAP_ALLOC // tracks leak in files it's included in
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

int main()
{
	// used to enable leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	bool init;
	GLFWwindow* window = App::initOpenGL(init);

	if (!init || window == nullptr)
		return -2;

	App::App app(init, window);

	if (!init)
		return -1;

	app.run();

	return 0;
}
