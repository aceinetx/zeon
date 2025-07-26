#pragma once

#include "browser_client.hh"
#include "render_handler.hh"
#include <cef_app.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <stdlib.h>
#include <vector>

namespace z {
extern int g_argc;
extern char** g_argv;

class Zeon {
private:
	SDL_Renderer* renderer;
	SDL_Window* window;

public:
	std::vector<CefRefPtr<CefBrowser>> browsers;
	std::vector<CefRefPtr<BrowserClient>> browserClients;
	std::vector<CefRefPtr<RenderHandler>> renderHandlers;
	CefRefPtr<CefCookieManager> cookieManager;
	int active_tab = 0;
	// Tab management
	int OpenTab(const std::string& url);
	void CloseTab(int idx);
	void SwitchTab(int idx);

	CefMainArgs cef_args;

	struct SearchEngine {
		std::string defaultUrl;
		std::string name;
	};

	std::vector<SearchEngine> searchEngines = {
			{"yandex.ru", "Yandex"},
			{"duckduckgo.com", "Duck duck go"},
			{"google.com", "Google"},
			{"bing.com", "Bing"},
	};
	int currentSearchEngine;

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
	static std::string encodeUrlIntoGetParameter(const std::string& value);

	Zeon();
	int Init();
	void Cleanup();
	void Run();
	~Zeon();
};

extern Zeon* g_zeon;
} // namespace z
