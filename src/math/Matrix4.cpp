//
// Matrix4 class definition
//

#include <math/thomath.h>

Mat4& Mat4::operator =(const Mat4& other) 
{
  memcpy(_m, other._m, SIZEOF_MAT4); 
  return *this;
}

bool Mat4::operator ==(const Mat4& other) const 
{
  // bad, change to vector 4 comparison!
  return memcmp(&_m, other[0], SIZEOF_MAT4) == 0; // same values == 0
}

Mat4& Mat4::operator +=(const Mat4& other) 
{
  __m[0] = _mm_add_ps(__m[0], other.__m[0]);
  __m[1] = _mm_add_ps(__m[1], other.__m[1]);
  __m[2] = _mm_add_ps(__m[2], other.__m[2]);
  __m[3] = _mm_add_ps(__m[3], other.__m[3]);
  return *this;
}
    
Mat4& Mat4::operator -=(const Mat4& other) 
{
  __m[0] = _mm_sub_ps(__m[0], other.__m[0]);
  __m[1] = _mm_sub_ps(__m[1], other.__m[1]);
  __m[2] = _mm_sub_ps(__m[2], other.__m[2]);
  __m[3] = _mm_sub_ps(__m[3], other.__m[3]);
  return *this;
}

Mat4& Mat4::operator *=(const Mat4& other) 
{
  // illustration on mat3 (same principle):

  //   column:  1   2   3
  // ---------------------
  // matrix M:  a   b   c 
  //            d   e   f    
  //            g   h   i 

  //   column:  4   5   6
  // ---------------------
  // matrix N:  j   k   l 
  //            m   n   o    
  //            p   q   r

  //   column:  X   Y    Z
  // ---------------------
  //    M * N:  x   y   z 
  //            x'  y'  z'    
  //            x'' y'' z''

  // If we write out the first colmun of the result matrix we get:
  //           x   = a * j + b * m + c * p
  //           x'  = d * j + e * m + f * p
  //           x'' = g * j + h * m + i * p

  // in column notation
  //           X   = 1 * J + 2 * M + 3 * P
  // where J is a column of only j, M of m and P of p

  //Mat4 copy = *this; // keep a reference copy of current
  __m128 c0, c1, c2, c3;

  {
    c0 = _mm_shuffle_ps(__m[0], __m[0], _MM_SHUFFLE(0, 0, 0, 0));
    c1 = _mm_shuffle_ps(__m[0], __m[0], _MM_SHUFFLE(1, 1, 1, 1));
    c2 = _mm_shuffle_ps(__m[0], __m[0], _MM_SHUFFLE(2, 2, 2, 2));
    c3 = _mm_shuffle_ps(__m[0], __m[0], _MM_SHUFFLE(3, 3, 3, 3));

    __m[0] = _mm_add_ps(
      _mm_add_ps(
        _mm_mul_ps(other.__m[0], c0), 
        _mm_mul_ps(other.__m[1], c1)),
      _mm_add_ps(
        _mm_mul_ps(other.__m[2], c2),
        _mm_mul_ps(other.__m[3], c3)));
  }

  std::cout << *this << "\n\n";

  {
    c0 = _mm_shuffle_ps(__m[1], __m[1], _MM_SHUFFLE(0, 0, 0, 0));
    c1 = _mm_shuffle_ps(__m[1], __m[1], _MM_SHUFFLE(1, 1, 1, 1));
    c2 = _mm_shuffle_ps(__m[1], __m[1], _MM_SHUFFLE(2, 2, 2, 2));
    c3 = _mm_shuffle_ps(__m[1], __m[1], _MM_SHUFFLE(3, 3, 3, 3));

    __m[1] = _mm_add_ps(
      _mm_add_ps(
        _mm_mul_ps(other.__m[0], c0),
        _mm_mul_ps(other.__m[1], c1)),
      _mm_add_ps(
        _mm_mul_ps(other.__m[2], c2),
        _mm_mul_ps(other.__m[3], c3)));
  }

  std::cout << *this << "\n\n";
  {
    c0 = _mm_shuffle_ps(__m[2], __m[2], _MM_SHUFFLE(0, 0, 0, 0));
    c1 = _mm_shuffle_ps(__m[2], __m[2], _MM_SHUFFLE(1, 1, 1, 1));
    c2 = _mm_shuffle_ps(__m[2], __m[2], _MM_SHUFFLE(2, 2, 2, 2));
    c3 = _mm_shuffle_ps(__m[2], __m[2], _MM_SHUFFLE(3, 3, 3, 3));

    __m[2] = _mm_add_ps(
      _mm_add_ps(
        _mm_mul_ps(other.__m[0], c0),
        _mm_mul_ps(other.__m[1], c1)),
      _mm_add_ps(
        _mm_mul_ps(other.__m[2], c2),
        _mm_mul_ps(other.__m[3], c3)));
  }

  std::cout << *this << "\n\n";
  {
    c0 = _mm_shuffle_ps(__m[3], __m[3], _MM_SHUFFLE(0, 0, 0, 0));
    c1 = _mm_shuffle_ps(__m[3], __m[3], _MM_SHUFFLE(1, 1, 1, 1));
    c2 = _mm_shuffle_ps(__m[3], __m[3], _MM_SHUFFLE(2, 2, 2, 2));
    c3 = _mm_shuffle_ps(__m[3], __m[3], _MM_SHUFFLE(3, 3, 3, 3));

    __m[3] = _mm_add_ps(
      _mm_add_ps(
        _mm_mul_ps(other.__m[0], c0),
        _mm_mul_ps(other.__m[1], c1)),
      _mm_add_ps(
        _mm_mul_ps(other.__m[2], c2),
        _mm_mul_ps(other.__m[3], c3)));
  }
  std::cout << *this << "\n";

  return *this;

  /*
  
  // loop based
  Mat4 p = *this;
  UI32 r, c = 0; // row col
  for (UI32 e = 0; e < 16; ++e) {
    r = e & 3;   // modulus 4
    c = e >> 2;  // divide 4
    _m[e] =
      p._m[(c << 2)] * other._m[r] + // column major product
      p._m[(c << 2) + 1] * other._m[4 + r] +
      p._m[(c << 2) + 2] * other._m[8 + r] +
      p._m[(c << 2) + 3] * other._m[12 + r];
  }
  return *this;


  // old simd solution
  Mat4 p = Mat4::Transpose(*this);
  __m128 entry, shufl;
  // loop over elements and horizontal add product
  for (UI32 e = 0; e < 16; ++e)
  {
    // multiply row with column
    entry = _mm_mul_ps(p.__m[e & 3], other.__m[e >> 2]);
    // horizontal add
    shufl = _mm_shuffle_ps(entry, entry, _MM_SHUFFLE(2, 3, 0, 1));
    entry = _mm_add_ps(entry, shufl);
    shufl = _mm_movehl_ps(shufl, entry);
    entry = _mm_add_ss(entry, shufl);
    _m[e] = _mm_cvtss_f32(entry);
  }
  return *this;
  */
}

