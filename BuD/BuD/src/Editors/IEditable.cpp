#include "IEditable.h"

#include <imgui.h>

namespace BuD
{
	void IEditable::EditorFor(Vector2& vec, const char* title, float min, float max)
	{
		ImGui::Text(title);
		EditorFor(vec, min, max);
	}

	void IEditable::EditorFor(Vector2& vec, float min, float max)
	{
		ImGui::DragFloat("x: ", &vec.x, min, max);
		ImGui::DragFloat("y: ", &vec.y, min, max);
	}

	void IEditable::EditorFor(Vector3& vec, const char* title, float min, float max)
	{
		ImGui::Text(title);
		EditorFor(vec, min, max);
	}

	void IEditable::EditorFor(Vector3& vec, float min, float max)
	{
		ImGui::DragFloat("x: ", &vec.x, min, max);
		ImGui::DragFloat("y: ", &vec.y, min, max);
		ImGui::DragFloat("z: ", &vec.z, min, max);
	}
}