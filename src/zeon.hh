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

class Zeon {
private:
	SDL_Renderer* renderer;
	SDL_Window* window;

	CefRefPtr<CefBrowser> browser;
	CefRefPtr<BrowserClient> browserClient;
	CefRefPtr<RenderHandler> renderHandler;

public:
	CefMainArgs cef_args;

	enum {
		BS_LOADING,
		BS_READY,
	} BrowserState = BS_READY;

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
