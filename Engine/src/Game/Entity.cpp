#include "Game/Entity.hpp"

Entity::Entity(std::string inTexName, std::string inMeshName, std::string inShaderName)
{
	model.meshName = inMeshName;
	model.textureName = inTexName;
	model.shaderName = inShaderName;
	life = maxLife;
}

Entity::Entity(const LowRenderer::Model& inModel)
{
	model = inModel;
	life = maxLife;
}

Entity::~Entity()
{
}