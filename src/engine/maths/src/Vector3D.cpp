#include "../include/Vector3D.h"
#include "../../../engine/maths/include/NameSpace.h"
#include <cmath>

DECLARE_ENGINE_NAMESPACE

// ==========================================
// CONSTRUCTORES
// ==========================================

Vector3D::Vector3D() : x(0), y(0), z(0) {}

Vector3D::Vector3D(scalar X, scalar Y, scalar Z) : x(X), y(Y), z(Z) {}

Vector3D::Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) {}

// ==========================================
// MÉTODOS DE ASIGNACIÓN
// ==========================================

void Vector3D::Add(const Vector3D& v1, const Vector3D& v2) { *this = v1 + v2; }
void Vector3D::Subtract(const Vector3D& v1, const Vector3D& v2) { *this = v1 - v2; }
void Vector3D::Multiply(const Vector3D& v1, const Vector3D& v2) { *this = v1 * v2; }
void Vector3D::Divide(const Vector3D& v1, const Vector3D& v2) { *this = v1 / v2; }

void Vector3D::Add(const Vector3D& v1, float f) { *this = v1 + f; }
void Vector3D::Subtract(const Vector3D& v1, float f) { *this = v1 - f; }
void Vector3D::Multiply(const Vector3D& v1, float f) { *this = v1 * f; }

void Vector3D::Divide(const Vector3D& v1, float f)
{
    float invF = (f != 0.0f) ? 1.0f / f : 1.0f;
    *this = v1 * invF;
}

// ==========================================
// OPERADORES DE ASIGNACIÓN COMPUESTA
// ==========================================

void Vector3D::operator=(const Vector3D& v) { x = v.x; y = v.y; z = v.z; }
void Vector3D::operator+=(const Vector3D& v) { x += v.x; y += v.y; z += v.z; }
void Vector3D::operator-=(const Vector3D& v) { x -= v.x; y -= v.y; z -= v.z; }
void Vector3D::operator/=(const Vector3D& v) { x /= v.x; y /= v.y; z /= v.z; }
void Vector3D::operator*=(const Vector3D& v) { x *= v.x; y *= v.y; z *= v.z; }

// ==========================================
// OPERADORES ARITMÉTICOS (ENTRE VECTORES)
// ==========================================

Vector3D Vector3D::operator+(const Vector3D& v2) const { return { x + v2.x, y + v2.y, z + v2.z }; }
Vector3D Vector3D::operator-(const Vector3D& v2) const { return { x - v2.x, y - v2.y, z - v2.z }; }
Vector3D Vector3D::operator/(const Vector3D& v2) const { return { x / v2.x, y / v2.y, z / v2.z }; }
Vector3D Vector3D::operator*(const Vector3D& v2) const { return { x * v2.x, y * v2.y, z * v2.z }; }

// ==========================================
// OPERADORES ARITMÉTICOS (CON ESCALARES)
// ==========================================

Vector3D Vector3D::operator+(float f) const { return { x + f, y + f, z + f }; }
Vector3D Vector3D::operator-(float f) const { return { x - f, y - f, z - f }; }
Vector3D Vector3D::operator/(float f) const { return { x / f, y / f, z / f }; }
Vector3D Vector3D::operator*(float f) const { return { x * f, y * f, z * f }; }

// ==========================================
// FUNCIONES MATEMÁTICAS VECTORIALES
// ==========================================

void Vector3D::Negate() { x = -x; y = -y; z = -z; }

scalar Vector3D::Dot3(const Vector3D& v) const { return x * v.x + y * v.y + z * v.z; }

scalar Vector3D::Magnitude() const { return std::sqrt(x * x + y * y + z * z); }

void Vector3D::Normalize()
{
    scalar len = Magnitude();
    scalar invLen = (len <= 0.00001) ? 1.0 : 1.0 / len;
    x *= invLen; y *= invLen; z *= invLen;
}

void Vector3D::Normalize(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3)
{
    Vector3D e1 = p1 - p2;
    Vector3D e2 = p2 - p3;
    e1.Normalize();
    e2.Normalize();
    *this = e1.CrossProduct(e2);
    Normalize();
}

Vector3D Vector3D::CrossProduct(const Vector3D& v) const
{
    return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
}

END_ENGINE_NAMESPACE
