//
// Matrix4 class definition
//

#include <math/Matrix4.h>

#include <cstring>

Matrix4::Matrix4() : _m{} {}

Matrix4::Matrix4(const Matrix4& other) : _m{} {
    std::memcpy(_m, other._m, MAT4_SIZE);
}

Matrix4::Matrix4(const float* values) : _m{} { // values given in row major format
    for (uint32_t e = 0; e < MAT4_ELEMENTS; ++e) 
        _m[e] = values[(e & 3)*4 + (e >> 2)]; // store in column major
}

Matrix4& Matrix4::operator =(const Matrix4& other) {
    std::memcpy(_m, other[0], MAT4_SIZE); 
    return *this;
}

bool Matrix4::operator ==(const Matrix4& other) const {
    return std::memcmp(_m, other[0], MAT4_SIZE) == 0; // same values == 0
}

Matrix4& Matrix4::operator +=(const Matrix4& other) {
    for (uint32_t e = 0; e < MAT4_ELEMENTS; ++e)
        _m[e] += other._m[e];
    return *this;
}
    
Matrix4& Matrix4::operator -=(const Matrix4& other) {
    for (uint32_t e = 0; e < MAT4_ELEMENTS; ++e)
        _m[e] -= other._m[e];
    return *this;
}

Matrix4& Matrix4::operator *=(const Matrix4& other) {
    // matrix multiplication
    Matrix4 p;
    uint32_t r, c = 0; // row col
    for (uint32_t e = 0; e < MAT4_ELEMENTS; ++e) {
        r = e & 3;   // modulus 4
        c = e >> 2;  // divide 4
        p._m[e] = 
            _m[c * 4]     * other._m[r] + // column major product
            _m[c * 4 + 1] * other._m[4 + r] +
            _m[c * 4 + 2] * other._m[8 + r] +
            _m[c * 4 + 3] * other._m[12 + r];
    }
    *this = p;
    return *this;
}

Matrix4& Matrix4::operator /=(const float& factor) {
    float inv = 1.0f / factor;
    for (uint32_t e = 0; e < MAT4_ELEMENTS; ++e)
        _m[e] *= inv;
    return *this;
}
    
Matrix4& Matrix4::operator *=(const float& factor) {
    for (uint32_t e = 0; e < MAT4_ELEMENTS; ++e)
        _m[e] *= factor;
    return *this;
}

float* Matrix4::operator [](const uint32_t index) {
    return &_m[index * 4];
}

const float* Matrix4::operator [](const uint32_t index) const {
    return &_m[index * 4];
}

Matrix4 Matrix4::Identity() {
    Matrix4 mat{};
    mat[0][0] = 1.0f;
    mat[1][1] = 1.0f;
    mat[2][2] = 1.0f;
    mat[3][3] = 1.0f;
    return mat;
}

Matrix4 Matrix4::Transpose(const Matrix4& mat) {
    Matrix4 t;
    for (uint32_t e = 0; e < 4; ++e) {
        // fast           slower
        t._m[e * 4]     = mat._m[e];
        t._m[e * 4 + 1] = mat._m[e + 4];
        t._m[e * 4 + 2] = mat._m[e + 8];
        t._m[e * 4 + 3] = mat._m[e + 12];
    }
    return t;
}

Matrix4 Matrix4::TranslationMatrix(const Vector3& v) {
    Matrix4 mat = Matrix4::Identity();
    std::memcpy(&mat._m[12], &v[0], VEC3_SIZE); // bonus of column major <3
    return mat;
}

Matrix4 Matrix4::RotationMatrix(const Quaternion& q) {
    Matrix4 mat = Matrix4::Identity();

    float xx = q[0] * q[0];
    float xy = q[0] * q[1];
    float xz = q[0] * q[2];
    float xw = q[0] * q[3];

    float yy = q[1] * q[1];
    float yz = q[1] * q[2];
    float yw = q[1] * q[3];
    
    float zz = q[2] * q[2];
    float zw = q[2] * q[3];

    mat[0][0] = 1.0f - 2.0f * ( yy + zz );
    mat[0][1] = 2.0f * ( xy + zw );
    mat[0][2] = 2.0f * ( xz - yw );

    mat[1][0] = 2.0f * ( xy - zw );
    mat[1][1] = 1.0f - 2.0f * ( xx + zz );
    mat[1][2] = 2.0f * ( yz + xw );

    mat[2][0] = 2.0f * ( xz + yw );
    mat[2][1] = 2.0f * ( yz - xw );
    mat[2][2] = 1.0f - 2.0f * ( xx + yy );

    mat[3][3] = 1.0f;
    
    return mat;
}

Matrix4 Matrix4::ScaleMatrix(const float& s) {
    Matrix4 mat{};
    mat[0][0] = s;
    mat[1][1] = s;
    mat[2][2] = s;
    mat[3][3] = 1.0f;
}

Matrix4 operator /(Matrix4 lhs, const float rhs) {
    return lhs /= rhs;
}

Matrix4 operator *(Matrix4 lhs, const float rhs) {
    return lhs *= rhs;
}

Matrix4 operator *(const float lhs, Matrix4& rhs) {
    return rhs * lhs;
}

std::istream & operator >> (std::istream &inStream, Matrix4 &matrix) {
    // converts to Vector4 (slow)
    return inStream >> *((Vector4*)matrix[0]) >> *((Vector4*)matrix[1]) >> 
    *((Vector4*)matrix[2]) >> *((Vector4*)matrix[3]);
}

std::ostream & operator << (std::ostream &outStream, const Matrix4 &matrix) {
    return outStream 
        << Vector4{matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]} << '\n' 
        << Vector4{matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]} << '\n' 
        << Vector4{matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]} << '\n' 
        << Vector4{matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]};
}