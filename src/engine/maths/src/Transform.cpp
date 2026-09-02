#include "../include/Transform.h"

DECLARE_ENGINE_NAMESPACE


Transform::Transform()
{
    position = Vector3D(
        0.0f,
        0.0f,
        0.0f
    );

    rotation = Vector3D(
        0.0f,
        0.0f,
        0.0f
    );

    scale = Vector3D(
        1.0f,
        1.0f,
        1.0f
    );

    UpdateMatrix();
}


void Transform::UpdateMatrix()
{
    Matrix4x4 scaleMatrix;
    Matrix4x4 rotationMatrix;
    Matrix4x4 translationMatrix;

    scaleMatrix.Identity();
    rotationMatrix.Identity();
    translationMatrix.Identity();


    // ------------------------------------------------------------
    // Scale
    // ------------------------------------------------------------

    scaleMatrix.Scale(scale);


    // ------------------------------------------------------------
    // Rotation
    //
    // Transform.rotation utiliza grados.
    // Matrix4x4::SetRotationRadians() utiliza radianes.
    // ------------------------------------------------------------

    const double DEG_TO_RAD_VALUE =
        3.14159265358979323846 / 180.0;


    rotationMatrix.SetRotationRadians(
        static_cast<double>(rotation.x) * DEG_TO_RAD_VALUE,
        static_cast<double>(rotation.y) * DEG_TO_RAD_VALUE,
        static_cast<double>(rotation.z) * DEG_TO_RAD_VALUE
    );


    // ------------------------------------------------------------
    // Translation
    // ------------------------------------------------------------

    translationMatrix.Translate(position);


    // ------------------------------------------------------------
    // Model matrix
    //
    // Scale -> Rotation -> Translation
    // ------------------------------------------------------------

    m_matrix =
        translationMatrix *
        rotationMatrix *
        scaleMatrix;
}


const Matrix4x4& Transform::GetMatrix() const
{
    return m_matrix;
}


END_ENGINE_NAMESPACE