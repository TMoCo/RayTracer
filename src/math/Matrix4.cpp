//
// Matrix4 class definition
//

#include <math/Matrix4.h>

#include <cstring>

Matrix4::Matrix4() : _m{} {}

Matrix4::Matrix4(const Matrix4& other) : _m{} {
    std::memcpy(_m, other._m, MAT4_SIZE);
}

Matrix4::Matrix4(const float* values) : _m{} {
    std::memcpy(_m, values, MAT4_SIZE);
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
        r = e >> 2; // divide by 4
        c = e & 3;  // modulo 4
        p._m[e] = 
            _m[r]      * other._m[c * 4] + // column major product
            _m[4 + r]  * other._m[c * 4 + 1] +
            _m[8 + r]  * other._m[c * 4 + 2] +
            _m[12 + r] * other._m[c * 4 + 3];
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
        t._m[e * 4]     = _m[e];
        t._m[e * 4 + 1] = _m[e + 4];
        t._m[e * 4 + 2] = _m[e + 8];
        t._m[e * 4 + 3] = _m[e + 12];
    }
    return t; // return transposed copy
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
    return outStream << *((Vector4*)matrix[0]) << '\n' << *((Vector4*)matrix[1]) << 
    '\n' << *((Vector4*)matrix[2]) << '\n' << *((Vector4*)matrix[3]);
}