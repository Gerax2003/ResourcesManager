#pragma once

#include "Game/Particles.hpp"

class Explosion : public Particle
{
public:
	Explosion(const Vector3 position = {}, const float lifeTime = 1.f);
	~Explosion();

	void update(float deltaTime) override;
private:

};
