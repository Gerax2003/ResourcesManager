
#include "Core/ResourcesManager.hpp"
#include "Core/Mesh.hpp"
#include "Core/Shader.hpp"
#include "Core/Texture.hpp"
#include "Core/Model.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

#include <thread>
#include <functional>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked
#define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

using namespace Resources;

ResourcesManager::ResourcesManager()
{
	if (THREADING_TYPE == 1)
		runThreads = false;

	for (int i = 0; i < 4; i++)
		threads[i] = std::thread(std::bind(&ResourcesManager::threadFunction, this, i));
}

#pragma region THREADING FUNCTIONS
// Adds a resource to the queue if not already in it
void ResourcesManager::addToLoad(const std::string filePath, const ResourceType type)
{
	for (Resource* resource : toLoad)
	{
		if (resource->m_name == filePath && resource->m_type == type)
			return;
	}

	switch (type)
	{
	case Resources::ResourceType::MESH:
		for (int i = 0; i < meshes.size(); i++)
			if (meshes[i]->m_name == filePath && meshes[i]->m_type == type)
				return;

		break;
	case Resources::ResourceType::SHADER:
		for (int i = 0; i < shaders.size(); i++)
			if (shaders[i]->m_name == filePath && shaders[i]->m_type == type)
				return;
		
		break;
	case Resources::ResourceType::TEXTURE:
		for (int i = 0; i < textures.size(); i++)
			if (textures[i]->m_name == filePath && textures[i]->m_type == type)
				return;

		break;
	default:
		return;
	}

	toLoad.push_back(new Resource(filePath, type));

	return;
}

void ResourcesManager::threadFunction(int threadID)
{
	if (THREADING_TYPE == 1) return;

	while (runThreads)
	{
		// spinlocks if nothing to load/other thread using toLoad

		for (int i = 0; i < toLoad.size(); i++)
		{
			while ((toLoad.size() == 0 && runThreads) || flag.test_and_set()) { std::this_thread::yield(); }
	
			if (i < toLoad.size() 
				&& toLoad[i] != nullptr
				&& toLoad[i]->inProcess.load() == false
				&& toLoad[i]->m_type.load() != ResourceType::SHADER)
			{
				toLoad[i]->inProcess.store(true);
				Resource resource{ toLoad[i]->m_name, toLoad[i]->m_type.load() };
				flag.clear();

				loadResource(resource.m_name, resource.m_type.load());

				for (int j = 0; j < toLoad.size(); j++)
				{
					if (toLoad[j]->m_type == resource.m_type && toLoad[j]->m_name == resource.m_name)
					{
						while (flag.test_and_set()) { std::this_thread::yield(); }

						if (j < toLoad.size())
							toLoad[j]->toDestroy.store(true);

						j = (int)(toLoad.size());
						flag.clear();
					}
				}
			}
			else
				flag.clear();
		}
	}
}
#pragma endregion

#pragma region RESOURCES CREATION/DESTRUCTION
// Resets the toLoad vector
void ResourcesManager::eraseQueue()
{
	while (flag.test_and_set()) { std::this_thread::yield(); }
	for (Resource* resource : toLoad)
		delete resource;

	toLoad.clear();
	flag.clear();
}

void ResourcesManager::cleanQueue()
{
	while (flag.test_and_set()) { std::this_thread::yield(); }

	for (int i = (int)toLoad.size() - 1; i >= 0; i--)
	{
		if (toLoad[i]->toDestroy)
		{
			delete toLoad[i];
			toLoad.erase(toLoad.begin() + i);
		}
	}
	flag.clear();
}

// Copy of searchResource, used in threads
void ResourcesManager::loadResource(const std::string filePath, const ResourceType type)
{
	switch (type)
	{
	case Resources::ResourceType::MESH:
		for (int i = 0; i < meshes.size(); i++)
			if (meshes[i]->m_name == filePath && meshes[i]->m_type == type)
				return;

		createResource(filePath, type);
		return;
	case Resources::ResourceType::SHADER:
		for (int i = 0; i < shaders.size(); i++)
			if (shaders[i]->m_name == filePath && shaders[i]->m_type == type)
				return;

		createResource(filePath, type);
		return;
	case Resources::ResourceType::TEXTURE:
		for (int i = 0; i < textures.size(); i++)
			if (textures[i]->m_name == filePath && textures[i]->m_type == type)
				return;

		createResource(filePath, type);
		return;
	default:
		return;
	}

}

// Generates a resource of a given name and a given type
void ResourcesManager::createResource(const std::string filePath, const ResourceType type)
{
	switch (type)
	{
	case Resources::ResourceType::MESH:
		meshes.push_back(std::make_unique<Mesh>(filePath, 1.f));
		break;
	case Resources::ResourceType::SHADER:
		shaders.push_back(std::make_unique<Shader>(filePath.c_str()));
		break;
	case Resources::ResourceType::TEXTURE:
		textures.push_back(std::make_unique<Texture>(filePath));
		break;
	default:
		break;
	}
}

