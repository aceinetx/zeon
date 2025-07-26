#pragma once

#include "browser_client.hh"
#include "render_handler.hh"
#include <cef_app.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <stdlib.h>

namespace z {
extern int g_argc;
extern char** g_argv;
// clang-format off
static constexpr const char* urlRegex = "/(?:http[s]?:\\/\\/.)?(?:www\\.)?[-a-zA-Z0-9@%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b(?:[-a-zA-Z0-9@:%_\\+.~#?&\\/\\/=]*)/gm";
// clang-format on

class Zeon {
private:
	SDL_Renderer* renderer;
	SDL_Window* window;

public:
	CefRefPtr<CefBrowser> browser;
	CefRefPtr<BrowserClient> browserClient;
	CefRefPtr<RenderHandler> renderHandler;
	CefRefPtr<CefCookieManager> cookieManager;

	CefMainArgs cef_args;

	enum {
		BS_LOADING,
		BS_READY,
	} BrowserState = BS_READY;

	float scrollSpeed = 5.0f;

	char ui_url[65535];

private:
	void InitAssets();
	void Draw();
	void ProcessEvent(SDL_Event& event);
	void SetupStyle();

public:
	Zeon();
	int Init();
	void Cleanup();
	void Run();
	~Zeon();
};

extern Zeon* g_zeon;
} // namespace z
