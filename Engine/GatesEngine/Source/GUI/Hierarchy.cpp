#include "..\..\Header\GUI\Hierarchy.h"
#include "..\..\Header\GUI\GUIManager.h"

bool GE::GUI::Hierarchy::OnGui(GameObject* gameObject)
{
#ifdef _DEBUG
#else
	return false;
#endif // _DEBUG

	bool isInputButton = false;
	ImGui::Begin("Hierarchy");
	ImGui::PushID(gameObject);
	isInputButton = ImGui::Button(gameObject->GetName().c_str());
	ImGui::PopID();
	ImGui::End();

	return isInputButton;
}
