#pragma once
#include <string>
#include "Core/Maths.hpp"
#include "Core/Model.hpp"

class Platform
{
public:
	Platform(LowRenderer::Model inModel);
	Platform(Maths::Vector3 position = { 0.f, 0.f, 0.f }, Maths::Vector3 rotation = { 0.f, 0.f, 0.f }, Maths::Vector3 scale = { 1.f, 1.f, 1.f });
	Platform(std::string textureName, std::string shaderName, Maths::Vector3 position, Maths::Vector3 rotation, Maths::Vector3 scale);
	void test(float time){
		collider.center = { 30.0f, -1.0f + sinf(time), 0.0f };
		model.position = { 30.0f, -1.0f + sinf(time), 0.0f };
	};
	LowRenderer::Model model;
	Maths::Box collider;
};