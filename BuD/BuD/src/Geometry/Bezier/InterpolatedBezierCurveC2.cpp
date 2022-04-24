#include "InterpolatedBezierCurveC2.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	InterpolatedBezierCurveC2::InterpolatedBezierCurveC2(const DX11Device& device, std::vector<SceneObject*> controlPoints)
		: BezierCurve(controlPoints)
	{
		m_tag = "Interpolated Bezier C2";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/interpolated_bezier_c2_vs.hlsl", elements);
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device.Raw(), L"../BuD/shaders/interpolated_bezier_c2_gs.hlsl");
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/bezier_with_polygon_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		geometryShader->AddConstantBuffer(GSConstantBuffer1(device));
		geometryShader->AddConstantBuffer(GSConstantBuffer2(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this, device](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				FilterControlPoints();
				UpdateCentroid();
				CalculateSplineParameters();

				size_t controlPointsCount = m_controlPoints.size();

				std::vector<unsigned short> indices;
				indices.reserve(controlPointsCount);

				for (int i = 0; i < m_splineParameters.size() * 4; i++)
				{
					indices.push_back(i);
				}

				entity->VertexBuffer()->Update(m_splineParameters.data(), m_splineParameters.size() * sizeof(Vector3[4]));
				entity->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));

				auto matrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->GeometryShader()->UpdateConstantBuffer(1, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));

				struct GSResource
				{
					uint32_t samples;
					uint32_t drawPolygon;
				};

				auto rect = GetSurroundingRectangle(camera, device.BufferWidth(), device.BufferHeight());
				auto longSide = max(rect.right - rect.left, rect.bottom - rect.top);

				GSResource resource = { longSide / 10, m_drawPolygon };
				entity->GeometryShader()->UpdateConstantBuffer(0, &resource, sizeof(GSResource));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);
	}

	std::vector<Vector3> InterpolatedBezierCurveC2::SolveTridiagonal()
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

		constexpr float a = 0.5f, b = 2.0f, c = 0.5f;

		cPrim[0] = c / b;
		dPrim[0] = EquationResult(0) / b;

		for (int i = 1; i < eqCount - 1; i++)
		{
			float denominator = b - a * cPrim[i - 1];
			cPrim[i] = c / denominator;
			dPrim[i] = (EquationResult(i) - a * dPrim[i - 1]) / denominator;
		}

		if (eqCount > 1)
		{
			dPrim[eqCount - 1] = (EquationResult(eqCount - 1) - a * dPrim[eqCount - 2]) / (b - a * cPrim[eqCount - 2]);
		}

		cParams[cpCount - 2] = dPrim[eqCount - 1];

		for (int i = cpCount - 3; i > 0; i--)
		{
			cParams[i] = dPrim[i - 1] - cPrim[i - 1] * cParams[i + 1];
		}

		return cParams;
	}

	void InterpolatedBezierCurveC2::CalculateSplineParameters()
	{
		auto cParams = SolveTridiagonal();
		auto cpCount = m_controlPoints.size();
		auto n = cpCount - 1;

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
			m_splineParameters[i - 1][3] = (m_splineParameters[i][2] - m_splineParameters[i - 1][2]) / 3.0f;
		}

		for (int i = 1; i < n; i++)
		{
			m_splineParameters[i - 1][1] = m_splineParameters[i][0] - m_splineParameters[i - 1][0] - m_splineParameters[i - 1][2] - m_splineParameters[i - 1][3];
		}

		if (n > 1)
		{
			m_splineParameters[n - 1][1] = m_splineParameters[n - 2][1] + 2.0f * m_splineParameters[n - 2][2] + 3.0f * m_splineParameters[n - 2][3];
			m_splineParameters[n - 1][3] = m_splineParameters[n][0] - m_splineParameters[n - 1][0] - m_splineParameters[n - 1][1] - m_splineParameters[n - 1][2];
		}
	}

	Vector3 InterpolatedBezierCurveC2::EquationResult(int i)
	{
		return 1.5f * (m_controlPoints[i + 2]->Position() - 2.0f * m_controlPoints[i + 1]->Position() + m_controlPoints[i]->Position());
	}

	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::VSConstantBuffer(const DX11Device& device)
	{
		if (!s_vsConstantBuffer)
		{
			s_vsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_vsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::GSConstantBuffer1(const DX11Device& device)
	{
		if (!s_gsConstantBuffer1)
		{
			s_gsConstantBuffer1 = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_gsConstantBuffer1;
	}

	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::GSConstantBuffer2(const DX11Device& device)
	{
		if (!s_gsConstantBuffer2)
		{
			s_gsConstantBuffer2 = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_gsConstantBuffer2;
	}

	GeometryType InterpolatedBezierCurveC2::GetType()
	{
		return GeometryType::BEZIER_C2;
	}

	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::PSConstantBuffer(const DX11Device& device)
	{
		if (!s_psConstantBuffer)
		{
			s_psConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Vector4));
		}

		return s_psConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::s_gsConstantBuffer1 = nullptr;
	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::s_gsConstantBuffer2 = nullptr;
	std::shared_ptr<DX11ConstantBuffer> InterpolatedBezierCurveC2::s_psConstantBuffer = nullptr;
}
