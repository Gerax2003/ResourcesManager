#include "Game/Platform.hpp"
#include "Core/Maths.hpp"
#include <string>

// The collider's scale 
Platform::Platform(Maths::Vector3 position, Maths::Vector3 rotation, Maths::Vector3 scale) : model(position, rotation, scale, "resources/blank.png", "resources/cube.obj", "resources/shader")
{
	model.position = position;
	collider = { scale * 2, position, rotation };
}
Platform::Platform(LowRenderer::Model inModel)
{
	model = inModel;
	collider = { model.scale * 2, model.position, model.rotation };
}
Platform::Platform(std::string textureName, std::string shaderName, Maths::Vector3 position, Maths::Vector3 rotation, Maths::Vector3 scale) : model(position, rotation, scale, textureName, "resources/box.obj", shaderName)
{
	collider = { scale * 2, position, rotation };
}