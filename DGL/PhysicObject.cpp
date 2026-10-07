#include "PhysicObject.h"

PhysicObject::PhysicObject(std::vector<float>& _vertices, 
    glm::vec3 _position,glm::vec3 _rotation,glm::vec3 _scale,
    float _offSet,bool _isStatic) 
    : position (_position),rotation (_rotation), scale(_scale), offSet(_offSet), isStatic(_isStatic), velocity(glm::vec3(0, 0, 0))
{
    initHalfExtents(_vertices);
}

std::tuple<glm::vec3, glm::vec3> PhysicObject::getAABB() const
{
    return {getMinPoints(),getMaxPoints()};
}
bool PhysicObject::collidesWith(const PhysicObject& neighbour) const
{
    glm::vec3 minPoint = getMinPoints();
    glm::vec3 maxPoint = getMaxPoints();

    glm::vec3 neighbourMinPoint = neighbour.getMinPoints();
    glm::vec3 neighbourMaxPoint = neighbour.getMaxPoints();

    bool x = minPoint.x <= neighbourMaxPoint.x && maxPoint.x >= neighbourMinPoint.x;
    bool y = minPoint.y <= neighbourMaxPoint.y && maxPoint.y >= neighbourMinPoint.y;
    bool z = minPoint.z <= neighbourMaxPoint.z && maxPoint.z >= neighbourMinPoint.z;

    return x && y && z;
}

int PhysicObject::getDirectionTo(const PhysicObject& neighbour, AABBCompareAxis axis) const
{
    float centerA, centerB;

    glm::vec3 minPoint = getMinPoints();
    glm::vec3 maxPoint = getMaxPoints();

    glm::vec3 neighbourMinPoint = neighbour.getMinPoints();
    glm::vec3 neighbourMaxPoint = neighbour.getMaxPoints();

    if (axis == AABBCompareAxis::X) {
        centerA = (minPoint.x + maxPoint.x) / 2;
        centerB = (neighbourMinPoint.x + neighbourMaxPoint.x) / 2;
    }
    else if (axis == AABBCompareAxis::Y) {
        centerA = (minPoint.y + maxPoint.y) / 2;
        centerB = (neighbourMinPoint.y + neighbourMaxPoint.y) / 2;
    }
    else {
        centerA = (minPoint.z + maxPoint.z) / 2;
        centerB = (neighbourMinPoint.z + neighbourMaxPoint.z) / 2;
    }
    return (centerA < centerB) ? -1 : 1;
}


void PhysicObject::initHalfExtents(std::vector<float>& vertices)
{
    glm::vec3 minPoint = glm::vec3(FLT_MAX);
    glm::vec3 maxPoint = glm::vec3(-FLT_MAX);

    for (int j = 0; j < vertices.size(); j += 6)
    {
        float x = vertices[j];
        float y = vertices[j + 1];
        float z = vertices[j + 2];

        minPoint.x = std::min(minPoint.x, x);
        minPoint.y = std::min(minPoint.y, y);
        minPoint.z = std::min(minPoint.z, z);

        maxPoint.x = std::max(maxPoint.x, x);
        maxPoint.y = std::max(maxPoint.y, y);
        maxPoint.z = std::max(maxPoint.z, z);
    }

    halfExtents = (maxPoint - minPoint) * 0.5f;
    halfExtents *= scale;
}

void PhysicObject::addVelo(glm::vec3 velo)
{
    velocity += velo;
}

void PhysicObject::resolveVelocity(const glm::vec3& pushDir)
{
    if (isStatic)
    {
        return;
    }

    for (int k = 0; k < 3; k++)
    {
        bool pushedPositive_movingNegative = pushDir[k] > 0.0f && velocity[k] < 0.0f;
        bool pushedNegative_movingPositive = pushDir[k] < 0.0f && velocity[k] > 0.0f;

        if (pushedPositive_movingNegative || pushedNegative_movingPositive)
        {
            velocity[k] = 0.0f;
        }
    }
}

void PhysicObject::integratePosition(float dt)
{
    if (isStatic)
    {
        return;
    }

    position += velocity * dt;
}

glm::vec3 PhysicObject::getMaxPoints() const
{
    return position + halfExtents + offSet;
}


glm::vec3 PhysicObject::getMinPoints() const
{
    return position - halfExtents - offSet;
}
