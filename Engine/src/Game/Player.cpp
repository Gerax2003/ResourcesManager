#include <GLFW/glfw3.h>
#include <memory>

#include "Core/Model.hpp"
#include "Core/Constants.hpp"
#include "Core/Maths.hpp"
#include "Game/Entity.hpp"
#include "Game/Player.hpp"
#include "Game/Particles.hpp"
#include "Game/Weapon.hpp"
#include "Game/Explosion.hpp"
#include "Game/Gunsmoke.hpp"
#include "Game/Enemy.hpp"
#include "Core/Camera.hpp"


Player::Player(const LowRenderer::Model& model, LowRenderer::Camera* camera, GLFWwindow* inWindow) : Entity(model)
{
	window = inWindow;
	p_camera = camera;
	velocity = {};
	weight = 2;
}

Player::~Player()
{
}

void Player::update(float deltaTime, const std::vector<Platform>& platforms, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Particle>>& particles, Weapon& weapon)
{
	if (!isGrounded)
		velocity.y -= GRAVITY * deltaTime * weight;
	else
		velocity.y = 0;

	playerDir = { cosf(p_camera->pitch) * cosf(p_camera->yaw + PI / 2), -sinf(p_camera->pitch), cosf(p_camera->pitch) * sinf(-(p_camera->yaw + PI / 2)) };

	Vector3 maxRange = p_camera->position.xyz + playerDir * rangeShoot;
	
	Vector3 intersectPoint = {};
	Vector3 intersectNormal = { 0.f, 0.f, 0.f };

	if (RPressed)
	{
		bulletNumber = 5;
		reloadTimer = maxReloadTimer;
		weapon.reload_animation(deltaTime);
	}

	if (canFire)
	{
		shootTimer = maxShootTimer; 
		bulletNumber -= 1;
		Vector3 rotation = { 0.f, 0.f, PI / 2.f };
		particles.push_back(std::make_shared<Smoke>(Smoke(weapon.canon.position, rotation, 0.05f, 0.1f, (LowRenderer::Model*)&weapon.model)));
		
		for (int i = 0; i < enemies.size(); i++)
		{
			if (Maths::intersectSegmentSphere(p_camera->position.xyz, maxRange, enemies[i]->collider, intersectPoint, intersectNormal))
			{
				particles.push_back(std::make_shared<Explosion>(Explosion(intersectPoint)));
				enemies.erase(enemies.begin() + i);
			}
		}
		for (int i = 0; i < platforms.size(); i++)
		{
			if (Maths::intersectSegmentBox(p_camera->position.xyz, maxRange, platforms[i].collider, intersectPoint, intersectNormal))
			{
				particles.push_back(std::make_shared<Explosion>(Explosion(intersectPoint)));
			}
		}

	}

	if (shootTimer > 0.f) shootTimer -= deltaTime;
	if (reloadTimer > 0.f) reloadTimer -= deltaTime;

	if (downPressed && !isDown)
	{
		save = model.position.y;
		model.position.y -= 0.75f;
		maxHorizontalSpeed = 0.1f;
		isDown = true;
	}
	else if(!downPressed && isDown)
	{
		model.position.y = save;
		maxHorizontalSpeed = 0.25f;
		isDown = false;
	}

	checkEvents();
	
	Vector3 left = { 1,0,0 };
	Vector3 up = { 0,1,0 };
	Vector3 forward = { 0,0,1 };

	quaternionEntity = Maths::QuaternionFromEuler(0, model.rotation.y, 0);

	left = Maths::Vector3RotateByQuaternion(left, quaternionEntity);
	left = Maths::normalizeVector3(left);

	up = Maths::Vector3RotateByQuaternion(up, quaternionEntity);
	up = Maths::normalizeVector3(up);

	forward = Maths::Vector3RotateByQuaternion(forward, quaternionEntity);
	forward = Maths::normalizeVector3(forward);

	float Forward_Mouvement = 0;
	float Straft_mouvement = 0;

	for (int i = 0; i < platforms.size(); i++)
	{
		intersectPoint = {};
		intersectNormal = { 0.f, 0.f, 0.f };

		if (Maths::intersectSphereBox(velocity, collider, platforms[i].collider, intersectPoint, intersectNormal) && i != 0)
		{
			model.position = intersectPoint + intersectNormal * 0.01f;
			collider.center = intersectPoint + intersectNormal * 0.01f;
			velocity = {};
			return;
		}

		if (leftPressed)
			Straft_mouvement = 1;

		if (rightPressed)
			Straft_mouvement = -1;

		if (forwardPressed)
			Forward_Mouvement = -1;

		if (backwardPressed)
			Forward_Mouvement = 1;

		velocity.x = -(Forward_Mouvement * sinf(-model.rotation.y) + Straft_mouvement * cosf(-model.rotation.y)) * speed * deltaTime;
		velocity.z = (Forward_Mouvement * cosf(-model.rotation.y) - Straft_mouvement * sinf(-model.rotation.y)) * speed * deltaTime;
	}

	velocity.y = Maths::clamp(velocity.y, -maxVerticalSpeed, 5.f);

	if (isGrounded && forwardPressed == false && backwardPressed == false && leftPressed == false && rightPressed == false)
	{
		velocity.x -= (velocity.x * deltaTime) * inertia;
		velocity.z -= (velocity.z * deltaTime) * inertia;
	}

	model.position = model.position + velocity;
	collider.center = model.position;
}

void Player::checkEvents()
{
#if AZERTY_KEYS
	leftPressed = glfwGetKey(window, GLFW_KEY_Q);
	forwardPressed = glfwGetKey(window, GLFW_KEY_Z);
#else
	leftPressed = glfwGetKey(window, GLFW_KEY_A);
	forwardPressed = glfwGetKey(window, GLFW_KEY_W);
#endif
	rightPressed = glfwGetKey(window, GLFW_KEY_D);
	backwardPressed = glfwGetKey(window, GLFW_KEY_S);
	
	downPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	RPressed = glfwGetKey(window, GLFW_KEY_R) && reloadTimer <= 0.f;
	canFire = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) && shootTimer <= 0.f && bulletNumber > 0;

	if (glfwGetKey(window, GLFW_KEY_SPACE) && isGrounded)
		velocity.y = jumpForce;
}