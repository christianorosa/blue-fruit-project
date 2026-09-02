#include "../include/Matrix.h"
#include "../include/Vector3D.h"
#include <math.h>

DECLARE_ENGINE_NAMESPACE


Matrix4x4::Matrix4x4()
{
    Identity();
}


Matrix4x4::Matrix4x4(float r11, float r12, float r13, float r14,
    float r21, float r22, float r23, float r24,
    float r31, float r32, float r33, float r34,
    float r41, float r42, float r43, float r44)
{
    matrix[0] = r11;
    matrix[1] = r12;
    matrix[2] = r13;
    matrix[3] = r14;

    matrix[4] = r21;
    matrix[5] = r22;
    matrix[6] = r23;
    matrix[7] = r24;

    matrix[8] = r31;
    matrix[9] = r32;
    matrix[10] = r33;
    matrix[11] = r34;

    matrix[12] = r41;
    matrix[13] = r42;
    matrix[14] = r43;
    matrix[15] = r44;
}


void Matrix4x4::Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
    for (int i = 0; i < 16; ++i)
        matrix[i] = m1.matrix[i] + m2.matrix[i];
}


void Matrix4x4::Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
    for (int i = 0; i < 16; ++i)
        matrix[i] = m1.matrix[i] - m2.matrix[i];
}


void Matrix4x4::Multiple(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
    const float* m1 = mat1.matrix;
    const float* m2 = mat2.matrix;

    matrix[0] =
        m1[0] * m2[0] +
        m1[4] * m2[1] +
        m1[8] * m2[2] +
        m1[12] * m2[3];

    matrix[1] =
        m1[1] * m2[0] +
        m1[5] * m2[1] +
        m1[9] * m2[2] +
        m1[13] * m2[3];

    matrix[2] =
        m1[2] * m2[0] +
        m1[6] * m2[1] +
        m1[10] * m2[2] +
        m1[14] * m2[3];

    matrix[3] =
        m1[3] * m2[0] +
        m1[7] * m2[1] +
        m1[11] * m2[2] +
        m1[15] * m2[3];


    matrix[4] =
        m1[0] * m2[4] +
        m1[4] * m2[5] +
        m1[8] * m2[6] +
        m1[12] * m2[7];

    matrix[5] =
        m1[1] * m2[4] +
        m1[5] * m2[5] +
        m1[9] * m2[6] +
        m1[13] * m2[7];

    matrix[6] =
        m1[2] * m2[4] +
        m1[6] * m2[5] +
        m1[10] * m2[6] +
        m1[14] * m2[7];

    matrix[7] =
        m1[3] * m2[4] +
        m1[7] * m2[5] +
        m1[11] * m2[6] +
        m1[15] * m2[7];


    matrix[8] =
        m1[0] * m2[8] +
        m1[4] * m2[9] +
        m1[8] * m2[10] +
        m1[12] * m2[11];

    matrix[9] =
        m1[1] * m2[8] +
        m1[5] * m2[9] +
        m1[9] * m2[10] +
        m1[13] * m2[11];

    matrix[10] =
        m1[2] * m2[8] +
        m1[6] * m2[9] +
        m1[10] * m2[10] +
        m1[14] * m2[11];

    matrix[11] =
        m1[3] * m2[8] +
        m1[7] * m2[9] +
        m1[11] * m2[10] +
        m1[15] * m2[11];


    matrix[12] =
        m1[0] * m2[12] +
        m1[4] * m2[13] +
        m1[8] * m2[14] +
        m1[12] * m2[15];

    matrix[13] =
        m1[1] * m2[12] +
        m1[5] * m2[13] +
        m1[9] * m2[14] +
        m1[13] * m2[15];

    matrix[14] =
        m1[2] * m2[12] +
        m1[6] * m2[13] +
        m1[10] * m2[14] +
        m1[14] * m2[15];

    matrix[15] =
        m1[3] * m2[12] +
        m1[7] * m2[13] +
        m1[11] * m2[14] +
        m1[15] * m2[15];
}


void Matrix4x4::operator=(const Matrix4x4& m)
{
    matrix[0] = m.matrix[0];
    matrix[1] = m.matrix[1];
    matrix[2] = m.matrix[2];
    matrix[3] = m.matrix[3];

    matrix[4] = m.matrix[4];
    matrix[5] = m.matrix[5];
    matrix[6] = m.matrix[6];
    matrix[7] = m.matrix[7];

    matrix[8] = m.matrix[8];
    matrix[9] = m.matrix[9];
    matrix[10] = m.matrix[10];
    matrix[11] = m.matrix[11];

    matrix[12] = m.matrix[12];
    matrix[13] = m.matrix[13];
    matrix[14] = m.matrix[14];
    matrix[15] = m.matrix[15];
}


