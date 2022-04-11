
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "Core/Resource.hpp"
#include "Core/Texture.hpp"

#include <mutex>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked
#define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

std::mutex lockT; // Used only for std::cout

using namespace Resources;

void Texture::generateOpenGl(GLint wrapping, GLint filtering)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);


	if (data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texValues.e[0], texValues.e[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glReady = true;
	}
	else
		std::cout << "[ERROR] Couldn't load texture: " << m_name << std::endl;

	stbi_image_free(data);
	dataFreed = true;
}

Texture::Texture(const std::string filePath)
{
	lockT.lock();
	std::cout << "Started parsing texture: " << filePath << std::endl;
	lockT.unlock();

	m_name = filePath;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(filePath.c_str(), &texValues.e[0], &texValues.e[1], &texValues.e[2], STBI_rgb);
	
	lockT.lock();
	std::cout << "Finished parsing texture: " << filePath << std::endl;
	lockT.unlock();
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
	if (dataFreed == false)
		stbi_image_free(data);
}

void Texture::bindResource(bool drawDefault)
{
	if (!glReady) generateOpenGl();

	if (drawDefault)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
		glBindTexture(GL_TEXTURE_2D, texture);
}