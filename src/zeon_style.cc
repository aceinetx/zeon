#include "zeon.hh"

static constexpr ImVec4 rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
}

void z::Zeon::SetupStyle() {
	auto& io = ImGui::GetIO();
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	style->FrameRounding = 2.0f;
	colors[ImGuiCol_FrameBg] = rgba(51, 51, 51, 255);
	colors[ImGuiCol_FrameBgHovered] = rgba(71, 71, 71, 255);
	colors[ImGuiCol_FrameBgActive] = rgba(81, 81, 81, 255);

	colors[ImGuiCol_TitleBgActive] = rgba(83, 83, 83, 255);

	colors[ImGuiCol_TabActive] = rgba(83, 83, 83, 255);
	colors[ImGuiCol_TabHovered] = rgba(85, 85, 85, 255);
	colors[ImGuiCol_Tab] = rgba(50, 50, 50, 255);

	colors[ImGuiCol_Header] = rgba(139, 139, 139, 79);
	colors[ImGuiCol_HeaderHovered] = rgba(149, 149, 149, 79);
	colors[ImGuiCol_HeaderActive] = rgba(159, 159, 159, 79);

	colors[ImGuiCol_CheckMark] = rgba(150, 150, 150, 255);
}
