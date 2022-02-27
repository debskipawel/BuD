#pragma once

#include <gmtl/gmtl.h>

#include "Vector3D.h"

namespace BuD
{
	class Matrix3D
	{
	public:
		Matrix3D() : m_matrix() {}
		Matrix3D(const float* data) : m_matrix()
		{
			m_matrix.set(data);
		}

		inline friend Vector3D operator*(const Matrix3D& matrix, const Vector3D& vector)
		{
			return Vector3D((matrix.m_matrix * gmtl::Vec4f(vector[0], vector[1], vector[2], 1.0)).getData());
		}

		inline friend Matrix3D operator*(const Matrix3D& matrix, const Matrix3D& other)
		{
			return Matrix3D((matrix.m_matrix * other.m_matrix).getData());
		}

	private:
		gmtl::Matrix44f m_matrix;
	};
}