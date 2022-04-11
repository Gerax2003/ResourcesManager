#pragma once

#include <vector>
#include <chrono>
#include <string>
#include <memory>

#include "Core/Model.hpp"
#include "Core/Maths.hpp"
#include "Core/Light.hpp"
#include "Core/Camera.hpp"
#include "Core/ResourcesManager.hpp"
#include "Core/Constants.hpp"
#include "Core/Interface.hpp"

#include "Game/Player.hpp"
#include "Game/Enemy.hpp"
#include "Game/Platform.hpp"
#include "Game/Particles.hpp"
#include "Game/Weapon.hpp"

namespace Resources
{
	namespace Scene
	{
		struct ShaderData
		{
			LowRenderer::Light lights[LIGHTS_NUMBER];
			float camPos[4];
		};

		class Scene
		{
		public:
			Scene();
			Scene(ResourcesManager* inResourcesManager, GLFWwindow* window);
			Scene(ResourcesManager* inResourcesManager, GLFWwindow* window, const std::string filePath);
			~Scene() {};
			
			void load();
			void unload();
			void update(float deltaTime, GLFWwindow* window, Maths::Vector2& mousePos, int& currScene);
			
			void render();
			void renderModel(int model, const Maths::mat4x4& modelMat, const Maths::mat4x4& viewProj);
			void renderModel(LowRenderer::Model& model, const Maths::mat4x4& modelMat, const Maths::mat4x4& viewProj);

			LowRenderer::Camera camera;
			LowRenderer::CameraInputs camInputs{};
			ResourcesManager* resourcesManager;
		private:
			std::vector<Platform> platforms;
			std::vector<std::shared_ptr<Particle>> particles;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<LowRenderer::Model> models;
			std::vector<LowRenderer::Model> ui;

			Interface m_interface;
			Player player;
			Weapon weapon;
			ShaderData shaderData = {};

			std::chrono::steady_clock::time_point start;

			unsigned int UBO;

			bool isMenu = false;
			bool firstLoop = true;
			bool loadingEnd = false;

			void processInput(GLFWwindow* window, Maths::Vector2& mousePos, int& currScene);
			void updateCollisions();
			void updateShaderData();
			void checkGameState(GLFWwindow* window, int& currScene);
			
			bool bindModelResources(const LowRenderer::Model& model);
			
			void parseLineModel(std::string& line, const std::string& path);
			void parseLineCamera(std::string& line);
			void parseLineLight(std::string& line);

			void loadResourceWrapper(const std::string filePath, const ResourceType type);
		};
	}
}
