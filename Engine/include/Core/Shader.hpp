#pragma once

#include <string>
#include "Resource.hpp"

namespace Resources
{
	class Shader
	{
	public:
		Shader(const char* filePath);
		~Shader();

		void bindResource();
		int getData();

		std::string m_name;
		ResourceType m_type = ResourceType::SHADER;
		bool glReady = false;
	private:
		int shaderProgram = 0;
	};
}