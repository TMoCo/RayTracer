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

Matrix4 Matrix4::Transpose() {
    Matrix4 t = *this; // copy this
    for (uint32_t e = 0; e < 4; ++e) {
        // fast           slower
        t._m[e * 4]     = _m[e];
        t._m[e * 4 + 1] = _m[e + 4];
        t._m[e * 4 + 2] = _m[e + 8];
        t._m[e * 4 + 3] = _m[e + 12];
    }
    return t; // return transposed copy
}

void Matrix4::Transpose(Matrix4& mat) {
    Matrix4 t = mat; // copy mat
    for (uint32_t e = 0; e < 4; ++e) {
        mat._m[e * 4]     = t._m[e];
        mat._m[e * 4 + 1] = t._m[e + 4];
        mat._m[e * 4 + 2] = t._m[e + 8];
        mat._m[e * 4 + 3] = t._m[e + 12];
    }
}

Matrix4 Matrix4::Translate(const Vector3& vec) {
    Matrix4 mat = *this;
    mat._m[12] += vec[0]; // column major, super fast and cheap
    mat._m[13] += vec[1];
    mat._m[14] += vec[2];
    return mat;
}

void Matrix4::Translate(Matrix4& mat, const Vector3& vec) {
    mat._m[12] += vec[0];
    mat._m[13] += vec[1];
    mat._m[14] += vec[2];
}

Matrix4 Matrix4::Identity() {
    Matrix4 mat{};
    mat[0][0] = 1.0f;
    mat[1][1] = 1.0f;
    mat[2][2] = 1.0f;
    mat[3][3] = 1.0f;
    return mat;
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