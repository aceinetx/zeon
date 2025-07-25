#pragma once

#include "browser_client.hh"
#include "render_handler.hh"
#include <cef_app.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <stdlib.h>

namespace z {
void check_vk_result(VkResult err);

class Zeon {
private:
	SDL_Renderer* renderer;
	SDL_Window* window;

	CefRefPtr<CefBrowser> browser;
	CefRefPtr<BrowserClient> browserClient;
	CefRefPtr<RenderHandler> renderHandler;

private:
	void InitAssets();
	int InitCef();
	void Draw();

public:
	Zeon();
	int Init();
	void Cleanup();
	void Run();
	~Zeon();
};
} // namespace z
