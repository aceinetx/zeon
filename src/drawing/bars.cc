#include "../zeon.hh"
#include "../zeondefs.hh"
#include "imgui.h"
#include <format>

using namespace z;

static void UpdateURL() {
	// update the ui url
	strncpy(g_zeon->ui_url,
					g_zeon->browsers[g_zeon->active_tab]->GetMainFrame()->GetURL().ToString().c_str(),
					sizeof g_zeon->ui_url);
}

void Zeon::DrawTopBar() {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({io.DisplaySize.x, ZEON_TOPBAR_HEIGHT / 2});

	ImGui::Begin("zeon", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetWindowFontScale(1.2f);
	ImGui::Text("zeon");
	ImGui::SetWindowFontScale(1.0f);

	ImGui::SameLine();
	if (ImGui::Button("  ")) {
		browsers[active_tab]->GoBack();
	}
	ImGui::SameLine();
	if (ImGui::Button("  ")) {
		browsers[active_tab]->GoForward();
	}
	ImGui::SameLine();
	if (ImGui::Button("  ")) {
		browsers[active_tab]->Reload();
	}
	ImGui::SameLine();
	if (ImGui::Button("  ")) {
		g_show_settings = !g_show_settings;
	}

	ImGui::SameLine();
	// url input
	if (ImGui::InputText("##url", ui_url, sizeof ui_url, ImGuiInputTextFlags_EnterReturnsTrue)) {
		browsers[active_tab]->GetMainFrame()->LoadURL(CefString(ui_url));
	}
	ImGui::SameLine();
	ImGui::End();
}

void Zeon::DrawTabsBar() {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, ZEON_TOPBAR_HEIGHT / 2});
	ImGui::SetNextWindowSize({io.DisplaySize.x, ZEON_TOPBAR_HEIGHT / 2});

	ImGui::Begin("tabs_bar", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar);
	for (int i = 0; i < browsers.size(); i++) {
		auto& browser = browsers[i];

		std::string tab_label;
		CefRefPtr<CefNavigationEntry> entry = browser->GetHost()->GetVisibleNavigationEntry();
		CefRefPtr<CefFrame> frame = browser->GetFocusedFrame();
		if (entry && entry->IsValid()) {
			tab_label = entry->GetTitle().ToString();
		}
		/* frame becomes a nullptr just before the browser is closing, so we need to check it with
		 * frame.get() */
		if (tab_label.empty() && frame.get()) {
			tab_label = frame->GetURL().ToString();
		}
		/* still empty wtf, make it show "(loading)" */
		if (tab_label.empty()) {
			tab_label = "(loading)";
		}

		if (tab_label.length() > maxTabNameLength) {
			tab_label.erase(tab_label.begin() +
													((maxTabNameLength % 2 == 0) ? maxTabNameLength - 1 : maxTabNameLength),
											tab_label.end());
			tab_label += "...";
		}

		if (ImGui::Button(std::format("{}##tab{}", tab_label, i).c_str())) {
			active_tab = i;
			UpdateURL();
		}
		// don't show the close button if there is only one tab
		if (browsers.size() > 1) {
			ImGui::SameLine();
			if (ImGui::Button(std::format("  ##tab{}", i).c_str())) {
				CloseTab(i);
				UpdateURL();
				break;
			}
		}
		ImGui::SameLine();
	}
	if (ImGui::Button("  ")) {
		OpenTab(searchEngines[currentSearchEngine].defaultUrl);
	}
	ImGui::End();
}
