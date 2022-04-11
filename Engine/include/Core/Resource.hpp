#pragma once

#include <string>
#include <atomic>

#include "Maths.hpp"

namespace Resources
{
	enum class ResourceType
	{
		MESH,
		SHADER,
		TEXTURE,
		OTHER
	};

	struct Resource
	{
	public:
		Resource(const Resource& resource) 
		{
			m_name = resource.m_name;
			m_type.store(resource.m_type.load());
		};
		Resource(const std::string& name, const ResourceType type) { m_name = name; m_type.store(type); };
		~Resource() {};

		std::string m_name;
		std::atomic<ResourceType> m_type;
		std::atomic<bool> inProcess {false};
		std::atomic<bool> toDestroy {false};
	};
}