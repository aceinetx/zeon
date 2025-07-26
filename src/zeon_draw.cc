#include "zeon.hh"
#include "zeondefs.hh"

using namespace z;

static char g_url[65535];
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
		std::string url = g_zeon->browser->GetMainFrame()->GetURL().ToString();
		if (!url.empty()) {
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("%s", url.c_str());
			strncpy(g_url, url.c_str(), sizeof url);
		}
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
	if (ImGui::InputText("##url", g_url, sizeof g_url, ImGuiInputTextFlags_EnterReturnsTrue)) {
		g_zeon->browser->GetMainFrame()->LoadURL(CefString(g_url));
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