Mat4& Mat4::operator /=(const F32& factor) 
{
  __m128 inv = _mm_set_ps1(1.0f / factor);
  __m[0] = _mm_mul_ps(__m[0], inv);
  __m[1] = _mm_mul_ps(__m[1], inv);
  __m[2] = _mm_mul_ps(__m[2], inv);
  __m[3] = _mm_mul_ps(__m[3], inv);
  return *this;
}
    
Mat4& Mat4::operator *=(const F32& factor) 
{
  __m128 f = _mm_set_ps1(factor);
  __m[0] = _mm_mul_ps(__m[0], f);
  __m[1] = _mm_mul_ps(__m[1], f);
  __m[2] = _mm_mul_ps(__m[2], f);
  __m[3] = _mm_mul_ps(__m[3], f);
  return *this;
}

F32* Mat4::operator [](const UI32 index) 
{
  return &_m[index << 2];
}

const F32* Mat4::operator [](const UI32 index) const 
{
  return &_m[index << 2];
}

Mat4 Mat4::Identity() 
{
  Mat4 mat{};
  mat[0][0] = 1.0f;
  mat[1][1] = 1.0f;
  mat[2][2] = 1.0f;
  mat[3][3] = 1.0f;
  return mat;
}

Mat4 Mat4::Transpose(const Mat4& mat) 
{
  // matrix:
  // c0: a   b   c   d
  // c1: e   f   g   h   
  // c2: i   j   k   l
  // c3: m   n   o   p

  // intermediate shuffle
  __m128 c0c1 = _mm_shuffle_ps(mat.__m[0], mat.__m[1], _MM_SHUFFLE(1, 0, 1, 0));
  __m128 c2c3 = _mm_shuffle_ps(mat.__m[2], mat.__m[3], _MM_SHUFFLE(1, 0, 1, 0));
  __m128 c1c0 = _mm_shuffle_ps(mat.__m[1], mat.__m[0], _MM_SHUFFLE(2, 3, 2, 3));
  __m128 c3c2 = _mm_shuffle_ps(mat.__m[3], mat.__m[2], _MM_SHUFFLE(2, 3, 2, 3));
  // matrix:
  // c0c1: a   b   e   f
  // c2c3: i   j   m   n   
  // c1c0: c   d   g   h
  // c3c2: k   l   o   p

  Mat4 t;
  // final shuffle
  t.__m[0] = _mm_shuffle_ps(c0c1, c2c3, _MM_SHUFFLE(2, 0, 2, 0));
  t.__m[1] = _mm_shuffle_ps(c0c1, c2c3, _MM_SHUFFLE(3, 1, 3, 1));
  t.__m[2] = _mm_shuffle_ps(c1c0, c3c2, _MM_SHUFFLE(1, 3, 1, 3));
  t.__m[3] = _mm_shuffle_ps(c1c0, c3c2, _MM_SHUFFLE(0, 2, 0, 2));
  // matrix:
  // t0: a   e   i   m
  // t1: b   f   j   n   
  // t2: c   g   k   o
  // t3: d   h   l   p
  return t;
}

