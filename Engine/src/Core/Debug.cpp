#include <iostream>
#include "Core/Mesh.hpp"
#include "Core/Debug.hpp"
#include "Core/ResourcesManager.hpp"
#include "Core/Maths.hpp"

#pragma region PARSING DEBUG
void Debug::printVectors(const std::vector<Maths::Vector3>& pos, const std::vector<Maths::Vector2>& uvc, const std::vector<Maths::Vector3>& norm, const std::vector<Resources::Face3>& faces)
{
	std::cout << "====PRINTING VECTORS (Ressources manager)====" << std::endl;

	for (int i = 0; i < pos.size(); i += 3)
		std::cout << "v " << pos[i].x << ' ' << pos[i].y << ' ' << pos[i].z << std::endl;

	std::cout << std::endl;

	for (int i = 0; i < uvc.size(); i++)
		std::cout << "vt " << uvc[i].x << ' ' << uvc[i].y << std::endl;

	std::cout << std::endl;

	for (int i = 0; i < norm.size(); i++)
		std::cout << "vn " << norm[i].x << ' ' << norm[i].y << ' ' << norm[i].z << std::endl;

	std::cout << std::endl;

	for (int i = 0; i < faces.size(); i++)
		std::cout << "f " << faces[i].v1.v << '/' << faces[i].v1.t << '/' << faces[i].v1.n << ' ' << faces[i].v2.v << '/' << faces[i].v2.t << '/' << faces[i].v2.n << ' ' << faces[i].v3.v << '/' << faces[i].v3.t << '/' << faces[i].v3.n << std::endl;

	std::cout << "=============================================" << std::endl;
}

void Debug::printVertexBufferRaw(const std::vector<float>& buffer)
{
	std::cout << "=====PRINTING VECTOR (Model.buffer, raw)=====" << std::endl;

	for (int i = 0; i < buffer.size(); i++)
		std::cout << "Entry " << i << " :" << buffer[i] << std::endl;

	std::cout << "=============================================" << std::endl;
}

void Debug::printVertexBufferParse(const std::vector<float>& buffer)
{
	std::cout << "====PRINTING VECTOR (Model.buffer, parsed)===" << std::endl;

	for (int i = 0; i < buffer.size(); i += 8)
	{
		std::cout << "Pos " << i / 8 << " :" << buffer[i + 0] << ' ' << buffer[i + 1] << ' ' << buffer[i + 2] << std::endl;

		std::cout << "TexCoord " << i / 8 << " :" << buffer[i + 3] << ' ' << buffer[i + 4] << std::endl;

		std::cout << "Normal " << i / 8 << " :" << buffer[i + 5] << ' ' << buffer[i + 6] << ' ' << buffer[i + 7] << std::endl;
	}

	std::cout << "=============================================" << std::endl;
}
#pragma endregion
