#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>

enum AABBCompareAxis
{
	X,
	Y,
	Z
};

#pragma once
class PhysicObject
{
public:

	glm::vec3 position;

	PhysicObject(std::vector<float>& _vertices, glm::vec3 _position,float offSet);

	std::tuple<glm::vec3,glm::vec3> getAABB() const;

	bool collidesWith(const PhysicObject& neighbour) const;
	int getDirectionTo(const PhysicObject& neighbour, AABBCompareAxis axis) const;

	void initHalfExtents(std::vector<float>& _vertices);

private:

	glm::vec3 halfExtents;

	glm::vec3 getMaxPoints() const;
	glm::vec3 getMinPoints() const;

	float offSet;
};

