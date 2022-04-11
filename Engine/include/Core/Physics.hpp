#pragma once

#include "Maths.hpp"

namespace Physics
{
	inline Maths::mat4x4 rotateX(float angleRadians)
	{
		return {
			1,                   0,                  0,                 0,
			0,                   cos(angleRadians),  sin(angleRadians), 0,
			0,                   -sin(angleRadians), cos(angleRadians), 0,
			0,                   0,                  0,                 1
		};
	}

	inline Maths::mat4x4 rotateY(float angleRadians)
	{
		return {
			cos(angleRadians), 0, -sin(angleRadians), 0,
			0, 1,  0, 0,
			sin(angleRadians), 0, cos(angleRadians), 0,
			0, 0, 0, 1
		};
	}

	inline Maths::mat4x4 rotateZ(float angleRadians)
	{
		return {
			cos(angleRadians),  sin(angleRadians),   0,                 0,
			-sin(angleRadians), cos(angleRadians),   0,					0,
			0,                   0,					 1,					0,
			0,                   0,                  0,                 1
		};
	}

	inline Maths::mat4x4 scale(float scale)
	{
		if (scale == 0)
			scale = 0.0001f;

		return {
			scale, 0.f, 0.f, 0.f,
			0.f, scale, 0.f, 0.f,
			0.f, 0.f, scale, 0.f,
			0.0f, 0.0f, 0.f, 1.f,
		};
	}

	inline Maths::mat4x4 scale(Maths::Vector3 scale)
	{
		if (Maths::lengthVector3(scale) == 0)
			scale.x = 0.0001f;

		return {
			scale.x, 0.f,     0.f,     0.f,
			0.f,     scale.y, 0.f,     0.f,
			0.f,     0.f,     scale.z, 0.f,
			0.0f,    0.0f,    0.f,     1.f,
		};
	}

	inline Maths::mat4x4 rotate3f(Maths::Vector3 rotation)
	{
		return rotateY(rotation.y) * rotateX(rotation.x) * rotateZ(rotation.z);
	}

	inline Maths::mat4x4 translate(Vector3 translate)
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translate.x, translate.y, translate.z, 1
		};
	}
}