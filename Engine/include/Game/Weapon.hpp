#pragma once

#include "Core/Model.hpp"

class Weapon
{
public:
	Weapon() {};
	~Weapon() {};
	Weapon(std::string inTexName, std::string inMeshName, std::string inShaderName);
	Weapon(const LowRenderer::Model& inModel);
	
	void update(float deltaTime);
	void reload_animation(float deltatime);
	
	bool reloading = false;

	float maxReloadingTimer = 1.f;
	float reloadingTimer = 0.f;

	Vector3 initialRot {0.f, 3.f, 0.f};

	Maths::Box weapon_collider {{1, 1, 1}, model.position, {0, 0, 0}};
	LowRenderer::Model model;
	LowRenderer::Model canon = LowRenderer::Model("resources/blank.png", "resources/blank.obj", "resources/skybox", { 0.005f, 0.02f, 0.04f }, {}, 0.01f, &model);

private:

};