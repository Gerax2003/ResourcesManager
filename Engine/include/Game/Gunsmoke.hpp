#pragma once

#include "Core/Model.hpp"
#include "Game/Particles.hpp"

class Smoke : public Particle
{
public:
	Smoke(const Vector3& position = {}, const Vector3& rotation = {}, const float scale = 1.f, const float lifeTime = 2.f, LowRenderer::Model* parent = nullptr);
	~Smoke() {};

	void update(float deltaTime) override;
private:
	Vector3 up = {0.f, 1.f, 0.f};
	float horizontalSpeed = 0.5f;
};
