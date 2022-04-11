
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <fstream>

#include "Core/Resource.hpp"
#include "Core/Mesh.hpp"
#include "Core/Maths.hpp"

#include <mutex>

std::mutex lock; // Used only for std::cout

using namespace Resources;

#pragma region LINE PARSING
Maths::Vector3 parseLine3f(const char* line)
{
	int blankCount = 0;
	int readBegin = 0;
	int readEnd = 0;
	Maths::Vector3 retData = {};

	for (int i = 0; i < 128; i++)
	{
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0')
		{
			if (blankCount != 0)
			{
				readEnd = i;
				int len = readEnd - readBegin;
				char read[32];

				if (len > 30)
					len = 30;

				for (int j = 0; j < len; j++)
					read[j] = line[readBegin + j];

				read[len] = '\0';
				try
				{
					retData.e[blankCount - 1] = std::stof(read);
				}
				catch (const std::exception& e)
				{
					std::cout << "Couldn't read number: " << e.what() << std::endl;
				}
			}

			readBegin = i + 1;
			blankCount++;
		}

		if (blankCount >= 4 || line[i] == '\0' || line[i] == '\n')
			return retData;
	}
	return retData;
}

Maths::Vector2 parseLine2f(const char* line)
{
	int blankCount = 0;
	int readBegin = 0;
	int readEnd = 0;
	Maths::Vector2 retData = {};

	for (int i = 0; i < 128; i++)
	{
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0')
		{
			if (blankCount != 0)
			{
				readEnd = i;
				int len = readEnd - readBegin;
				char read[32];

				if (len > 30)
					len = 30;

				for (int j = 0; j < len; j++)
					read[j] = line[readBegin + j];

				read[len] = '\0';
				try
				{
					retData.e[blankCount - 1] = std::stof(read);
				}
				catch (const std::exception& e)
				{
					std::cout << "Couldn't read number: " << e.what() << std::endl;
				}
			}

			readBegin = i + 1;
			blankCount++;
		}

		if (blankCount >= 3 || line[i] == '\0' || line[i] == '\n')
			return retData;
	}
	return retData;
}

Maths::int3 parseReadFace3(const char* read)
{
	int slashCount = 0;
	int readBegin = 0;
	int readEnd = 0;
	Maths::int3 retData = {};

	for (int i = 0; i < 32; i++)
	{
		if (read[i] == '/' || read[i] == '\n' || read[i] == '\0')
		{
			readEnd = i;
			int len = readEnd - readBegin;
			char num[16];

			if (len > 14)
				len = 14;

			for (int j = 0; j < len; j++)
				num[j] = read[readBegin + j];

			num[len] = '\0';

			if (num[0] == '/' || read[0] == '\n' || read[0] == '\0')
				retData.e[slashCount] = 0;
			else
				try
				{
					retData.e[slashCount] = std::stoi(num);
				}
				catch (const std::exception&)
				{
					//std::cout << "couldn't read index, setting value to 0" << std::endl;
					retData.e[slashCount] = 0;
				}

			readBegin = i + 1;
			slashCount++;
		}

		if (slashCount >= 5 || read[i] == '\0' || read[i] == '\n')
			return retData;
	}
	return retData;
}

Face3 parseLineFace3(const char* line)
{
	int blankCount = 0;
	int readBegin = 0;
	int readEnd = 0;
	Face3 retData = {};

	for (int i = 0; i < 128; i++)
	{
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0')
		{
			if (blankCount != 0)
			{
				readEnd = i;
				int len = readEnd - readBegin;
				char read[32];

				if (len > 30)
					len = 30;


				for (int j = 0; j < len; j++)
					read[j] = line[readBegin + j];

				read[len] = '\0';
				retData.e[blankCount - 1] = parseReadFace3(read);
			}

			readBegin = i + 1;
			blankCount++;
		}

		if (blankCount >= 4 || line[i] == '\0' || line[i] == '\n')
			return retData;
	}
	return retData;
}
#pragma endregion

#pragma region POST PARSING OPERATIONS
void fillBuffer(std::vector<float>& buffer, std::vector<Maths::Vector3>& position, std::vector<Maths::Vector2>& uvCoords, std::vector<Maths::Vector3>& normals, std::vector<Face3>& faces)
{
	for (int i = 0; i < faces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int posIndex = faces[i].e[j].v - 1;
			int texIndex = faces[i].e[j].t - 1;
			int normalIndex = faces[i].e[j].n - 1;

			if (posIndex < 0) posIndex = 0;
			if (texIndex < 0) texIndex = 0;
			if (normalIndex < 0) normalIndex = 0;

			// pushing back individually because insert makes the vector "too long"
			buffer.push_back(position[posIndex].e[0]);
			buffer.push_back(position[posIndex].e[1]);
			buffer.push_back(position[posIndex].e[2]);

			buffer.push_back(uvCoords[texIndex].e[0]);
			buffer.push_back(uvCoords[texIndex].e[1]);

			buffer.push_back(normals[normalIndex].e[0]);
			buffer.push_back(normals[normalIndex].e[1]);
			buffer.push_back(normals[normalIndex].e[2]);
		}
	}
}

void scalePositions(std::vector<Maths::Vector3>& positions, float scale)
{
	for (int i = 0; i < positions.size(); i++)
		positions[i] = positions[i] * scale;
}

//unsigned int& VAO, unsigned int& VBO, std::vector<float>& buffer
void Mesh::createVAOandVBO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	buffer.clear();
	glReady = true;
}
#pragma endregion

void Mesh::objParser(std::string filePath, float scale)
{
	lock.lock();
	std::cout << "Starting parsing mesh: " << filePath << std::endl;
	lock.unlock();

	std::vector<Maths::Vector3> positions;
	std::vector<Maths::Vector2> uvCoords;
	std::vector<Maths::Vector3> normals;
	std::vector<Face3> faces;

	std::fstream file;
	file.open(filePath);

	if (!file.is_open())
	{
		std::cout << "[ERROR] Couldn't open object: " << filePath << std::endl;
		return;
	}

	while (!file.eof())
	{
		char line[128];
		file.getline(line, 128);

		if (line[0] == 'v')
		{
			if (line[1] == ' ')
			{
				Maths::Vector3 pos = parseLine3f(line);
				positions.push_back(pos);
			}
			else if (line[1] == 'n')
				normals.push_back(parseLine3f(line));
			else if (line[1] == 't')
				uvCoords.push_back(parseLine2f(line));
		}
		else if (line[0] == 'f')
			faces.push_back(parseLineFace3(line));
	}
	file.close();

	scalePositions(positions, scale);

	fillBuffer(buffer, positions, uvCoords, normals, faces);
	//createVAOandVBO(VAO, VBO, buffer);

	bufferSize = (int)(buffer.size());
}

Mesh::Mesh(std::string filePath, float scale)
{
	objParser(filePath, scale);
	m_name = filePath;

	lock.lock();
	std::cout << "Finished parsing mesh: " << filePath << std::endl;
	lock.unlock();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::bindResource()
{
	if (!glReady) createVAOandVBO();

	glBindVertexArray(VAO);
}
