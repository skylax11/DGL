#pragma once
#include <vector>

class Mesh
{
public:
	Mesh(const	std::vector<float>& vertices);

	void draw();

	~Mesh();

private:
	unsigned int VAO, VBO;
	int vertexCount;

};