void ResourcesManager::destroyResource(const int index, const ResourceType type)
{
	if (index < 0) return;
	
	switch (type)
	{
	case Resources::ResourceType::MESH:
		meshes.erase(meshes.begin() + index);
		break;
	case Resources::ResourceType::SHADER:
		shaders.erase(shaders.begin() + index);
		break;
	case Resources::ResourceType::TEXTURE:
		textures.erase(textures.begin() + index);
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region OGL USAGE
// Searches for a resource with a given name and a given type
int ResourcesManager::searchResource(const std::string filePath, const ResourceType type)
{
#if THREADING_TYPE == 0
	for (Resource* resource : toLoad)
	{
		if (resource->m_name == filePath && resource->m_type == type && !resource->toDestroy)
			return CUT_RENDERING_CODE;
	}
#elif THREADING_TYPE == 1
	for (Resource* resource : toLoad)
	{
		if (resource->m_name == filePath && resource->m_type == type)
		{
			createResource(filePath, type);

			for (int i = 0; i < toLoad.size(); i++)
				if (toLoad[i] == resource)
				{
					delete toLoad[i];
					toLoad.erase(toLoad.begin() + i);
					i = (int)toLoad.size();
				}

			switch (type)
			{
			case Resources::ResourceType::MESH:
				return (int)(meshes.size() - 1);
			case Resources::ResourceType::SHADER:
				return (int)(shaders.size() - 1);
			case Resources::ResourceType::TEXTURE:
				return (int)(textures.size() - 1);
			default:
				return CUT_RENDERING_CODE;
			}
		}
	}
#endif

	while (flag.test_and_set()) { std::this_thread::yield(); }

	switch (type)
	{
	case Resources::ResourceType::MESH:
		for (int i = 0; i < meshes.size(); i++)
			if (meshes[i]->m_name == filePath && meshes[i]->m_type == type)
			{
				flag.clear();
				return i;
			}

		break;
	case Resources::ResourceType::SHADER:
		for (int i = 0; i < shaders.size(); i++)
			if (shaders[i]->m_name == filePath && shaders[i]->m_type == type)
			{
				flag.clear();
				return i;
			}

		flag.clear();
		createResource(filePath, type);
		return (int)(shaders.size() - 1);
	case Resources::ResourceType::TEXTURE:
		for (int i = 0; i < textures.size(); i++)
			if (textures[i]->m_name == filePath && textures[i]->m_type == type)
			{
				flag.clear();
				return i;
			}

		break;
	default:
		return CUT_RENDERING_CODE;
	}

	toLoad.push_back(new Resource(filePath, type));
	flag.clear();

	return CUT_RENDERING_CODE;
}

void ResourcesManager::bindResource(int index, const ResourceType type, bool bindDefault)
{
	if (index < 0) return;

	switch (type)
	{
	case Resources::ResourceType::MESH:
		meshes[index]->bindResource();
		break;
	case Resources::ResourceType::SHADER:
		shaders[index]->bindResource();
		break;
	case Resources::ResourceType::TEXTURE:
		textures[index]->bindResource(bindDefault);
		break;
	default:
		break;
	}
}

void ResourcesManager::checkResourcesState(const ResourceType type)
{
	switch (type)
	{
	case Resources::ResourceType::MESH:
		for (std::shared_ptr<Mesh> mesh : meshes)
			if (!mesh->glReady) mesh->createVAOandVBO();

		break;
	case Resources::ResourceType::TEXTURE:
		for (std::shared_ptr<Texture> texture : textures)
			if (!texture->glReady) texture->generateOpenGl();
		
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region GETTERS
// Gets the first shader in the list of the resources
int ResourcesManager::getFirstShader()
{
	if (shaders.size() == 0) 
		return -1;

	return 0;
}

// Gets the locations of the shader's uniforms
Maths::int3 ResourcesManager::getUniforms()
{
	int shader = getFirstShader();

	if (shader == -1)
		return { -1, -1, -1 };

	int shaderProgram = shaders[0]->getData();

	GLint vpLocation = glGetUniformLocation(shaderProgram, "viewProj");
	GLint modelLocation = glGetUniformLocation(shaderProgram, "modelMat");
	GLint shaderDataLocation = glGetUniformBlockIndex(shaderProgram, "shaderData");

	return { vpLocation, modelLocation, shaderDataLocation };
}

int ResourcesManager::getShaderProgram(const std::string& name)
{
	int index = searchResource(name, ResourceType::SHADER);

	if (index < 0) 
		return 0;

	return shaders[index]->getData();
}

int ResourcesManager::getBufferSize(const std::string& name)
{
	int index = searchResource(name, ResourceType::MESH);
	
	if (index < 0) 
		return 0;

	return meshes[index]->bufferSize;
}
#pragma endregion