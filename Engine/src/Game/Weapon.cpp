#include "Game/Weapon.hpp"

Weapon::Weapon(std::string inTexName, std::string inMeshName, std::string inShaderName) : model(inTexName, inMeshName, inShaderName)
{
}

Weapon::Weapon(const LowRenderer::Model& inModel) : model(inModel)
{
}

void Weapon::reload_animation(float deltatime)
{
	reloading = true;
}

void Weapon::update(float deltaTime)
{
	if (reloading)
	{
		if (reloadingTimer > 0.5f)
			model.rotation.x -= 3.f * deltaTime;
		else if(reloadingTimer > 0.f)
			model.rotation.x += 3.f * deltaTime;
			else
		{
			reloadingTimer = maxReloadingTimer;
			reloading = false;
		}
		reloadingTimer -= deltaTime;
	}
	else
		model.rotation = initialRot;
}
