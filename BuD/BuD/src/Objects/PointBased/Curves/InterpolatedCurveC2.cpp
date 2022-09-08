#include "InterpolatedCurveC2.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

#include <Objects/Scene.h>
#include <Objects/Independent/Point.h>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	InterpolatedCurveC2::InterpolatedCurveC2(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints)
		: BezierCurve(scene, controlPoints)
	{
		m_tag = "Interpolated Bezier C2";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device, L"../BuD/shaders/bezier_curve_c0_gs.hlsl", { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/bezier_with_polygon_ps.hlsl", { sizeof(Vector4) });

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this, device](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));

				struct GSResource
				{
					uint32_t samples;
					uint32_t drawPolygon;
				};

				auto rect = GetSurroundingRectangle(view, projection, device.BufferWidth(), device.BufferHeight());
				auto longSide = max(rect.right - rect.left, rect.bottom - rect.top);

				GSResource resource = { longSide / 10, m_drawPolygon };
				entity->GeometryShader()->UpdateConstantBuffer(0, &resource, sizeof(GSResource));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);

		OnUpdate();
	}

	void InterpolatedCurveC2::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}

	void InterpolatedCurveC2::OnUpdate()
	{
		auto cpCount = m_controlPoints.size();
		auto n = cpCount - 1;

		if (cpCount < 2)
		{
			return;
		}

		// nie jestem z tego dumny, ale dziala
		if (cpCount == 2)
		{
			m_splineParameters.resize(1);

			auto p1 = m_controlPoints[0]->Position();
			auto p2 = m_controlPoints[1]->Position();

			m_splineParameters[0][0] = p1;
			m_splineParameters[0][1] = p1;
			m_splineParameters[0][2] = p2;
			m_splineParameters[0][3] = p2;

			// TODO: update the buffers
			return;
		}

		m_distances.resize(n + 1);

		for (int i = 0; i < n; i++)
		{
			auto dist = (m_controlPoints[i + 1]->Position() - m_controlPoints[i]->Position()).Length();
			m_distances[i] = abs(dist) > 0.001f ? dist : 1.0f;
		}

		m_distances[n] = 1.0f;

		auto cParams = SolveTridiagonal();

		m_splineParameters.clear();
		m_splineParameters.resize(n + 1);

		for (int i = 0; i < n; i++)
		{
			m_splineParameters[i][0] = m_controlPoints[i]->Position();
			m_splineParameters[i][2] = cParams[i];
		}

		m_splineParameters[n][0] = m_controlPoints[n]->Position();

		for (int i = 1; i < n; i++)
		{
			m_splineParameters[i - 1][3] = (m_splineParameters[i][2] - m_splineParameters[i - 1][2]) / (3.0f * m_distances[i - 1]);
		}

		for (int i = 1; i < n; i++)
		{
			m_splineParameters[i - 1][1] = (m_splineParameters[i][0] - m_splineParameters[i - 1][0] - m_splineParameters[i - 1][2] * powf(m_distances[i - 1], 2.0f) - m_splineParameters[i - 1][3] * powf(m_distances[i - 1], 3.0f)) / m_distances[i - 1];
		}

		if (n > 1)
		{
			m_splineParameters[n - 1][1] = m_splineParameters[n - 2][1] + 2.0f * m_splineParameters[n - 2][2] * m_distances[n - 2] + 3.0f * m_splineParameters[n - 2][3] * powf(m_distances[n - 2], 2.0f);
			m_splineParameters[n - 1][3] = (m_splineParameters[n][0] - m_splineParameters[n - 1][0] - m_splineParameters[n - 1][1] * m_distances[n - 1] - m_splineParameters[n - 1][2] * powf(m_distances[n - 1], 2.0f)) / powf(m_distances[n - 1], 3.0f);
		}

		TransformPowerToBernstein();

		size_t controlPointsCount = m_controlPoints.size();

		std::vector<unsigned short> indices;
		indices.reserve(controlPointsCount);

		for (int i = 0; i < m_splineParameters.size() * 4; i++)
		{
			indices.push_back(i);
		}

		m_meshes[0]->VertexBuffer()->Update(m_splineParameters.data(), m_splineParameters.size() * sizeof(Vector3[4]));
		m_meshes[0]->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));
	}

	std::vector<Vector3> InterpolatedCurveC2::SolveTridiagonal()
	{
		uint32_t cpCount = m_controlPoints.size();
		uint32_t eqCount = cpCount - 2;

		if (cpCount < 2)
		{
			return std::vector<Vector3>();
		}

		std::vector<Vector3> cParams(cpCount);

		if (eqCount == 0)
		{
			return cParams;
		}

		std::vector<float> cPrim(max(1, eqCount - 1));
		std::vector<Vector3> dPrim(eqCount);

		cPrim[0] = m_distances[1] / (2.0f * (m_distances[0] + m_distances[1]));
		dPrim[0] = EquationResult(0) / 2.0f;

		for (int i = 1; i < eqCount - 1; i++)
		{
			float b = 2.0f;
			float a = m_distances[i] / (m_distances[i] + m_distances[i + 1]);
			float c = 1.0f - a;

			float denominator = b - a * cPrim[i - 1];
			cPrim[i] = c / denominator;
			dPrim[i] = (EquationResult(i) - a * dPrim[i - 1]) / denominator;
		}

		if (eqCount > 1)
		{
			float b = 2.0f;
			float a = m_distances[eqCount - 1] / (m_distances[eqCount - 1] + m_distances[eqCount]);
			float c = 1.0f - a;

			dPrim[eqCount - 1] = (EquationResult(eqCount - 1) - a * dPrim[eqCount - 2]) / (b - a * cPrim[eqCount - 2]);
		}

		cParams[cpCount - 2] = dPrim[eqCount - 1];

		for (int i = cpCount - 3; i > 0; i--)
		{
			cParams[i] = dPrim[i - 1] - cPrim[i - 1] * cParams[i + 1];
		}

		return cParams;
	}

	void InterpolatedCurveC2::TransformPowerToBernstein()
	{
		for (int i = 0; i < m_splineParameters.size(); i++)
		{
			auto& parameter = m_splineParameters[i];
			float dist = m_distances[i];

			Matrix bernsteinToPower(
				1.0f, -3.0f / dist, 3.0f / powf(dist, 2.0f), -1.0f / powf(dist, 3.0f),
				0.0f, 3.0f / dist, -6.0f / powf(dist, 2.0f), 3.0f / powf(dist, 3.0f),
				0.0f, 0.0f, 3.0f / powf(dist, 2.0f), -3.0f / powf(dist, 3.0f),
				0.0f, 0.0f, 0.0f, 1.0f / powf(dist, 3.0f)
			);

			Matrix powerToBernstein = bernsteinToPower.Invert();

			Vector4 v1, v2, v3;

			v1 = { parameter[0].x, parameter[1].x, parameter[2].x, parameter[3].x };
			v2 = { parameter[0].y, parameter[1].y, parameter[2].y, parameter[3].y };
			v3 = { parameter[0].z, parameter[1].z, parameter[2].z, parameter[3].z };

			v1 = Vector4::Transform(v1, powerToBernstein);
			v2 = Vector4::Transform(v2, powerToBernstein);
			v3 = Vector4::Transform(v3, powerToBernstein);

			parameter[0].x = v1.x;
			parameter[1].x = v1.y;
			parameter[2].x = v1.z;
			parameter[3].x = v1.w;

			parameter[0].y = v2.x;
			parameter[1].y = v2.y;
			parameter[2].y = v2.z;
			parameter[3].y = v2.w;

			parameter[0].z = v3.x;
			parameter[1].z = v3.y;
			parameter[2].z = v3.z;
			parameter[3].z = v3.w;
		}
	}

	Vector3 InterpolatedCurveC2::EquationResult(int i)
	{
		return 3.0f / (m_distances[i] + m_distances[i + 1]) * (
			(m_controlPoints[i + 2]->Position() - m_controlPoints[i + 1]->Position()) / m_distances[i + 1] -
			(m_controlPoints[i + 1]->Position() - m_controlPoints[i]->Position()) / m_distances[i]
		);
	}
}