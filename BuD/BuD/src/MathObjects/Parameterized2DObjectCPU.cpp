#include "Parameterized2DObjectCPU.h"

namespace BuD
{
	void Parameterized2DObjectCPU::Meshify()
	{
		int samplesU = m_samplesU + 1;
		int samplesV = m_samplesV + 1;

		UINT verticesCount = samplesU * samplesV;

		m_vertices.clear();
		m_indices.clear();

		m_vertices.reserve(verticesCount);
		m_indices.reserve(6 * verticesCount);

		auto minDom = MinDomain();
		auto maxDom = MaxDomain();

		float du = (maxDom.x - minDom.x) / (samplesU - 1);
		float dv = (maxDom.y - minDom.y) / (samplesV - 1);

		float u = minDom.x;
		float v = minDom.y;

		for (int i = 0; i < samplesU - 1; i++)
		{
			v = minDom.y;

			for (int j = 0; j < samplesV - 1; j++)
			{
				auto point = GetPoint({ u, v });

				m_vertices.push_back(point);

				int v1 = i * samplesV + j;
				int v2 = i * samplesV + j + 1;
				int v3 = (i + 1) * samplesV + j + 1;
				int v4 = (i + 1) * samplesV + j;

				m_indices.push_back(v1);
				m_indices.push_back(v2);
				m_indices.push_back(v3);
				m_indices.push_back(v1);
				m_indices.push_back(v3);
				m_indices.push_back(v4);

				v += dv;
			}

			auto point = GetPoint({ u, v });

			m_vertices.push_back(point);

			// connect with the beginning
			if (ParameterizationWrapped())
			{
				int v1 = (i + 1) * samplesV - 1;
				int v2 = i * samplesV;
				int v3 = (i + 1) * samplesV;
				int v4 = (i + 2) * samplesV - 1;

				m_indices.push_back(v1);
				m_indices.push_back(v2);
				m_indices.push_back(v4);
				m_indices.push_back(v1);
				m_indices.push_back(v4);
				m_indices.push_back(v3);
			}

			u += du;
		}

		int i = samplesU - 1;

		v = minDom.y;

		for (int j = 0; j < samplesV; j++)
		{
			auto point = GetPoint({ u, v });

			m_vertices.push_back(point);

			if (ParameterizationWrapped())
			{
				int v1 = (i * samplesV + j) % verticesCount;
				int v2 = (i * samplesV + (j + 1) % samplesV) % verticesCount;
				int v3 = ((i + 1) * samplesV + (j + 1) % samplesV) % verticesCount;
				int v4 = ((i + 1) * samplesV + j) % verticesCount;

				m_indices.push_back(v1);
				m_indices.push_back(v2);
				m_indices.push_back(v3);
				m_indices.push_back(v1);
				m_indices.push_back(v3);
				m_indices.push_back(v4);
			}

			v += dv;
		}
	}
}