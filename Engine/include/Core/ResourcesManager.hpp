#pragma once

#include "Maths.hpp"
#include "Constants.hpp"
#include "Resource.hpp"

#include <thread>
#include <atomic>

#include <deque>
#include <memory>
#include <string>
#include <glad/glad.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked

namespace Resources
{
	class Texture;
	class Shader;
	class Mesh;

	class ResourcesManager
	{
	private:
		std::thread threads[THREADS_NUMBER];
		std::deque<Resource*> toLoad;
		std::deque<std::shared_ptr<Texture>> textures; 
		std::deque<std::shared_ptr<Mesh>> meshes; 
		std::deque<std::shared_ptr<Shader>> shaders;

		bool runThreads = true;

		void threadFunction(int threadID);
	public:
		// flag used to synchronize threads
		std::atomic_flag flag = ATOMIC_FLAG_INIT;
		
		ResourcesManager();
		~ResourcesManager() 
		{
			runThreads = false;

			while (flag.test_and_set()) { std::this_thread::yield(); }
			
			for (Resource* resource : toLoad)
				delete resource;
			
			flag.clear();

			for (int i = 0; i < THREADS_NUMBER; i++)
				threads[i].join();
		};

		int searchResource(const std::string filePath, const ResourceType type);
		void bindResource(int index, const ResourceType type, bool bindDefault = false);
		void checkResourcesState(const ResourceType type);

		void resetResources()
		{	
			while (flag.test_and_set()) { std::this_thread::yield(); }
			
			textures.clear();
			shaders.clear();
			meshes.clear();

			flag.clear();
			eraseQueue();
		}
		void eraseQueue();
		void cleanQueue();
		void addToLoad(const std::string filePath, const ResourceType type);
		void loadResource(const std::string filePath, const ResourceType type);
		void createResource(const std::string filePath, const ResourceType type);
		void destroyResource(const int index, const ResourceType type);

		size_t getArraySize(ResourceType type) 
		{
			switch (type)
			{
			case Resources::ResourceType::MESH:
				return meshes.size();
			case Resources::ResourceType::SHADER:
				return shaders.size();
			case Resources::ResourceType::TEXTURE:
				return textures.size();
			default:
				return toLoad.size();
			}
		};
		Maths::int3 getUniforms();
		int getFirstShader();
		int getShaderProgram(const std::string& name);
		int getBufferSize(const std::string& name);
	};
}