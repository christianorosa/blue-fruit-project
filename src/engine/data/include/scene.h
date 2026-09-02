#pragma once

#include <string>
#include <vector>

#include "../../maths/include/Vector3D.h"
#include "../../maths/include/Transform.h"

enum class SceneObjectType
{
    Line,
    Rectangle,
    Circle,
    Cube
};

struct SceneObject
{
    std::string name;

    SceneObjectType type;

    bfp::Transform transform;

    float width = 0.0f;
    float height = 0.0f;
    float radius = 0.0f;
};

struct Scene
{
    std::vector<SceneObject> objects;
};