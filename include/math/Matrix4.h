//
// Matrix4 class declaration
//

#ifndef MATRIX4_H_
#define MATRIX4_H_ 1

#include <core/types.h>

#include <math/Vector4.h>
#include <math/Quaternion.h>

#include <string.h>

constexpr UI32 SIZEOF_MAT4 = 0x40;

// TODO: add matrix inverse method
typedef struct Matrix4 {
public:
// column major matrix
  union {
    F32     _m[16];
    __m128 __m[4];
  };

  constexpr Matrix4() : _m{} {}

  Matrix4(const Matrix4& other) : _m{} { memcpy(&_m, &other._m, SIZEOF_MAT4); }

  Matrix4(const F32* values) : _m{}
  {
    for (UI32 e = 0; e < 16; ++e)
      _m[e] = values[((e & 3) << 2) + (e >> 2)]; // store in column major
  }

  Matrix4& operator =(const Matrix4& other);

  bool operator ==(const Matrix4& other) const;

  Matrix4& operator +=(const Matrix4& other);

  Matrix4& operator -=(const Matrix4& other);

  Matrix4& operator *=(const Matrix4& other);

  Matrix4& operator *=(const F32& factor);

  Matrix4& operator /=(const F32& other);

  friend inline Matrix4 operator *(Matrix4 lhs, const Matrix4& rhs) 
  {
    return lhs *= rhs;
  }

  friend inline Matrix4 operator +(Matrix4 lhs, const Matrix4& rhs) {
      return lhs += rhs;
  }

  friend inline Matrix4 operator -(Matrix4 lhs, const Matrix4& rhs) {
      return lhs -= rhs;
  }

  // access operators
  F32* operator [](const UI32 index);

  const F32* operator [](const UI32 index) const;

  // static matrix builders
  static Matrix4 identity();

  static Matrix4 transpose(const Matrix4& m);

  static Matrix4 translation(const Vector3& v);

  static Matrix4 rotation(const Quaternion& q);

  static Matrix4 scale(const Vector3& scale);

  static Matrix4 perspective(F32 FOV, F32 aspectRatio, F32 near, F32 far);

  static Matrix4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
} Matrix4;

// binary operators
Matrix4 operator *(Matrix4 lhs, const F32 rhs);

Matrix4 operator *(const F32 lhs, Matrix4& rhs);

Matrix4 operator /(Matrix4 lhs, const F32 rhs);

Vector4 operator *(Matrix4 mat, const Vector4& vec);

// stream i/o
std::istream & operator >> (std::istream &outStream, Matrix4 &matrix);

std::ostream & operator << (std::ostream &outStream, const Matrix4 &matrix);

#endif // !MATRIX4_H_
