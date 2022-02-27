#include "OrthographicCamera.h"

BuD::OrthographicCamera::OrthographicCamera(gmtl::Vec3f position, gmtl::Vec3f front, gmtl::Vec3f worldUp, float ratio)
	: AbstractCamera(position, front, worldUp, ratio)
{
}

void BuD::OrthographicCamera::UpdateProjectionMatrix()
{
	float data[] = {
		2.0f / (m_projRight - m_projLeft), 0.0f, 0.0f, -(m_projRight + m_projLeft) / (m_projRight - m_projLeft),
		0.0f, 2.0f / (m_projTop - m_projBottom), 0.0f, -(m_projTop + m_projBottom) / (m_projTop - m_projBottom),
		0.0f, 0.0f, -2.0f / (m_projFar - m_projNear), (m_projFar + m_projNear) / (m_projFar - m_projNear),
		0.0f, 0.0f, 0.0f, 1.0f
	};

	m_projectionMatrix.set(data);
}
