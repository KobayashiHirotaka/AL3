#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateMatrix(const Vector3& radian);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 VectorAdd(const Vector3& translation, const Vector3& move);