
#include <STB/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "Core/ResourcesManager.hpp"
#include "Core/Camera.hpp"
#include "Core/Physics.hpp"
#include "Core/Scene.hpp"
#include "Core/Constants.hpp"
#include "Core/App.hpp"


#pragma region INITIALIZATION
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

GLFWwindow* App::initOpenGL(bool& initOK)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PlatformerFPS", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	return window;
}

App::App::App(bool& initOK, GLFWwindow* inWindow)
{
	window = inWindow;

	scenes[0] = Resources::Scene::Scene(&resourcesManager, window, "resources/mainmenu.txt");
	scenes[1] = Resources::Scene::Scene(&resourcesManager, window, "resources/level.txt");
	scenes[2] = Resources::Scene::Scene(&resourcesManager, window, "resources/winmenu.txt");
	scenes[3] = Resources::Scene::Scene(&resourcesManager, window, "resources/gameovermenu.txt");

	currScene = 0;
	
	time = (float)glfwGetTime();

	double mPosX, mPosY;
	glfwGetCursorPos(window, &mPosX, &mPosY);
	mousePos = { (float)mPosX, (float)mPosY };
}
#pragma endregion

void App::App::run()
{
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = updateDeltaTime(time);
		processInput();

		resourcesManager.cleanQueue();
		scenes[currScene].update(deltaTime, window, mousePos, currScene);
		scenes[currScene].render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::App::processInput()
{
	// Used for debugging scene loading
	//if (glfwGetKey(window, GLFW_KEY_LEFT))
	//	if (currScene > 0) currScene--;
	//
	//
	//if (glfwGetKey(window, GLFW_KEY_RIGHT))
	//	if (currScene < SCENES_NUMBER - 1) currScene++;

#if AZERTY_KEYS
	if (glfwGetKey(window, GLFW_KEY_A))
#else
	if (glfwGetKey(window, GLFW_KEY_Q))
#endif
	{
		if (isCursorHidden)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		isCursorHidden = !isCursorHidden;
	}
}

App::App::~App()
{
	glfwTerminate();
}
