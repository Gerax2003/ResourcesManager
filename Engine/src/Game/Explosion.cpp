
#include "Core/Maths.hpp"
#include "Core/Model.hpp"
#include "Core/Constants.hpp"
#include "Game/Particles.hpp"
#include "Game/Explosion.hpp"
#include <stdlib.h>

const int particlesNumber = 10;

Explosion::Explosion(const Vector3 position, const float lifeTime) : Particle(position, lifeTime)
{
	for (int i = 0; i < particlesNumber; i++)
	{
		models[i] = LowRenderer::Model("resources/red.png", "resources/cube.obj", "resources/skybox", position,
			{ (float)(rand() % 1000) / 100.f, (float)(rand() % 1000) / 100.f, (float)(rand() % 1000) / 100.f }, (float)(rand() % 100) / 2000.f + 0.05f);

		int color = rand() % 3;
		if (color == 0) models[i].textureName = "resources/red.png";
		else if (color == 1) models[i].textureName = "resources/orange.png";
		else if (color == 2) models[i].textureName = "resources/yellow.png";

		Maths::Vector3 direction = { (float)(rand() % 500) - 250.f, (float)(rand() % 500) - 250.f , (float)(rand() % 500) - 250.f };
		speeds[i] = (float)(rand() % 100) / 300.f + 0.4f;
		directions[i] = Maths::normalizeVector3(direction);
	}
}

Explosion::~Explosion()
{
}

void Explosion::update(float deltaTime)
{
	for (int i = 0; i < particlesNumber; i++)
	{
		Maths::Vector3 movement = speeds[i] * directions[i] * deltaTime;
		models[i].position = models[i].position + movement;
	}

	currentTime += deltaTime;
	if (currentTime >= timer) toDestroy = true;
}
