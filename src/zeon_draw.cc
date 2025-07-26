#include "zeon.hh"
#include "zeondefs.hh"
#include <regex>

using namespace z;

static bool g_show_demo_window = false;
static bool g_show_settings = false;

static void DrawState(int state) {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, io.DisplaySize.y - ZEON_TOPBAR_HEIGHT});
	ImGui::SetNextWindowSize({0, io.DisplaySize.y});
	if (state == Zeon::BS_READY)
		return;
	ImGui::Begin("state", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	switch (state) {
	case Zeon::BS_LOADING: {
		std::string url = g_zeon->browser->GetFocusedFrame()->GetURL().ToString();
		ImGui::Text("%s", url.c_str());
	} break;
	case Zeon::BS_READY:
		break;
	}
	ImGui::End();
}

static void DrawTopBar() {
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
		g_zeon->browser->GoBack();
	}
	ImGui::SameLine();
	if (ImGui::Button("forward")) {
		g_zeon->browser->GoForward();
	}
	ImGui::SameLine();
	if (ImGui::Button("settings")) {
		g_show_settings = !g_show_settings;
	}

	ImGui::SameLine();
	if (ImGui::InputText("##url", g_zeon->ui_url, sizeof g_zeon->ui_url,
											 ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (std::regex_match(g_zeon->ui_url, std::regex(z::urlRegex))) {
			g_zeon->browser->GetFocusedFrame()->LoadURL(CefString(g_zeon->ui_url));
		} else {
			// search
			std::string text = std::string(g_zeon->ui_url);
			std::string url = "duckduckgo.com/?t=h_&q=";
			for (char c : text) {
				switch (c) {
				case ' ':
					url += '+';
					break;
				case ':':
					url += "%3A";
					break;
				default:
					url += c;
					break;
				}
			}
			url += "&ia=web";
			g_zeon->browser->GetFocusedFrame()->LoadURL(CefString(url));
		}
	}
	ImGui::SameLine();
	ImGui::End();
}

static void DrawSettings() {
	ImGui::SetNextWindowPos({0, ZEON_TOPBAR_HEIGHT});
	ImGui::SetNextWindowSize({350, 200});
	ImGui::Begin("settings", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
									 ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
	ImGui::SliderFloat("Scroll speed", &g_zeon->scrollSpeed, 1.0f, 20.0f);
	ImGui::End();
}

void Zeon::Draw() {
	DrawTopBar();
	DrawState(BrowserState);
	if (g_show_demo_window)
		ImGui::ShowDemoWindow(nullptr);
	if (g_show_settings)
		DrawSettings();
}
