#pragma once

#include "Core/Maths.hpp"

namespace LowRenderer
{
	struct Light
	{
		Maths::Vector4 position;
		Maths::Vector4 ambient;
		Maths::Vector4 diffuse;
		Maths::Vector4 specular;
		Maths::Vector4 attenuation;
		bool lightEnabled;
		char padding1[3];
		float padding2[3];
	};
}