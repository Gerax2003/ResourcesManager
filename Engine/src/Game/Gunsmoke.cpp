
#include "Core/Maths.hpp"
#include "Core/Model.hpp"
#include "Core/Constants.hpp"
#include "Game/Particles.hpp"
#include "Game/Gunsmoke.hpp"
#include <stdlib.h>

const int particlesNumber = 8;

Smoke::Smoke(const Vector3& position, const Vector3& rotation, const float scale, const float lifeTime, LowRenderer::Model* parent) : Particle(position, lifeTime)
{
	Vector3 forward = {
		cos(rotation.y) * cos(rotation.x),
		sin(rotation.y) * cos(rotation.x),
		sin(rotation.x)};
	Vector3 right = {
		cos(rotation.y + PI / 2.f) * cos(rotation.x),
		sin(rotation.y + PI / 2.f) * cos(rotation.x),
		sin(rotation.x) };

	up = Maths::normalizeVector3(Maths::crossProductVector3(forward, right));

	for (int i = 0; i < particlesNumber; i++)
	{
		models[i] = LowRenderer::Model("resources/blank.png", "resources/icosphere.obj", "resources/skybox", position,
			{ 0.f, (float)(rand() % 1000) / 100.f, 0.f }, ((float)(rand() % 100) / 2000.f + 0.01f) * scale);

		models[i].parent = parent;

		int color = rand() % 4;

		if (color == 0) 
			models[i].textureName = "resources/black.png";
		else if (color == 1) 
			models[i].textureName = "resources/lightGrey.png";
		else if (color == 2) 
			models[i].textureName = "resources/grey.png";
		else 
			models[i].textureName = "resources/darkGrey.png";

		Maths::Vector3 direction = {cos(rotation.y + (float)(rand() % 100) / 100) * cos(rotation.x), sin(rotation.y + (float)(rand() % 100) / 100) * cos(rotation.x), sin(rotation.x) };
		speeds[i] = (float)(rand() % 100) / 300.f + 0.4f;
		directions[i] = Maths::normalizeVector3(direction);
	}
}

void Smoke::update(float deltaTime)
{
	horizontalSpeed -= (currentTime / timer) * 0.5f;
	horizontalSpeed = clamp(horizontalSpeed, 0.f, 100.f);

	for (int i = 0; i < particlesNumber; i++)
	{
		Maths::Vector3 movement = deltaTime * horizontalSpeed * (float)(rand() % 100) / 100.f * directions[i];
		movement = movement + up * speeds[i] * deltaTime;
		models[i].position = models[i].position + movement;
	
		if (currentTime / timer >= 0.6f)
			models[i].scale = models[i].scale / (1.1f);
	}

	currentTime += deltaTime;
	if (currentTime >= timer) toDestroy = true;
}