Matrix4x4 Matrix4x4::operator-(const Matrix4x4& m)
{
    Matrix4x4 result;

    result.Subtract(*this, m);

    return result;
}


Matrix4x4 Matrix4x4::operator+(const Matrix4x4& m)
{
    Matrix4x4 result;

    result.Add(*this, m);

    return result;
}


Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m)
{
    Matrix4x4 result;

    result.Multiple(*this, m);

    return result;
}


void Matrix4x4::Identity()
{
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}


void Matrix4x4::Zero()
{
    for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;
}


void Matrix4x4::Translate(const Vector3D& v)
{
    matrix[12] = v.x;
    matrix[13] = v.y;
    matrix[14] = v.z;
    matrix[15] = 1.0f;
}


void Matrix4x4::Translate(float x, float y, float z)
{
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
    matrix[15] = 1.0f;
}


Vector3D Matrix4x4::inverseTranslateVector(const Vector3D& v)
{
    return Vector3D(
        v.x - matrix[12],
        v.y - matrix[13],
        v.z - matrix[14]
    );
}


bool Matrix4x4::inverseMatrix(const Matrix4x4& m)
{
    const float* a = m.matrix;

    float inv[16];

    inv[0] =
        a[5] * a[10] * a[15] -
        a[5] * a[11] * a[14] -
        a[9] * a[6] * a[15] +
        a[9] * a[7] * a[14] +
        a[13] * a[6] * a[11] -
        a[13] * a[7] * a[10];

    inv[4] =
        -a[4] * a[10] * a[15] +
        a[4] * a[11] * a[14] +
        a[8] * a[6] * a[15] -
        a[8] * a[7] * a[14] -
        a[12] * a[6] * a[11] +
        a[12] * a[7] * a[10];

    inv[8] =
        a[4] * a[9] * a[15] -
        a[4] * a[11] * a[13] -
        a[8] * a[5] * a[15] +
        a[8] * a[7] * a[13] +
        a[12] * a[5] * a[11] -
        a[12] * a[7] * a[9];

    inv[12] =
        -a[4] * a[9] * a[14] +
        a[4] * a[10] * a[13] +
        a[8] * a[5] * a[14] -
        a[8] * a[6] * a[13] -
        a[12] * a[5] * a[10] +
        a[12] * a[6] * a[9];

    inv[1] =
        -a[1] * a[10] * a[15] +
        a[1] * a[11] * a[14] +
        a[9] * a[2] * a[15] -
        a[9] * a[3] * a[14] -
        a[13] * a[2] * a[11] +
        a[13] * a[3] * a[10];

    inv[5] =
        a[0] * a[10] * a[15] -
        a[0] * a[11] * a[14] -
        a[8] * a[2] * a[15] +
        a[8] * a[3] * a[14] +
        a[12] * a[2] * a[11] -
        a[12] * a[3] * a[10];

    inv[9] =
        -a[0] * a[9] * a[15] +
        a[0] * a[11] * a[13] +
        a[8] * a[1] * a[15] -
        a[8] * a[3] * a[13] -
        a[12] * a[1] * a[11] +
        a[12] * a[3] * a[9];

    inv[13] =
        a[0] * a[9] * a[14] -
        a[0] * a[10] * a[13] -
        a[8] * a[1] * a[14] +
        a[8] * a[2] * a[13] +
        a[12] * a[1] * a[10] -
        a[12] * a[2] * a[9];

    inv[2] =
        a[1] * a[6] * a[15] -
        a[1] * a[7] * a[14] -
        a[5] * a[2] * a[15] +
        a[5] * a[3] * a[14] +
        a[13] * a[2] * a[7] -
        a[13] * a[3] * a[6];

    inv[6] =
        -a[0] * a[6] * a[15] +
        a[0] * a[7] * a[14] +
        a[4] * a[2] * a[15] -
        a[4] * a[3] * a[14] -
        a[12] * a[2] * a[7] +
        a[12] * a[3] * a[6];

    inv[10] =
        a[0] * a[5] * a[15] -
        a[0] * a[7] * a[13] -
        a[4] * a[1] * a[15] +
        a[4] * a[3] * a[13] +
        a[12] * a[1] * a[7] -
        a[12] * a[3] * a[5];

    inv[14] =
        -a[0] * a[5] * a[14] +
        a[0] * a[6] * a[13] +
        a[4] * a[1] * a[14] -
        a[4] * a[2] * a[13] -
        a[12] * a[1] * a[6] +
        a[12] * a[2] * a[5];

    inv[3] =
        -a[1] * a[6] * a[11] +
        a[1] * a[7] * a[10] +
        a[5] * a[2] * a[11] -
        a[5] * a[3] * a[10] -
        a[9] * a[2] * a[7] +
        a[9] * a[3] * a[6];

    inv[7] =
        a[0] * a[6] * a[11] -
        a[0] * a[7] * a[10] -
        a[4] * a[2] * a[11] +
        a[4] * a[3] * a[10] +
        a[8] * a[2] * a[7] -
        a[8] * a[3] * a[6];

    inv[11] =
        -a[0] * a[5] * a[11] +
        a[0] * a[7] * a[9] +
        a[4] * a[1] * a[11] -
        a[4] * a[3] * a[9] -
        a[8] * a[1] * a[7] +
        a[8] * a[3] * a[5];

    inv[15] =
        a[0] * a[5] * a[10] -
        a[0] * a[6] * a[9] -
        a[4] * a[1] * a[10] +
        a[4] * a[2] * a[9] +
        a[8] * a[1] * a[6] -
        a[8] * a[2] * a[5];

    float determinant =
        a[0] * inv[0] +
        a[1] * inv[4] +
        a[2] * inv[8] +
        a[3] * inv[12];

    if (determinant == 0.0f)
    {
        Identity();
        return false;
    }

    determinant = 1.0f / determinant;

    for (int i = 0; i < 16; ++i)
        matrix[i] = inv[i] * determinant;

    return true;
}


