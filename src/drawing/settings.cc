#include "../zeon.hh"
#include "../zeondefs.hh"
#include "imgui.h"

using namespace z;

void Zeon::DrawSettings() {
	ImGui::SetNextWindowPos({0, ZEON_TOPBAR_HEIGHT}, ImGuiCond_Once);
	ImGui::SetNextWindowSize({350, 200}, ImGuiCond_Once);
	ImGui::Begin("settings", nullptr,
							 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
									 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SeparatorText("User experience");
	ImGui::SliderFloat("Scroll speed", &scrollSpeed, 1.0f, 20.0f);
	ImGui::SliderInt("Max tab name length", &maxTabNameLength, 15, 50);

	ImGui::SeparatorText("Search engine");
	ImGui::Text("Current search engine: %s", searchEngines[currentSearchEngine].name.c_str());
	int i = 0;
	for (Zeon::SearchEngine& se : searchEngines) {
		if (ImGui::Button(se.name.c_str())) {
			currentSearchEngine = i;
		}
		i++;
	}
	ImGui::SeparatorText("Developer");

	if (ImGui::Button("Show devtools")) {
		browsers[active_tab]->GetHost()->ShowDevTools(CefWindowInfo(), nullptr, CefBrowserSettings(),
																									CefPoint());
	}
	ImGui::SameLine();
	if (ImGui::Button("Show imgui demo window")) {
		g_show_demo_window = !g_show_demo_window;
	}

	ImGui::End();
}
