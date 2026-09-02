#pragma once

#include "../../../engine/maths/include/NameSpace.h"
#include "../../../engine/maths/include/Defines.h"


DECLARE_ENGINE_NAMESPACE

class Vector3D
{
public:
    scalar x, y, z;

    // Constructores
    Vector3D();
    Vector3D(scalar X, scalar Y, scalar Z);
    Vector3D(const Vector3D& v);

    // Métodos de asignación y cálculo
    void Add(const Vector3D& v1, const Vector3D& v2);
    void Subtract(const Vector3D& v1, const Vector3D& v2);
    void Multiply(const Vector3D& v1, const Vector3D& v2);
    void Divide(const Vector3D& v1, const Vector3D& v2);

    void Add(const Vector3D& v1, float f);
    void Subtract(const Vector3D& v1, float f);
    void Multiply(const Vector3D& v1, float f);
    void Divide(const Vector3D& v1, float f);

    // Operadores de asignación compuesta
    void operator=(const Vector3D& v);
    void operator+=(const Vector3D& v);
    void operator-=(const Vector3D& v);
    void operator/=(const Vector3D& v);
    void operator*=(const Vector3D& v);

    // Operadores aritméticos entre vectores (Marcados como const)
    Vector3D operator+(const Vector3D& v2) const;
    Vector3D operator-(const Vector3D& v2) const;
    Vector3D operator/(const Vector3D& v2) const;
    Vector3D operator*(const Vector3D& v2) const;

    // Operadores aritméticos con escalares (Marcados como const)
    Vector3D operator+(float f) const;
    Vector3D operator-(float f) const;
    Vector3D operator/(float f) const;
    Vector3D operator*(float f) const;

    // Funciones matemáticas vectoriales
    void Negate();
    scalar Dot3(const Vector3D& v) const;        // Marcado como const
    scalar Magnitude() const;                    // Marcado como const
    void Normalize();

    // Optimizado: Paso por referencia constante para evitar duplicar memoria
    void Normalize(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3);

    Vector3D CrossProduct(const Vector3D& v) const; // Marcado como const
};

END_ENGINE_NAMESPACE
