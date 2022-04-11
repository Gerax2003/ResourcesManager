#pragma once

#include <string>
#include <vector>

#include "Core/Maths.hpp"
#include "Core/Resource.hpp"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked
#define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW


namespace Resources
{
	union Face3
	{
		Face3() { v1 = {}; v2 = {}; v3 = {}; };
		Maths::int3 e[3];
		struct { Maths::int3 v1; Maths::int3 v2; Maths::int3 v3; };
	};

	class Mesh
	{
	public:
		void objParser(std::string filePath, float scale);
		void createVAOandVBO();

		Mesh() {};
		Mesh(std::string filePath, float scale);
		~Mesh();

		void bindResource();
		int bufferSize = 0;


		std::string m_name = "blank.obj";
		ResourceType m_type = ResourceType::MESH;
		bool glReady = false;
	private:
		std::vector<float> buffer;

		unsigned int VAO = 0;
		unsigned int VBO = 0;
	};
}
