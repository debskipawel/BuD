#include "Parameterized2DEntity.h"

namespace BuD
{
	Parameterized2DEntity::Parameterized2DEntity(Vector2 minDomain, Vector2 maxDomain, std::function<Vector3(Vector2)> objectFunction)
		: ParameterizedObject<Vector2>(objectFunction, minDomain, maxDomain)
	{
	}

	void Parameterized2DEntity::SampleBy(Vector2 interval)
	{
		if (m_prevInterval == interval)
		{
			return;
		}

		m_prevInterval = interval;

		UINT xSplits = (m_max.x - m_min.x) / interval.x;
		UINT ySplits = (m_max.y - m_min.y) / interval.y;

		UINT verticesCount = xSplits * ySplits;

		m_vertices.clear();
		m_indices.clear();

		m_vertices.reserve(verticesCount);
		m_indices.reserve(3 * verticesCount);

		for (int i = 0; i < xSplits; i++)
		{
			float x = m_min.x + i * interval.x;

			for (int j = 0; j < ySplits; j++)
			{
				float y = m_min.y + j * interval.y;

				auto point = GetPoint({ x, y });

				m_vertices.push_back(point);

				int t = (i * ySplits + j) % verticesCount;
				int u = ((i + 1) * ySplits + j + 1) % verticesCount;
				int v = ((i + 1) * ySplits + j) % verticesCount;

				m_indices.push_back(t);
				m_indices.push_back(u);
				m_indices.push_back(v);
			}
		}
	}
}
