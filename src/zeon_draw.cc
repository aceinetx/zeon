#include "zeon.hh"
#include "zeondefs.hh"

static char url[65535];

void z::Zeon::Draw() {
	auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({io.DisplaySize.x, ZEON_TOPBAR_HEIGHT - 1});
	ImGui::Begin("zeon", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Text("zeon");
	ImGui::SameLine();
	if (ImGui::InputText("##url", url, sizeof url, ImGuiInputTextFlags_EnterReturnsTrue)) {
		browser->GetMainFrame()->LoadURL(CefString(url));
	}

	ImGui::SameLine();

	ImGui::End();
}
