//
// Matrix4 class definition
//

#include <math/Matrix4.h>

#include <cstring>
#include <cmath>

Matrix4::Matrix4() : _m{} {

}

Matrix4::Matrix4(const Matrix4& other) : _m{} {
    std::memcpy(&_m, &other._m, MAT4_SIZE);
}

Matrix4::Matrix4(const F32* values) : _m{} { // values given in row major format
    for (UI32 e = 0; e < 16; ++e) 
        _m[e] = values[(e & 3)*4 + (e >> 2)]; // store in column major
}

Matrix4& Matrix4::operator =(const Matrix4& other) {
    std::memcpy(&_m, other[0], MAT4_SIZE); 
    return *this;
}

bool Matrix4::operator ==(const Matrix4& other) const {
    return std::memcmp(&_m, other[0], MAT4_SIZE) == 0; // same values == 0
}

Matrix4& Matrix4::operator +=(const Matrix4& other) {
    _m16[0] = _mm_add_ps(_m16[0], other._m16[0]);
    _m16[1] = _mm_add_ps(_m16[1], other._m16[1]);
    _m16[2] = _mm_add_ps(_m16[2], other._m16[2]);
    _m16[3] = _mm_add_ps(_m16[3], other._m16[3]);
    return *this;
}
    
Matrix4& Matrix4::operator -=(const Matrix4& other) {
    _m16[0] = _mm_sub_ps(_m16[0], other._m16[0]);
    _m16[1] = _mm_sub_ps(_m16[1], other._m16[1]);
    _m16[2] = _mm_sub_ps(_m16[2], other._m16[2]);
    _m16[3] = _mm_sub_ps(_m16[3], other._m16[3]);
    return *this;
}

Matrix4& Matrix4::operator *=(const Matrix4& other) {
    // matrix multiplication
    Matrix4 p;
    UI32 r, c = 0; // row col
    for (UI32 e = 0; e < 16; ++e) {
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

Matrix4& Matrix4::operator /=(const F32& factor) {
    __m128 inv = _mm_set1_ps(1.0f / factor);
    _m16[0] = _mm_mul_ps(_m16[0], inv);
    _m16[1] = _mm_mul_ps(_m16[1], inv);
    _m16[2] = _mm_mul_ps(_m16[2], inv);
    _m16[3] = _mm_mul_ps(_m16[3], inv);
    return *this;
}
    
Matrix4& Matrix4::operator *=(const F32& factor) {
    __m128 f = _mm_set1_ps(factor);
    _m16[0] = _mm_mul_ps(_m16[0], f);
    _m16[1] = _mm_mul_ps(_m16[1], f);
    _m16[2] = _mm_mul_ps(_m16[2], f);
    _m16[3] = _mm_mul_ps(_m16[3], f);
    return *this;
}

F32* Matrix4::operator [](const UI32 index) {
    return &_m[index * 4];
}

const F32* Matrix4::operator [](const UI32 index) const {
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
    // matrix:
    // c0: a   b   c   d
    // c1: e   f   g   h   
    // c2: i   j   k   l
    // c3: m   n   o   p

    // intermediate shuffle
    __m128 c0c1 = _mm_shuffle_ps(mat._m16[0], mat._m16[1], _MM_SHUFFLE(1, 0, 1, 0));
    __m128 c2c3 = _mm_shuffle_ps(mat._m16[2], mat._m16[3], _MM_SHUFFLE(1, 0, 1, 0));
    __m128 c1c0 = _mm_shuffle_ps(mat._m16[1], mat._m16[0], _MM_SHUFFLE(2, 3, 2, 3));
    __m128 c3c2 = _mm_shuffle_ps(mat._m16[3], mat._m16[2], _MM_SHUFFLE(2, 3, 2, 3));
    // matrix:
    // c0c1: a   b   e   f
    // c2c3: i   j   m   n   
    // c1c0: c   d   g   h
    // c3c2: k   l   o   p

    Matrix4 t;
    // final shuffle
    t._m16[0] = _mm_shuffle_ps(c0c1, c2c3, _MM_SHUFFLE(2, 0, 2, 0));
    t._m16[1] = _mm_shuffle_ps(c0c1, c2c3, _MM_SHUFFLE(3, 1, 3, 1));
    t._m16[2] = _mm_shuffle_ps(c1c0, c3c2, _MM_SHUFFLE(2, 0, 2, 0));
    t._m16[3] = _mm_shuffle_ps(c1c0, c3c2, _MM_SHUFFLE(3, 1, 3, 1));
    // matrix:
    // t0: a   e   i   m
    // t1: b   f   j   n   
    // t2: c   g   k   o
    // t3: d   h   l   p
    return t;
}

Matrix4 Matrix4::TranslationMatrix(const Vector3& v) {
    Matrix4 mat = Matrix4::Identity();
    std::memcpy(&mat._m[12], &v[0], VEC3_SIZE); // bonus of column major <3
    return mat;
}

Matrix4 Matrix4::RotationMatrix(const Quaternion& q) {
    Matrix4 mat{};

    F32 xx = q[0] * q[0];
    F32 xy = q[0] * q[1];
    F32 xz = q[0] * q[2];
    F32 xw = q[0] * q[3];

    F32 yy = q[1] * q[1];
    F32 yz = q[1] * q[2];
    F32 yw = q[1] * q[3];
    
    F32 zz = q[2] * q[2];
    F32 zw = q[2] * q[3];

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

Matrix4 Matrix4::ScaleMatrix(const F32& s) {
    Matrix4 mat{};
    mat[0][0] = s;
    mat[1][1] = s;
    mat[2][2] = s;
    mat[3][3] = 1.0f;
    return mat;
}

Matrix4 Matrix4::Perspective(F32 fov, F32 aspectRatio, F32 near, F32 far) {
    F32 tanHalfFov = std::tan(fov * 0.5f);
    Matrix4 mat{};
    mat[0][0] = 1.0f / (aspectRatio * tanHalfFov);
    mat[1][1] = 1.0f / tanHalfFov;
    mat[2][2] = -far / (far - near);
    mat[2][3] = -1.0f;
    mat[3][2] = -(far * near) /  (far - near);
    return mat;
}

Matrix4 operator /(Matrix4 lhs, const F32 rhs) {
    return lhs /= rhs;
}

Matrix4 operator *(Matrix4 lhs, const F32 rhs) {
    return lhs *= rhs;
}

Matrix4 operator *(const F32 lhs, Matrix4& rhs) {
    return rhs * lhs;
}

Vector4 operator *(Matrix4 mat, const Vector4& vec) {
    __m128 res = _mm_add_ps( // 7 instructions instead of 24
        _mm_add_ps(
            _mm_mul_ps(_mm_set_ps1(vec.x), mat._m16[0]),
            _mm_mul_ps(_mm_set_ps1(vec.y), mat._m16[1])), 
        _mm_add_ps(
            _mm_mul_ps(_mm_set_ps1(vec.z), mat._m16[2]),
            _mm_mul_ps(_mm_set_ps1(vec.w), mat._m16[3])));
    return {(float*)&res};
}

std::istream & operator >> (std::istream &inStream, Matrix4 &matrix) {
    // converts to Vector4 (s²low)
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