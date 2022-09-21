#include "TessellatedBezierSurface.h"

#include <Objects/Scene.h>

namespace BuD
{
	TessellatedBezierSurface::TessellatedBezierSurface(Scene& scene, const DX11Device& device,
		Vector3 position, float surfaceWidth, float surfaceLength, int patchesU, int patchesV, bool asCylinder)
		: PointBasedObject(scene, {}), m_patchesU(patchesU), m_patchesV(patchesV), m_wrappedU(asCylinder), m_wrappedV(false)
	{
		
	}
}