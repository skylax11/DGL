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
	glm::vec3 rotation;
	glm::vec3 scale;

	bool isStatic;

	PhysicObject(std::vector<float>& _vertices, glm::vec3 _position,glm::vec3 _rotation,glm::vec3 _scale,float offSet,bool _isStatic);

	std::tuple<glm::vec3,glm::vec3> getAABB() const;

	bool collidesWith(const PhysicObject& neighbour) const;
	int getDirectionTo(const PhysicObject& neighbour, AABBCompareAxis axis) const;

	void initHalfExtents(std::vector<float>& _vertices);

	void addVelo(glm::vec3 velo);
	void resolveVelocity(const glm::vec3& pushDir);
	void integratePosition(float dt);

private:

	glm::vec3 velocity;

	glm::vec3 halfExtents;

	glm::vec3 getMaxPoints() const;
	glm::vec3 getMinPoints() const;

	float offSet;
};

