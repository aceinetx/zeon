#include "zeon.hh"
#include "zeondefs.hh"

using namespace z;

static char url[65535];
static bool show_demo_window = false;

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
	case Zeon::BS_LOADING:
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("%s", g_zeon->browser->GetMainFrame()->GetURL().ToString().c_str());
		break;
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
	if (ImGui::InputText("##url", url, sizeof url, ImGuiInputTextFlags_EnterReturnsTrue)) {
		g_zeon->browser->GetMainFrame()->LoadURL(CefString(url));
	}
	ImGui::SameLine();
	ImGui::End();
}

void Zeon::Draw() {
	DrawTopBar();
	DrawState(BrowserState);
	if (show_demo_window)
		ImGui::ShowDemoWindow(nullptr);
}