void Matrix4x4::invertMatrix(const Matrix4x4& m)
{
    Transpose(m);

    matrix[3] = 0.0f;
    matrix[7] = 0.0f;
    matrix[11] = 0.0f;
    matrix[15] = 1.0f;

    matrix[12] =
        -(m.matrix[12] * m.matrix[0]) -
        (m.matrix[13] * m.matrix[1]) -
        (m.matrix[14] * m.matrix[2]);

    matrix[13] =
        -(m.matrix[12] * m.matrix[4]) -
        (m.matrix[13] * m.matrix[5]) -
        (m.matrix[14] * m.matrix[6]);

    matrix[14] =
        -(m.matrix[12] * m.matrix[8]) -
        (m.matrix[13] * m.matrix[9]) -
        (m.matrix[14] * m.matrix[10]);
}


Vector3D Matrix4x4::VectorMatrixMultiply(const Vector3D& v) const
{
    Vector3D out;

    out.x =
        (v.x * matrix[0]) +
        (v.y * matrix[4]) +
        (v.z * matrix[8]) +
        matrix[12];

    out.y =
        (v.x * matrix[1]) +
        (v.y * matrix[5]) +
        (v.z * matrix[9]) +
        matrix[13];

    out.z =
        (v.x * matrix[2]) +
        (v.y * matrix[6]) +
        (v.z * matrix[10]) +
        matrix[14];

    return out;
}


Vector3D Matrix4x4::VectorMatrixMultiply3x3(const Vector3D& v) const
{
    Vector3D out;

    out.x =
        (v.x * matrix[0]) +
        (v.y * matrix[4]) +
        (v.z * matrix[8]);

    out.y =
        (v.x * matrix[1]) +
        (v.y * matrix[5]) +
        (v.z * matrix[9]);

    out.z =
        (v.x * matrix[2]) +
        (v.y * matrix[6]) +
        (v.z * matrix[10]);

    return out;
}


Vector3D Matrix4x4::VectorMatrixMultiply3x3Inv(const Vector3D& v) const
{
    Vector3D out;

    out.x =
        (v.x * matrix[0]) +
        (v.y * matrix[1]) +
        (v.z * matrix[2]);

    out.y =
        (v.x * matrix[4]) +
        (v.y * matrix[5]) +
        (v.z * matrix[6]);

    out.z =
        (v.x * matrix[8]) +
        (v.y * matrix[9]) +
        (v.z * matrix[10]);

    return out;
}


void Matrix4x4::Transpose(const Matrix4x4& m)
{
    matrix[0] = m.matrix[0];
    matrix[1] = m.matrix[4];
    matrix[2] = m.matrix[8];

    matrix[4] = m.matrix[1];
    matrix[5] = m.matrix[5];
    matrix[6] = m.matrix[9];

    matrix[8] = m.matrix[2];
    matrix[9] = m.matrix[6];
    matrix[10] = m.matrix[10];

    matrix[3] = m.matrix[12];
    matrix[7] = m.matrix[13];
    matrix[11] = m.matrix[14];

    matrix[12] = m.matrix[3];
    matrix[13] = m.matrix[7];
    matrix[14] = m.matrix[11];

    matrix[15] = m.matrix[15];
}


void Matrix4x4::Scale(const Vector3D& scale)
{
    matrix[0] = scale.x;
    matrix[5] = scale.y;
    matrix[10] = scale.z;
}


