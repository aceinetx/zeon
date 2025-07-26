#include "../zeon.hh"
#include "../zeondefs.hh"

using namespace z;

void Zeon::DrawState() {
	static auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos({0, io.DisplaySize.y - ZEON_TOPBAR_HEIGHT / 2});
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