Mat4 Mat4::TranslationMatrix(const Vector3& v) 
{
  Mat4 mat = Mat4::Identity();
  memcpy(mat._m + 12, v._v, SIZEOF_VEC3); // bonus of column major <3
  return mat;
}

Mat4 Mat4::RotationMatrix(const Quaternion& q) 
{
  Mat4 mat{};

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
  mat[1][0] = 2.0f * ( xy - zw );
  mat[2][0] = 2.0f * ( xz + yw );

  mat[0][1] = 2.0f * ( xy + zw );
  mat[1][1] = 1.0f - 2.0f * ( xx + zz );
  mat[2][1] = 2.0f * ( yz - xw );

  mat[0][2] = 2.0f * ( xz - yw );
  mat[1][2] = 2.0f * ( yz + xw );
  mat[2][2] = 1.0f - 2.0f * ( xx + yy );

  mat[3][3] = 1.0f;
  return mat;
}

Mat4 Mat4::ScaleMatrix(const F32& s) 
{
  Mat4 mat{};
  mat[0][0] = s;
  mat[1][1] = s;
  mat[2][2] = s;
  mat[3][3] = 1.0f;
  return mat;
}

Mat4 Mat4::Perspective(F32 fov, F32 aspectRatio, F32 near, F32 far) 
{
  F32 tanHalfFov = tan(RADIANS(fov * 0.5f));
  Mat4 mat{};
  mat[0][0] = 1.0f / (aspectRatio * tanHalfFov);
  mat[1][1] = 1.0f / tanHalfFov;
  mat[2][2] = -far / (near - far);
  mat[3][2] = -1.0f;
  mat[2][3] = -(2.0f * far * near) / (far - near);
  return mat;
}

Matrix4 operator /(Matrix4 lhs, const F32 rhs) {
  return lhs /= rhs;
}

Matrix4 operator *(Matrix4 lhs, const F32 rhs) {
  return lhs *= rhs;
}

Matrix4 operator *(const F32 lhs, Matrix4& rhs) 
{
  return rhs * lhs;
}

Vector4 operator *(Matrix4 mat, const Vector4& vec) 
{
  __m128 res = _mm_add_ps( // 7 instructions instead of 24
    _mm_add_ps(
      _mm_mul_ps(_mm_set_ps1(vec.x), mat.__m[0]),
      _mm_mul_ps(_mm_set_ps1(vec.y), mat.__m[1])), 
    _mm_add_ps(
      _mm_mul_ps(_mm_set_ps1(vec.z), mat.__m[2]),
      _mm_mul_ps(_mm_set_ps1(vec.w), mat.__m[3])));

  return {reinterpret_cast<F32*>(&res)};
}

std::istream & operator >> (std::istream &inStream, Matrix4 &matrix) {
  // converts to Vector4 (slow)
  return inStream 
    >> *reinterpret_cast<Vector4*>(matrix[0]) 
    >> *reinterpret_cast<Vector4*>(matrix[1])
    >> *reinterpret_cast<Vector4*>(matrix[2])
    >> *reinterpret_cast<Vector4*>(matrix[3]);
}

std::ostream & operator << (std::ostream &outStream, const Matrix4 &matrix) {
  return outStream // constructs Vector4s (slow)
    << Vector4{matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]} << '\n' 
    << Vector4{matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]} << '\n' 
    << Vector4{matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]} << '\n' 
    << Vector4{matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]};
}