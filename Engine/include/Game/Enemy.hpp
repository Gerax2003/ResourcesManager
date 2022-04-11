#pragma once
#include <GLFW/glfw3.h>
#include "Game/Entity.hpp"
#include "Game/Player.hpp"

class Enemy : public Entity
{
public:
	Enemy() : Entity() {};
	Enemy(const LowRenderer::Model& model);
	~Enemy() {};
	float inertia = 10.f;
	float damage = 1.f;
	float range = 50.f;
	void update(float deltaTime, const std::vector<Platform>& platforms, Player& player);
	float maxHorizontalSpeed = 2.f;
	float maxVerticalSpeed = 2.f;
	Maths::Sphere collider = { model.position, 1.f };
	bool isChasing = false;
};
