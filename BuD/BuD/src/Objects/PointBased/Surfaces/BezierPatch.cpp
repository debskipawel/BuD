#include "BezierPatch.h"
#include "BezierSurface.h"

#include <Objects/Scene.h>

namespace BuD
{
	BezierPatch::BezierPatch(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurface* owner)
		: PointBasedObject(scene, controlPoints), m_owner(owner), m_showBezierPolygon(false)
	{
		m_samplesU = samplesU;
		m_samplesV = samplesV;
	}

	void BezierPatch::OnDelete()
	{
		PointBasedObject::OnDelete();

		if (m_owner)
		{
			m_owner->OnDeletePatch(this);
		}
	}
	
	void BezierPatch::OnSelect()
	{
		SceneObject::OnSelect();
		m_color = { 0.7f, 0.5f, 0.0f };
	}

	void BezierPatch::OnUnselect()
	{
		SceneObject::OnUnselect();
		m_color = { 1.0f, 1.0f, 1.0f };
	}

	void BezierPatch::ToggleBezierPolygon(bool show)
	{
		if (show)
		{
			if (m_showBezierPolygon)
			{
				return;
			}

			m_showBezierPolygon = true;
			m_meshes.push_back(m_bezierPolygonMesh);
		}
		else
		{
			m_showBezierPolygon = false;
			m_meshes.erase(std::find(m_meshes.begin(), m_meshes.end(), m_bezierPolygonMesh));
		}
	}

}