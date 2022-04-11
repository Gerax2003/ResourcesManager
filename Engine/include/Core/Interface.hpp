#pragma once

#include <STB/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "Core/Constants.hpp"
#include "Core/Model.hpp"

class Interface
{
public:
	Interface();
	~Interface();

	void update(GLFWwindow* window, int& curScene, std::vector<LowRenderer::Model>& models);

	void startButton(GLFWwindow* window, int& curScene);
	void optionButton();
	void creditsButton(std::vector<LowRenderer::Model>& models);
	void closecreditsButton(std::vector<LowRenderer::Model>& models);
	void quitButton(GLFWwindow* window);

	double wMposX, wMposY; // Mouse Pos in Windows
	double MouseX, MouseY; // Mouse Pos in Orthographic Camera

	bool isInCreditsMenu = false;
	bool isInGame = false;

private:

};