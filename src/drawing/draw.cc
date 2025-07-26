#include "../zeon.hh"

using namespace z;

bool z::g_show_demo_window = false;
bool z::g_show_settings = false;

void Zeon::Draw() {
	DrawTopBar();
	DrawTabsBar();
	DrawState();
	if (g_show_demo_window)
		ImGui::ShowDemoWindow(nullptr);
	if (g_show_settings)
		DrawSettings();
}
