#pragma once

#include "Vector3D.h"
#include "Matrix.h"

DECLARE_ENGINE_NAMESPACE

class Transform
{
public:

    Transform();

    void UpdateMatrix();

    const Matrix4x4& GetMatrix() const;



    Vector3D position;
    Vector3D rotation;
    Vector3D scale;


    Matrix4x4 m_matrix;
};

END_ENGINE_NAMESPACE