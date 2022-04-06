#include "Parameterized2DEntity.h"

#include <imgui.h>

namespace BuD
{
	Parameterized2DEntity::Parameterized2DEntity(Vector2 minDomain, Vector2 maxDomain, std::function<Vector3(Vector2)> objectFunction)
		: ParameterizedObject<Vector2>(objectFunction, minDomain, maxDomain), m_samplesU(0), m_samplesV(0)
	{
	}

	bool Parameterized2DEntity::UpdateSampleIntervals(UINT samplesU, UINT samplesV)
	{
		if (samplesU == m_samplesU && samplesV == m_samplesV)
		{
			return false;
		}

		m_samplesU = samplesU;
		m_samplesV = samplesV;

		auto domain = m_max - m_min;
		m_prevInterval = { domain.x / samplesU, domain.y / samplesV };

		Sample();

		return true;
	}

	bool Parameterized2DEntity::DrawGui()
	{
		int samplesU = m_samplesU;
		int samplesV = m_samplesV;

		const int samplesMinU = 3;
		const int samplesMinV = 3;

		ImGui::Text("Samples count");
		ImGui::DragInt("U", &samplesU, 1.0f, 1, 100);
		ImGui::DragInt("V", &samplesV, 1.0f, 1, 100);

		samplesU = samplesU < samplesMinU ? samplesMinU : samplesU;
		samplesV = samplesV < samplesMinV ? samplesMinV : samplesV;

		if (UpdateSampleIntervals(samplesU, samplesV))
		{
			UpdateRenderableModel();

			return true;
		}

		return false;
	}

	void Parameterized2DEntity::Sample()
	{
		UINT xSplits = (m_max.x - m_min.x) / m_prevInterval.x;
		UINT ySplits = (m_max.y - m_min.y) / m_prevInterval.y;

		UINT verticesCount = xSplits * ySplits;

		m_vertices.clear();
		m_indices.clear();

		m_vertices.reserve(verticesCount);
		m_indices.reserve(6 * verticesCount);

		for (int i = 0; i < xSplits; i++)
		{
			float x = m_min.x + i * m_prevInterval.x;

			for (int j = 0; j < ySplits; j++)
			{
				float y = m_min.y + j * m_prevInterval.y;

				auto point = GetPoint({ x, y });

				m_vertices.push_back(point);

				int t = (i * ySplits + j) % verticesCount;
				int u = (i * ySplits + (j + 1) % ySplits) % verticesCount;
				int v = ((i + 1) * ySplits + (j + 1) % ySplits) % verticesCount;
				int w = ((i + 1) * ySplits + j) % verticesCount;

				m_indices.push_back(t);
				m_indices.push_back(u);
				m_indices.push_back(v);
				m_indices.push_back(t);
				m_indices.push_back(v);
				m_indices.push_back(w);
			}
		}
	}
}
