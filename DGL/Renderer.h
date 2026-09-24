#pragma once
#include <vector>
#include "Mesh.h"
#include "Shader.h"

struct RenderCommand
{
	Mesh* mesh;
	Shader* shader;
	glm::mat4 model;
};

class Renderer
{
public:
	
	void begin(float r, float g, float b);

	void submit(Mesh* mesh, Shader* shader, glm::mat4 model);

	void end();

private:
	std::vector<RenderCommand> commands;

};

