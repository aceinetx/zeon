#include "imgui.h"
#include "internal/cef_linux.h"
#include "internal/cef_types_wrappers.h"
#include "zeon.hh"
#include "zeondefs.hh"
#include <cef_parser.h>
#include <format>

using namespace z;

static bool g_show_demo_window = false;
static bool g_show_settings = false;
static bool g_show_tabs = false;

static void UpdateURL() {
	strncpy(z::g_zeon->ui_url,
					g_zeon->browsers[g_zeon->active_tab]->GetMainFrame()->GetURL().ToString().c_str(),
					sizeof z::g_zeon->ui_url);
}

void Zeon::DrawState() {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, io.DisplaySize.y - ZEON_TOPBAR_HEIGHT});
	ImGui::SetNextWindowSize({0, io.DisplaySize.y});
	if (BrowserState == BS_READY)
		return;
	ImGui::Begin("state", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	switch (BrowserState) {
	case Zeon::BS_LOADING: {
		std::string url = browsers[active_tab]->GetMainFrame()->GetURL().ToString();
		ImGui::Text("%s", url.c_str());
	} break;
	case Zeon::BS_READY:
		break;
	}
	ImGui::End();
}

void Zeon::DrawTopBar() {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, -1});
	ImGui::SetNextWindowSize({io.DisplaySize.x, ZEON_TOPBAR_HEIGHT});

	ImGui::Begin("zeon", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetWindowFontScale(1.2f);
	ImGui::Text("zeon");
	ImGui::SetWindowFontScale(1.0f);

	ImGui::SameLine();
	if (ImGui::Button("back")) {
		browsers[active_tab]->GoBack();
	}
	ImGui::SameLine();
	if (ImGui::Button("forward")) {
		browsers[active_tab]->GoForward();
	}
	ImGui::SameLine();
	if (ImGui::Button("settings")) {
		g_show_settings = !g_show_settings;
	}
	ImGui::SameLine();
	if (ImGui::Button("tabs")) {
		g_show_tabs = !g_show_tabs;
	}

	ImGui::SameLine();
	if (ImGui::InputText("##url", ui_url, sizeof ui_url, ImGuiInputTextFlags_EnterReturnsTrue)) {
		browsers[active_tab]->GetMainFrame()->LoadURL(CefString(ui_url));
	}
	ImGui::SameLine();
	if (ImGui::Button("Search w/ duckduckgo")) {
		std::string text = std::string(ui_url);
		std::string url = "duckduckgo.com/?t=h_&q=";
		url += Zeon::encodeUrlIntoGetParameter(text);
		url += "&ia=web";
		browsers[active_tab]->GetFocusedFrame()->LoadURL(CefString(url));
	}
	ImGui::End();
}

void Zeon::DrawSettings() {
	ImGui::SetNextWindowPos({0, ZEON_TOPBAR_HEIGHT}, ImGuiCond_Once);
	ImGui::SetNextWindowSize({350, 200}, ImGuiCond_Once);
	ImGui::Begin("settings", nullptr,
							 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
									 ImGuiWindowFlags_NoCollapse);
	ImGui::SliderFloat("Scroll speed", &scrollSpeed, 1.0f, 20.0f);

	ImGui::Text("Current search engine: %s", searchEngines[currentSearchEngine].name.c_str());
	int i = 0;
	for (Zeon::SearchEngine& se : searchEngines) {
		if (ImGui::Button(se.name.c_str())) {
			currentSearchEngine = i;
		}
		i++;
	}
	ImGui::Separator();

	if (ImGui::Button("Show devtools")) {
		browsers[active_tab]->GetHost()->ShowDevTools(CefWindowInfo(), nullptr, CefBrowserSettings(),
																									CefPoint());
	}

	ImGui::End();
}

void Zeon::DrawTabs() {
	ImGui::SetNextWindowPos({0, ZEON_TOPBAR_HEIGHT}, ImGuiCond_Once);
	ImGui::SetNextWindowSize({0, 200}, ImGuiCond_Once);
	ImGui::Begin("tabs", nullptr,
							 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
									 ImGuiWindowFlags_NoCollapse);
	int i = 0;
	for (int i = 0; i < browsers.size(); i++) {
		auto& browser = browsers[i];

		std::string url = browser->GetMainFrame()->GetURL().ToString();
		if (ImGui::Button(std::format("{}##tab{}", url, i).c_str())) {
			active_tab = i;
			UpdateURL();
		}
		if (browsers.size() > 1) {
			ImGui::SameLine();
			if (ImGui::Button(std::format("x##tab{}", i).c_str())) {
				CloseTab(i);
				UpdateURL();
				break;
			}
		}
	}
	if (ImGui::Button("+")) {
		OpenTab(searchEngines[currentSearchEngine].defaultUrl);
	}
	ImGui::End();
}

void Zeon::Draw() {
	DrawTopBar();
	DrawState();
	if (g_show_demo_window)
		ImGui::ShowDemoWindow(nullptr);
	if (g_show_settings)
		DrawSettings();
	if (g_show_tabs)
		DrawTabs();
}
