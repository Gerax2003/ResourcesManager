
#include "Core/Interface.hpp"

Interface::Interface()
{
	isInCreditsMenu = false;
	MouseX = 0;
	MouseY = 0;
	wMposX = 0;
	wMposY = 0;
}

Interface::~Interface()
{
}

void Interface::update(GLFWwindow* window, int& curScene, std::vector<LowRenderer::Model>& models)
{
	glfwGetCursorPos(window, &wMposX, &wMposY);

	MouseX = -ORTHOGRAPHIC_SRC_SIZE + (wMposX / SCR_WIDTH) * 20;
	MouseY = -ORTHOGRAPHIC_SRC_SIZE + (wMposY / SCR_HEIGHT) * 20;

	//std::cout << "X : " << MouseX << "| Y : " << MouseY << std::endl;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) && !isInCreditsMenu && !isInGame)
	{
		if (MouseX > -1.6f && MouseX < 1.7f && MouseY > 1.5f && MouseY < 3.5f)
		{
			startButton(window, curScene);
		}
		else if (MouseX > -1.6f && MouseX < 1.7f && MouseY > 4 && MouseY < 6)
		{
			creditsButton(models);
		}
		else if (MouseX > -1.6f && MouseX < 1.7f && MouseY > 6.5f && MouseY < 8.5f)
		{
			quitButton(window);
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) && isInCreditsMenu && !isInGame)
	{
		if (MouseX > 4.2f && MouseX < 4.7f && MouseY > -3.75f && MouseY < -2.72f)
		{
			closecreditsButton(models);
		}
	}
}

void Interface::startButton(GLFWwindow* window, int& curScene)
{
	curScene = 1;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Interface::creditsButton(std::vector<LowRenderer::Model>& models)
{
	LowRenderer::Model creditsBannerModel = { { 0,0,-0.01f }, { 0,0,0 }, { 15.f,7.f,0.1f }, "resources/credits_banner.png", "resources/quad.obj", "resources/skybox", nullptr };
	models.push_back(creditsBannerModel);

	LowRenderer::Model closeModel = { { 8,3.f,-0.01f }, { 0,0,0 }, { 1,1,0.1f }, "resources/close.png", "resources/quad.obj", "resources/skybox", nullptr };
	models.push_back(closeModel);

	isInCreditsMenu = true;
}

void Interface::closecreditsButton(std::vector<LowRenderer::Model>& models)
{
	models.pop_back();
	models.pop_back();

	isInCreditsMenu = false;
}

void Interface::optionButton()
{

}

void Interface::quitButton(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, true);
}