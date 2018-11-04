#include "Component.h"
#include "Application.h"
#include "GameObject.h"

void Component::DrawUI()
{
	float Y = ImGui::GetCursorPosY();
	DrawInfo();
	float currY = ImGui::GetCursorPosY();
	ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 40 , Y });
	DrawConfig();
	ImGui::SetCursorPosY(currY);
}

void Component::DrawConfig()
{
	ImGui::Button("+", { 19,19 });
	if (closeConfig)
	{
		if (ImGui::BeginPopupContextItem(std::to_string(type).c_str(), 0))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.9f,0.45f,0.0f,0.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.95f,0.5f,0.0f,0.4f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.37f,0.0f,0.4f });

			if (DrawExtraConfig() && type != TRANSFORMATION)
				ImGui::Separator();
			if (type != TRANSFORMATION)
			{
				ImGui::PushID("RC" + type);
				if (ImGui::Button("Remove component", { ImGui::GetContentRegionAvailWidth(), 19 }))
				{
					toDelete = true;
					closeConfig = false;
				}
				ImGui::PopID();
			}
			ImGui::PopStyleColor(3);
			ImGui::EndPopup();
		}
	}
	else
		closeConfig = true;
}

bool Component::DrawExtraConfig()
{
	ImGui::InvisibleButton("inv", { 125.0f,0.01f });

	return false;
}
