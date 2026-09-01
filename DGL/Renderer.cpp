#include "Renderer.h"
#include <glad/glad.h>
#include <glad/glad.h>

void Renderer :: begin(float r, float g, float b)
{
	commands.clear();

	glClearColor(r,g,b,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::submit(Mesh* mesh, Shader* shader)
{
	RenderCommand command;
	command.mesh = mesh;
	command.shader = shader;

	commands.push_back(command);
}

void Renderer::end() 
{

	for (const RenderCommand& command : commands) {
		command.shader->use();
		command.mesh->draw();
	}
}