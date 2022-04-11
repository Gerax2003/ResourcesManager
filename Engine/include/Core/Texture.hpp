#pragma once

#include <string>

#include "Resource.hpp"

typedef int GLint;

namespace Resources
{
	class Texture
	{
	public:
		Texture(const std::string filePath);
		~Texture();

		void bindResource(bool drawDefault = false);
		void generateOpenGl(GLint wrapping = GL_REPEAT, GLint filtering = GL_LINEAR);

		std::string m_name;
		ResourceType m_type = ResourceType::TEXTURE;
		bool glReady = false;
	private:
		bool dataFreed = false;
		Maths::int3 texValues{};
		unsigned int texture;
		unsigned char* data;
	};
}