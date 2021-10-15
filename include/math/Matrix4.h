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

// column major matrix
typedef struct Matrix4 {
public:
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
  static Matrix4 Identity();

  static Matrix4 Transpose(const Matrix4& mat);

  static Matrix4 TranslationMatrix(const Vector3& v);

  static Matrix4 RotationMatrix(const Quaternion& q);

  static Matrix4 ScaleMatrix(const F32& s);

  static Matrix4 Perspective(F32 FOV, F32 aspectRatio, F32 near, F32 far);
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
