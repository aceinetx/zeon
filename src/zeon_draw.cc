#include "zeon.hh"
#include "zeondefs.hh"

using namespace z;

static char url[65535];
static bool show_demo_window = false;

static void DrawState(int state) {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, io.DisplaySize.y - ZEON_TOPBAR_HEIGHT});
	ImGui::Begin("state", nullptr,
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
									 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
									 ImGuiWindowFlags_NoBackground);
	switch (state) {
	case Zeon::BS_LOADING:
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("loading");
		break;
	case Zeon::BS_READY:
		break;
	}
	ImGui::End();
}

static void DrawTopBar(CefBrowser* browser) {
	static auto& io = ImGui::GetIO();
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
	ImGui::Checkbox("Demo window", &show_demo_window);
	ImGui::End();
}

void Zeon::Draw() {
	DrawTopBar(browser.get());
	DrawState(BrowserState);
	if (show_demo_window)
		ImGui::ShowDemoWindow(nullptr);
}
