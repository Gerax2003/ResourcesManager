
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Core/Resource.hpp"
#include "Core/Shader.hpp"
#include "Core/ResourcesManager.hpp"

#include <mutex>

std::mutex lockS; // Used only for std::cout

using namespace Resources;

unsigned int loadShader(const char* filePath)
{
	lockS.lock();
	std::cout << "Started parsing shader: " << filePath << std::endl;
	lockS.unlock();

	std::string fileFrag = filePath;
	fileFrag += ".frag";
	std::string fileVert = filePath;
	fileVert += ".vert";
	std::stringstream fragBuffer;
	std::stringstream vertBuffer;
	std::fstream file1;
	std::fstream file2;

	file1.open(fileFrag);
	fragBuffer << file1.rdbuf();
	std::string str = fragBuffer.str();
	const char* fragmentShaderSource = str.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	file2.open(fileVert);
	vertBuffer << file2.rdbuf();
	str = vertBuffer.str();
	const char* vertexShaderSource = str.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: VERTEX SHADER COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: FRAGMENT SHADER COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	file1.close();
	file2.close();

	return shaderProgram;
}

Shader::Shader(const char* filePath)
{
	shaderProgram = loadShader(filePath);
	glReady = true;
	m_name = filePath;

	lockS.lock();
	std::cout << "Finished parsing shader: " << filePath << std::endl;
	lockS.unlock();
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::bindResource()
{
	glUseProgram(shaderProgram);
}

int Shader::getData()
{
	return shaderProgram;
}