#pragma once
#include "Core/Model.hpp"
#include "Core/Maths.hpp"

#include "Game/Platform.hpp"
#include "Game/Weapon.hpp"

class Particle;
class Enemy;

class Entity
{
public:
	LowRenderer::Model model;
	Maths::Vector3 velocity = {};
	int maxLife = 5;
	int life;
	float speed = 20.f;
	float weight = 0.f;
	bool isGrounded = false;
	int rgCurrentGroundPlatform = 0;
	Quaternion quaternionEntity = Maths::QuaternionFromEuler(0,0,0);

	Entity(std::string inTexName, std::string inMeshName, std::string inShaderName);
	Entity(const LowRenderer::Model& inModel);
	Entity() : model() { life = maxLife; };
	~Entity();

	virtual void update(float deltaTime,const std::vector<Platform>& platforms) {};
	virtual void update(float deltaTime, const std::vector<Platform>& platforms, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Particle>>& particles, Weapon& weapon) {};
};
