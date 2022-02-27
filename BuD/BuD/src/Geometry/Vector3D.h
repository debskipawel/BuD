#pragma once

#include <gmtl/gmtl.h>

namespace BuD
{
	class Vector3D
	{
	public:
		Vector3D() : m_vector() {}
		Vector3D(float x, float y, float z) : m_vector(x, y, z) {}
		Vector3D(const Vector3D& vec) : m_vector(vec.m_vector) {}
		Vector3D(float* data) : m_vector(data[0], data[1], data[2]) {}

		inline void operator=(const Vector3D& other) { m_vector = other.m_vector; }

		inline float operator[](int index) const { return m_vector[index]; }

		inline Vector3D operator-() const
		{
			return Vector3D(((gmtl::Vec3f)(-m_vector)).getData());
		}

		inline Vector3D operator+(const Vector3D& other) const
		{
			return Vector3D(((gmtl::Vec3f)(m_vector + other.m_vector)).getData());
		}

		inline void operator+=(const Vector3D& other)
		{
			m_vector += other.m_vector;
		}

		inline void operator-=(const Vector3D& other)
		{
			m_vector -= other.m_vector;
		}

		inline Vector3D operator*(float value) const
		{
			return Vector3D(((gmtl::Vec3f)(value * m_vector)).getData());
		}

		inline Vector3D operator/(float value) const
		{
			return Vector3D(((gmtl::Vec3f)(m_vector / value)).getData());
		}

		inline Vector3D operator-(const Vector3D& other) const
		{
			auto res = m_vector - other.m_vector;
			return Vector3D(res[0], res[1], res[2]);
		}

		inline float DotProduct(const Vector3D& other) const
		{
			return gmtl::dot(m_vector, other.m_vector);
		}

		inline Vector3D CrossProduct(const Vector3D& other) const
		{
			return Vector3D(gmtl::makeCross(m_vector, other.m_vector).getData());
		}

		inline float Length() const { return gmtl::length(m_vector); }

		inline void Normalize() { m_vector = gmtl::makeNormal(m_vector); }
		
		inline Vector3D Normalized() const
		{
			return Vector3D(gmtl::makeNormal(m_vector).getData());
		}

	private:
		gmtl::Vec3f m_vector;
	};
}