void Matrix4x4::SetRotationRadians(double x, double y, double z)
{
    double cosX = cos(x);
    double cosY = cos(y);
    double cosZ = cos(z);

    double sinX = sin(x);
    double sinY = sin(y);
    double sinZ = sin(z);

    matrix[0] = (float)(cosY * cosZ);
    matrix[1] = (float)(cosY * sinZ);
    matrix[2] = (float)-sinY;

    matrix[4] =
        (float)((sinX * sinY) * cosZ - cosX * sinZ);

    matrix[5] =
        (float)((sinX * sinY) * sinZ + cosX * cosZ);

    matrix[6] =
        (float)(sinX * cosY);

    matrix[8] =
        (float)((cosX * sinY) * cosZ + sinX * sinZ);

    matrix[9] =
        (float)((cosX * sinY) * sinZ - sinX * cosZ);

    matrix[10] =
        (float)(cosX * cosY);
}


void Matrix4x4::Rotate(float angle, int x, int y, int z)
{
    angle = (float)DEG_TO_RAD(angle);

    float cosAngle = (float)cos(angle);
    float sineAngle = (float)sin(angle);

    if (z)
    {
        matrix[0] = cosAngle;
        matrix[1] = sineAngle;

        matrix[4] = -sineAngle;
        matrix[5] = cosAngle;
    }

    if (y)
    {
        matrix[0] = cosAngle;
        matrix[2] = -sineAngle;

        matrix[8] = sineAngle;
        matrix[10] = cosAngle;
    }

    if (x)
    {
        matrix[5] = cosAngle;
        matrix[6] = sineAngle;

        matrix[9] = -sineAngle;
        matrix[10] = cosAngle;
    }
}


void Matrix4x4::RotateAxis(double angle, Vector3D axis)
{
    axis.Normalize();

    float sinAngle =
        (float)sin(PI_CONST * angle / 180.0);

    float cosAngle =
        (float)cos(PI_CONST * angle / 180.0);

    float oneSubCos = 1.0f - cosAngle;

    matrix[0] =
        (axis.x * axis.x) * oneSubCos + cosAngle;

    matrix[4] =
        (axis.x * axis.y) * oneSubCos -
        (axis.z * sinAngle);

    matrix[8] =
        (axis.x * axis.z) * oneSubCos +
        (axis.y * sinAngle);


    matrix[1] =
        (axis.y * axis.x) * oneSubCos +
        (sinAngle * axis.z);

    matrix[5] =
        (axis.y * axis.y) * oneSubCos +
        cosAngle;

    matrix[9] =
        (axis.y * axis.z) * oneSubCos -
        (axis.x * sinAngle);


    matrix[2] =
        (axis.z * axis.x) * oneSubCos -
        (axis.y * sinAngle);

    matrix[6] =
        (axis.z * axis.y) * oneSubCos +
        (axis.x * sinAngle);

    matrix[10] =
        (axis.z * axis.z) * oneSubCos +
        cosAngle;
}


void Matrix4x4::RotateX(double angle)
{
    matrix[5] =
        (float)cos(PI_CONST * angle / 180.0);

    matrix[6] =
        (float)sin(PI_CONST * angle / 180.0);

    matrix[9] = -matrix[6];
    matrix[10] = matrix[5];
}


void Matrix4x4::RotateY(double angle)
{
    matrix[0] =
        (float)cos(PI_CONST * angle / 180.0);

    matrix[2] =
        -(float)sin(PI_CONST * angle / 180.0);

    matrix[8] = -matrix[2];
    matrix[10] = matrix[0];
}


void Matrix4x4::RotateZ(double angle)
{
    matrix[0] =
        (float)cos(PI_CONST * angle / 180.0);

    matrix[1] =
        (float)sin(PI_CONST * angle / 180.0);

    matrix[4] = -matrix[1];
    matrix[5] = matrix[0];
}


void Matrix4x4::CreateViewMatrix(
    Vector3D pos,
    Vector3D dir,
    Vector3D up,
    Vector3D right)
{
    matrix[3] = 0.0f;
    matrix[7] = 0.0f;
    matrix[11] = 0.0f;
    matrix[15] = 1.0f;

    matrix[0] = right.x;
    matrix[4] = right.y;
    matrix[8] = right.z;
    matrix[12] = right.Dot3(pos);

    matrix[1] = up.x;
    matrix[5] = up.y;
    matrix[9] = up.z;
    matrix[13] = up.Dot3(pos);

    matrix[2] = dir.x;
    matrix[6] = dir.y;
    matrix[10] = dir.z;
    matrix[14] = dir.Dot3(pos);
}


END_ENGINE_NAMESPACE