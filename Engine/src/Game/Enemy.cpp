#include "Game/Enemy.hpp"
#include "Core/Constants.hpp"

#include <iostream>
#include <stdlib.h>

Enemy::Enemy(const LowRenderer::Model& model) : Entity(model)
{
	velocity = {};
	maxHorizontalSpeed = .2f;
	weight = 1;
}

void Enemy::update(float deltaTime, const std::vector<Platform>& platforms, Player& player)
{
	Vector3 intersectPoint = {};
	Vector3 intersectNormal = { 0.f, 0.f, 0.f };

	if (!isGrounded)
		velocity.y -= GRAVITY * deltaTime * weight;
	else
		velocity.y = 0;

	float distance = (player.model.position.x - model.position.x) * (player.model.position.x - model.position.x) + (player.model.position.y - model.position.y) * (player.model.position.y - model.position.y) + (player.model.position.z - model.position.z) * (player.model.position.z - model.position.z);
	isChasing = distance <= range * range;


	Vector3 toPlayer = player.model.position - model.position;

	if (isChasing)
	{
		velocity = velocity + normalizeVector3(toPlayer) * maxHorizontalSpeed * deltaTime;

		if (Maths::dotVector3(toPlayer, velocity) <= 0)
		{
			velocity.x -= (velocity.x * deltaTime) * inertia;
			velocity.z -= (velocity.z * deltaTime) * inertia;
		}
	}
	else
	{
		velocity = {};
	}

	if (Maths::collisionSphereSphere(player.collider, collider))
	{
		player.model.position = player.respawn;
		player.collider.center = player.model.position;
		player.velocity = {};
		player.life -= 1;
		return;
	}

	for (int i = 0; i < platforms.size(); i++)
	{
		intersectPoint = {};
		intersectNormal = { 0.f, 0.f, 0.f };

		if (Maths::intersectSphereBox(velocity, collider, platforms[i].collider, intersectPoint, intersectNormal) && i != 0)
		{
			Vector3 PointAUp = { collider.center.x,collider.center.y + collider.radius * 2,collider.center.z };
			Vector3 PointBUp = { PointAUp.x + Maths::normalizeVector3(toPlayer).x * 2, PointAUp.x ,PointAUp.z + Maths::normalizeVector3(toPlayer).z * 2 };

			if (Maths::intersectSegmentBox(PointAUp, PointBUp, platforms[i].collider, intersectPoint, intersectNormal))
			{
				model.position = intersectPoint + intersectNormal * 0.01f;
				collider.center = intersectPoint + intersectNormal * 0.01f;
				velocity = {};
				return;
			}
			else 
			{
				if(isGrounded)
				{
					model.position.y += platforms[i].collider.dimensions.y;
					collider.center.y += platforms[i].collider.dimensions.y;
					return;
				}
			}
		}
	}



	Vector3 FrontEnemy = { Maths::normalizeVector3(toPlayer).x * collider.radius * 3, 0 , Maths::normalizeVector3(toPlayer).z * collider.radius * 3 };
	Vector3 PointAF = collider.center + FrontEnemy;
	Vector3 PointBF = {PointAF.x, PointAF.y - collider.radius * 5, PointAF.z};

	Vector3 PointAV = collider.center + velocity;
	Vector3 PointBV = { PointAV.x, PointAV.y - collider.radius * 5, PointAV.z };

	if (isGrounded)
	{
		intersectPoint = {};
		intersectNormal = { 0.f, 0.f, 0.f };

		if (!Maths::intersectSegmentBox(PointAF, PointBF, platforms[rgCurrentGroundPlatform].collider, intersectPoint, intersectNormal))
		{
			velocity = {};
			return;
		}
		else if (!Maths::intersectSegmentBox(PointAV, PointBV, platforms[rgCurrentGroundPlatform].collider, intersectPoint, intersectNormal))
		{
			velocity = {};
			return;
		}
	}

	model.rotation = model.rotation * toPlayer;

	velocity.x = Maths::clamp(velocity.x, -maxHorizontalSpeed, maxHorizontalSpeed);
	velocity.z = Maths::clamp(velocity.z, -maxHorizontalSpeed, maxHorizontalSpeed);
	velocity.y = Maths::clamp(velocity.y, -maxVerticalSpeed, 5.f);

	model.position = model.position + velocity;
	collider.center = model.position;
}