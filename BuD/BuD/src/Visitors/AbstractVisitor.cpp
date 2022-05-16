#include "AbstractVisitor.h"

#include <Objects/Abstract/SceneObject.h>

namespace BuD
{
	void AbstractVisitor::Visit(SceneObject& obj)
	{
		obj.Accept(*this);
	}
}
