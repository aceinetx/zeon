#include "zeon.hh"

using namespace z;

static constexpr ImVec4 rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
}

void Zeon::SetupStyle() {
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

	colors[ImGuiCol_Button] = rgba(177, 188, 201, 102);
	colors[ImGuiCol_ButtonHovered] = rgba(187, 198, 211, 122);
	colors[ImGuiCol_ButtonActive] = rgba(197, 208, 221, 142);

	colors[ImGuiCol_Header] = rgba(139, 139, 139, 79);
	colors[ImGuiCol_HeaderHovered] = rgba(149, 149, 149, 99);
	colors[ImGuiCol_HeaderActive] = rgba(159, 159, 159, 119);

	colors[ImGuiCol_SliderGrab] = rgba(185, 185, 185, 255);
	colors[ImGuiCol_SliderGrabActive] = rgba(195, 195, 195, 255);

	colors[ImGuiCol_ResizeGrip] = rgba(185, 185, 185, 70);
	colors[ImGuiCol_ResizeGripHovered] = rgba(185, 185, 185, 110);
	colors[ImGuiCol_ResizeGripActive] = rgba(185, 185, 185, 150);

	colors[ImGuiCol_CheckMark] = rgba(150, 150, 150, 255);
}
